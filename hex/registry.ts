import { parseCan } from "./parsers/can";
import { ParseType, Parser } from "./types";

export const parserRegistry: Record<ParseType, Parser> = {
  can: {
    type: "can",
    title: "CAN Frame",
    description: "ID + up to 8 data bytes. Example: 18FEF100 01 0A FF 1C",
    parse: parseCan,
  },
};

export function listParsers() {
  return Object.values(parserRegistry).map(({ type, title, description }) => ({
    type,
    title,
    description,
  }));
}
