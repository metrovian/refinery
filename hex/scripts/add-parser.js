#!/usr/bin/env node

const fs = require("fs");
const path = require("path");

const MARKERS = {
  parseType: "// [PARSER:PARSE_TYPE]",
  registryImport: "// [PARSER:REGISTRY_IMPORT]",
  registryEntry: "// [PARSER:REGISTRY_ENTRY]",
  uiOption: "<!-- [PARSER:UI_OPTION] -->",
};

function usage() {
  console.log(
    [
      "Usage:",
      "  node hex/scripts/add-parser.js <type> [title] [description]",
      "",
      "Examples:",
      "  node hex/scripts/add-parser.js modbus",
      '  node hex/scripts/add-parser.js nmea "NMEA Sentence" "NMEA 0183 parser"',
      "",
      "Rules:",
      "  - <type> must match /^[a-z][a-z0-9-]*$/",
      "  - This command edits only files in hex/",
    ].join("\n")
  );
}

function fail(message) {
  console.error(`Error: ${message}`);
  process.exit(1);
}

function toPascalCase(value) {
  return value
    .split(/[-_]/g)
    .filter(Boolean)
    .map((part) => part.charAt(0).toUpperCase() + part.slice(1))
    .join("");
}

function toTitle(type) {
  return type
    .split(/[-_]/g)
    .filter(Boolean)
    .map((part) => part.charAt(0).toUpperCase() + part.slice(1))
    .join(" ");
}

function readFile(filePath) {
  return fs.readFileSync(filePath, "utf8");
}

function writeFile(filePath, content) {
  fs.writeFileSync(filePath, content, "utf8");
}

function insertBeforeMarkerLine(content, markerToken, linesToInsert) {
  const lines = content.split("\n");
  const markerIndex = lines.findIndex((line) => line.includes(markerToken));
  if (markerIndex < 0) {
    fail(`Missing marker: ${markerToken}`);
  }

  const markerLine = lines[markerIndex];
  const indentMatch = markerLine.match(/^(\s*)/);
  const indent = indentMatch ? indentMatch[1] : "";

  const normalizedLines = linesToInsert.map((line) => {
    if (!line) return "";
    return `${indent}${line}`;
  });

  const block = normalizedLines.join("\n");
  if (content.includes(block)) {
    return content;
  }

  lines.splice(markerIndex, 0, ...normalizedLines);
  return lines.join("\n");
}

function ensureDir(dirPath) {
  fs.mkdirSync(dirPath, { recursive: true });
}

function parserTemplate(parseFnName, type) {
  return `import { parseBytes, toHexByte } from "./base";
import { ParseResult } from "../types";

export function ${parseFnName}(input: string): ParseResult {
  try {
    const bytes = parseBytes(input);
    return {
      ok: true,
      type: "${type}",
      bytes,
      hex: bytes.map(toHexByte),
      length: bytes.length,
      parsed: {
        mode: "${type}",
        data: bytes.map(toHexByte),
      },
    };
  } catch (error) {
    const message = error instanceof Error ? error.message : "Unknown parse error";
    return { ok: false, type: "${type}", error: message };
  }
}
`;
}

function main() {
  const [, , rawType, rawTitle, rawDescription] = process.argv;
  if (!rawType || rawType === "--help" || rawType === "-h") {
    usage();
    process.exit(rawType ? 0 : 1);
  }

  if (!/^[a-z][a-z0-9-]*$/.test(rawType)) {
    fail("Invalid <type>. Use lowercase letters, numbers, and hyphen only.");
  }

  const type = rawType;
  const title = rawTitle || toTitle(type);
  const description = rawDescription || `${title} parser`;
  const parseFnName = `parse${toPascalCase(type)}`;

  const hexDir = path.resolve(__dirname, "..");
  const parserDir = path.join(hexDir, "parsers");
  const parserFile = path.join(parserDir, `${type}.ts`);
  const typesFile = path.join(hexDir, "types.ts");
  const registryFile = path.join(hexDir, "registry.ts");
  const uiFile = path.join(hexDir, "ui.ts");

  if (fs.existsSync(parserFile)) {
    fail(`Parser file already exists: ${parserFile}`);
  }

  const typesContent = readFile(typesFile);
  if (typesContent.includes(`"${type}"`)) {
    fail(`ParseType already includes "${type}".`);
  }

  const registryContent = readFile(registryFile);
  if (registryContent.includes(`"./parsers/${type}"`)) {
    fail(`Registry already imports parser "${type}".`);
  }

  const uiContent = readFile(uiFile);
  if (uiContent.includes(`<option value="${type}">`)) {
    fail(`UI option already exists for "${type}".`);
  }

  ensureDir(parserDir);
  writeFile(parserFile, parserTemplate(parseFnName, type));

  const nextTypes = insertBeforeMarkerLine(typesContent, MARKERS.parseType, [`| "${type}"`]);
  writeFile(typesFile, nextTypes);

  const nextRegistryImport = insertBeforeMarkerLine(
    registryContent,
    MARKERS.registryImport,
    [`import { ${parseFnName} } from "./parsers/${type}";`]
  );
  const registryEntryLines = [
    `${JSON.stringify(type)}: {`,
    `  type: ${JSON.stringify(type)},`,
    `  title: ${JSON.stringify(title)},`,
    `  description: ${JSON.stringify(description)},`,
    `  parse: ${parseFnName},`,
    "},",
  ];
  const nextRegistry = insertBeforeMarkerLine(
    nextRegistryImport,
    MARKERS.registryEntry,
    registryEntryLines
  );
  writeFile(registryFile, nextRegistry);

  const nextUi = insertBeforeMarkerLine(
    uiContent,
    MARKERS.uiOption,
    [`<option value="${type}">${title}</option>`]
  );
  writeFile(uiFile, nextUi);

  console.log(`Added parser "${type}"`);
  console.log(`- ${path.relative(process.cwd(), parserFile)}`);
  console.log(`- ${path.relative(process.cwd(), typesFile)}`);
  console.log(`- ${path.relative(process.cwd(), registryFile)}`);
  console.log(`- ${path.relative(process.cwd(), uiFile)}`);
}

main();
