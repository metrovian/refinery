import { parseModbusRtu } from "./parsers/modbus-rtu";
import { parseMidi } from "./parsers/midi";
// [PARSER:REGISTRY_IMPORT]
import { ParseType, Parser } from "./types";

function formatParserLabel(type: ParseType): string {
  return type.toUpperCase();
}

export const parserRegistry: Record<ParseType, Parser> = {
  "modbus-rtu": {
    parse: parseModbusRtu,
  },
  "midi": {
    parse: parseMidi,
  },
  // [PARSER:REGISTRY_ENTRY]
};

export function listParsers() {
  return (Object.keys(parserRegistry) as ParseType[]).map((type) => ({
    type,
    label: formatParserLabel(type),
  }));
}
