import { parseCan } from "./parsers/can";
import { parseModbus } from "./parsers/modbus-rtu";
// [PARSER:REGISTRY_IMPORT]
import { ParseType, Parser } from "./types";

export const parserRegistry: Record<ParseType, Parser> = {
  "can": {
    parse: parseCan,
  },
  "modbus-rtu": {
    parse: parseModbus,
  },
  // [PARSER:REGISTRY_ENTRY]
};

export function listParsers() {
  return (Object.keys(parserRegistry) as ParseType[]).map((type) => ({ type }));
}
