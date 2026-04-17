import { parseModbusRtu } from "./parsers/modbus-rtu";
// [PARSER:REGISTRY_IMPORT]
import { ParseType, Parser } from "./types";

export const parserRegistry: Record<ParseType, Parser> = {
  "modbus-rtu": {
    parse: parseModbusRtu,
  },
  // [PARSER:REGISTRY_ENTRY]
};

export function listParsers() {
  return (Object.keys(parserRegistry) as ParseType[]).map((type) => ({ type }));
}
