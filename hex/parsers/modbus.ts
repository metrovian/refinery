import { parseBytes, toHexByte } from "./base";
import { ParseResult } from "../types";

function crcDisplay(crcLo: number, crcHi: number): string {
  return `0x${toHexByte(crcHi)}${toHexByte(crcLo)}`;
}

export function parseModbus(input: string): ParseResult {
  try {
    const bytes = parseBytes(input);
    if (bytes.length < 4) {
      throw new Error("MODBUS RTU frame requires at least 4 bytes.");
    }

    const slave = bytes[0];
    const functionCode = bytes[1];
    const crcLo = bytes[bytes.length - 2];
    const crcHi = bytes[bytes.length - 1];
    const payload = bytes.slice(2, -2);

    return {
      ok: true,
      type: "modbus",
      bytes,
      hex: bytes.map(toHexByte),
      length: bytes.length,
      parsed: {
        mode: "modbus-rtu",
        slaveAddress: slave,
        functionCode,
        payload: payload.map(toHexByte),
        crc: crcDisplay(crcLo, crcHi),
      },
    };
  } catch (error) {
    const message = error instanceof Error ? error.message : "Unknown parse error";
    return { ok: false, type: "modbus", error: message };
  }
}
