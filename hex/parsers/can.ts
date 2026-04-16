import { parseBytes, tokenize, toHexByte } from "./base";
import { ParseResult } from "../types";

export function parseCan(input: string): ParseResult {
  try {
    const tokens = tokenize(input);
    if (tokens.length < 1) {
      throw new Error("CAN input must include at least an arbitration ID.");
    }

    const idToken = tokens[0].replace(/^0x/i, "");
    if (!/^[0-9a-fA-F]+$/.test(idToken) || idToken.length > 8) {
      throw new Error("CAN ID must be hex and up to 8 characters.");
    }

    const id = parseInt(idToken, 16);
    const dataInput = tokens.slice(1).join(" ");
    const dataBytes = dataInput ? parseBytes(dataInput) : [];
    if (dataBytes.length > 8) {
      throw new Error("Classic CAN payload supports up to 8 bytes.");
    }

    const allBytes = dataBytes;
    return {
      ok: true,
      type: "can",
      bytes: allBytes,
      hex: allBytes.map(toHexByte),
      length: allBytes.length,
      parsed: {
        mode: "can",
        arbitrationId: `0x${id.toString(16).toUpperCase()}`,
        isExtendedId: idToken.length > 3,
        dlc: dataBytes.length,
        data: dataBytes.map(toHexByte),
      },
    };
  } catch (error) {
    const message = error instanceof Error ? error.message : "Unknown parse error";
    return { ok: false, type: "can", error: message };
  }
}
