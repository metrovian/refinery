import { parseBytes, toHexByte } from "./base";
import { ParseResult } from "../types";

function readU16BE(bytes: number[], offset: number): number {
  return (bytes[offset] << 8) | bytes[offset + 1];
}

function modbusCrc16(frame: number[]): number {
  let crc = 0xffff;
  for (const byte of frame) {
    crc ^= byte;
    for (let bit = 0; bit < 8; bit += 1) {
      const lsb = crc & 0x0001;
      crc >>= 1;
      if (lsb) {
        crc ^= 0xa001;
      }
    }
  }
  return crc & 0xffff;
}

function toHexWord(value: number): string {
  return `0x${value.toString(16).toUpperCase().padStart(4, "0")}`;
}

function functionName(functionCode: number): string {
  const isException = (functionCode & 0x80) !== 0;
  const baseCode = functionCode & 0x7f;

  let name: string;
  switch (baseCode) {
    case 0x01:
      name = "read coils";
      break;
    case 0x02:
      name = "read discrete inputs";
      break;
    case 0x03:
      name = "read holding registers";
      break;
    case 0x04:
      name = "read input registers";
      break;
    case 0x05:
      name = "write single coil";
      break;
    case 0x06:
      name = "write single register";
      break;
    case 0x0f:
      name = "write multiple coils";
      break;
    case 0x10:
      name = "write multiple registers";
      break;
    default:
      name = "unknown";
  }
  return isException ? `${name} (exception)` : name;
}

function decodePdu(functionCode: number, payload: number[]): Record<string, unknown> {
  if (payload.length === 0) {
    return { payload: [] };
  }

  switch (functionCode) {
    case 0x01:
    case 0x02: {
      if (payload.length === 4) {
        return {
          frameKind: "request",
          startAddress: toHexWord(readU16BE(payload, 0)),
          quantity: readU16BE(payload, 2),
        };
      }
      if (payload.length >= 1) {
        const byteCount = payload[0];
        const status = payload.slice(1);
        return {
          frameKind: "response",
          byteCount,
          statusBytes: status.map(toHexByte),
        };
      }
      return { payload: payload.map(toHexByte) };
    }
    case 0x03:
    case 0x04: {
      if (payload.length === 4) {
        return {
          frameKind: "request",
          startAddress: toHexWord(readU16BE(payload, 0)),
          quantity: readU16BE(payload, 2),
        };
      }
      if (payload.length >= 1) {
        const byteCount = payload[0];
        const dataBytes = payload.slice(1);
        const registerValues: string[] = [];
        for (let i = 0; i + 1 < dataBytes.length; i += 2) {
          registerValues.push(toHexWord(readU16BE(dataBytes, i)));
        }
        return {
          frameKind: "response",
          byteCount,
          registerBytes: dataBytes.map(toHexByte),
          registerValues,
        };
      }
      return { payload: payload.map(toHexByte) };
    }
    case 0x05: {
      if (payload.length === 4) {
        const rawValue = readU16BE(payload, 2);
        return {
          frameKind: "request_or_response",
          outputAddress: toHexWord(readU16BE(payload, 0)),
          value: toHexWord(rawValue),
          state: rawValue === 0xff00 ? "ON" : rawValue === 0x0000 ? "OFF" : "INVALID",
        };
      }
      return { payload: payload.map(toHexByte) };
    }
    case 0x06: {
      if (payload.length === 4) {
        return {
          frameKind: "request_or_response",
          registerAddress: toHexWord(readU16BE(payload, 0)),
          value: toHexWord(readU16BE(payload, 2)),
        };
      }
      return { payload: payload.map(toHexByte) };
    }
    case 0x0f: {
      if (payload.length === 4) {
        return {
          frameKind: "response",
          startAddress: toHexWord(readU16BE(payload, 0)),
          quantity: readU16BE(payload, 2),
        };
      }
      if (payload.length >= 5) {
        const byteCount = payload[4];
        return {
          frameKind: "request",
          startAddress: toHexWord(readU16BE(payload, 0)),
          quantity: readU16BE(payload, 2),
          byteCount,
          values: payload.slice(5).map(toHexByte),
        };
      }
      return { payload: payload.map(toHexByte) };
    }
    case 0x10: {
      if (payload.length === 4) {
        return {
          frameKind: "response",
          startAddress: toHexWord(readU16BE(payload, 0)),
          quantity: readU16BE(payload, 2),
        };
      }
      if (payload.length >= 5) {
        const byteCount = payload[4];
        const dataBytes = payload.slice(5);
        const registerValues: string[] = [];
        for (let i = 0; i + 1 < dataBytes.length; i += 2) {
          registerValues.push(toHexWord(readU16BE(dataBytes, i)));
        }
        return {
          frameKind: "request",
          startAddress: toHexWord(readU16BE(payload, 0)),
          quantity: readU16BE(payload, 2),
          byteCount,
          registerBytes: dataBytes.map(toHexByte),
          registerValues,
        };
      }
      return { payload: payload.map(toHexByte) };
    }
    default:
      return { payload: payload.map(toHexByte) };
  }
}

export function parseModbus(input: string): ParseResult {
  try {
    const bytes = parseBytes(input);
    if (bytes.length < 4) {
      throw new Error("MODBUS RTU frame must include address, function, and CRC(2 bytes).");
    }

    const slaveAddress = bytes[0];
    const functionCodeRaw = bytes[1];
    const isException = (functionCodeRaw & 0x80) !== 0;
    const baseFunctionCode = functionCodeRaw & 0x7f;
    const payload = bytes.slice(2, -2);

    const crcReceived = bytes[bytes.length - 2] | (bytes[bytes.length - 1] << 8);
    const crcCalculated = modbusCrc16(bytes.slice(0, -2));

    const parsed: Record<string, unknown> = {
      parser: "modbus-rtu",
      slaveAddress: toHexByte(slaveAddress),
      functionCode: toHexByte(functionCodeRaw),
      functionName: functionName(baseFunctionCode),
      payload: payload.map(toHexByte),
      crcReceived: toHexWord(crcReceived),
      crcCalculated: toHexWord(crcCalculated),
      crcValid: crcReceived === crcCalculated,
    };

    if (isException) {
      parsed.exception = true;
      parsed.originalFunctionCode = toHexByte(baseFunctionCode);
      parsed.exceptionCode = payload.length > 0 ? toHexByte(payload[0]) : "MISSING";
    } else {
      Object.assign(parsed, decodePdu(baseFunctionCode, payload));
    }

    return {
      ok: true,
      type: "modbus-rtu",
      bytes,
      hex: bytes.map(toHexByte),
      length: bytes.length,
      parsed,
    };
  } catch (error) {
    const message = error instanceof Error ? error.message : "Unknown parse error";
    return { ok: false, type: "modbus-rtu", error: message };
  }
}
