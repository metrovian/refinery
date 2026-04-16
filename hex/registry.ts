import { parseCan } from "./parsers/can";
import { parseModbus } from "./parsers/modbus";
import { parseRaw } from "./parsers/raw";
import { ParseType, Parser } from "./types";

export const parserRegistry: Record<ParseType, Parser> = {
  raw: {
    type: "raw",
    title: "Raw Bytes",
    description: "Hex bytes only. Example: 0A FF 1C",
    parse: parseRaw,
  },
  can: {
    type: "can",
    title: "CAN Frame",
    description: "ID + up to 8 data bytes. Example: 18FEF100 01 0A FF 1C",
    parse: parseCan,
  },
  modbus: {
    type: "modbus",
    title: "MODBUS RTU",
    description: "RTU frame with CRC. Example: 01 03 00 6B 00 03 76 87",
    parse: parseModbus,
  },
};

export function listParsers() {
  return Object.values(parserRegistry).map(({ type, title, description }) => ({
    type,
    title,
    description,
  }));
}
