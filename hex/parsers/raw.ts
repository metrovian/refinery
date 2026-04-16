import { parseBytes, toHexByte } from "./base";
import { ParseResult } from "../types";

export function parseRaw(input: string): ParseResult {
  try {
    const bytes = parseBytes(input);
    return {
      ok: true,
      type: "raw",
      bytes,
      hex: bytes.map(toHexByte),
      length: bytes.length,
      parsed: {
        mode: "raw",
      },
    };
  } catch (error) {
    const message = error instanceof Error ? error.message : "Unknown parse error";
    return { ok: false, type: "raw", error: message };
  }
}
