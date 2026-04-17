const inputEl = document.getElementById("hex-input");
const parserTypeEl = document.getElementById("parser-type");
const resultOutputEl = document.getElementById("result-output");
let parseTimer = null;

function formatPrimitive(value) {
  if (typeof value === "number") {
    return String(value);
  }
  if (typeof value === "boolean") return value ? "true" : "false";
  if (value === null) return "null";
  if (value === undefined) return "undefined";
  return String(value);
}

function formatArray(values) {
  if (values.length === 0) return "[]";
  if (values.every((v) => typeof v === "number")) {
    return values
      .map((v) => `0x${Number(v).toString(16).toUpperCase().padStart(2, "0")}`)
      .join(" ");
  }
  return values.map(formatPrimitive).join(", ");
}

function formatParsedValue(value) {
  if (Array.isArray(value)) {
    return formatArray(value);
  }
  if (value && typeof value === "object") {
    return JSON.stringify(value);
  }
  return formatPrimitive(value);
}

function renderParsed(parsed) {
  if (parsed === null || parsed === undefined) {
    resultOutputEl.textContent = String(parsed);
    return;
  }

  if (!parsed || typeof parsed !== "object") {
    resultOutputEl.textContent = formatPrimitive(parsed);
    return;
  }

  const entries = Object.entries(parsed);
  if (entries.length === 0) {
    resultOutputEl.textContent = "{}";
    return;
  }

  resultOutputEl.textContent = entries
    .map(([key, value]) => `${key}: ${formatParsedValue(value)}`)
    .join("\n");
}

function reset() {
  renderParsed({});
}

async function parse() {
  try {
    const response = await fetch("/api/hex/parse", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ input: inputEl.value, type: parserTypeEl.value }),
    });
    const data = await response.json();
    if (!response.ok) {
      throw new Error(data.error || "Parse failed");
    }
    renderParsed(data.parsed);
  } catch (error) {
    resultOutputEl.textContent = String(error && error.message ? error.message : error);
  }
}

function scheduleParse() {
  if (parseTimer) {
    clearTimeout(parseTimer);
  }
  parseTimer = setTimeout(parse, 120);
}

inputEl.addEventListener("input", scheduleParse);
parserTypeEl.addEventListener("change", scheduleParse);
reset();
parse();
