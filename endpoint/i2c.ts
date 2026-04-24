import { formatHex, parseHexInput, parseNumber } from "./base";
import { runPythonHelper } from "./helper";

const DEFAULT_I2C_DEVICE = "/dev/i2c-1";

export type I2cRequest = {
  input: string;
  devicePath?: string;
  address?: string;
  speed?: number;
  readLength?: number;
  timeout?: number;
};

export type I2cConfig = {
  devicePath: string;
  address: string;
  addressValue: number;
  speed: number;
  readLength: number;
  timeout: number;
};

export type I2cTransferResult = {
  ok: true;
  driver: "i2c";
  config: I2cConfig;
  tx: {
    bytes: number[];
    hex: string;
    length: number;
  };
  rx: {
    bytes: number[];
    hex: string;
    length: number;
  };
};

type I2cHelperResult = I2cTransferResult;

function resolveI2cDevice(devicePath: string): string {
  const trimmed = devicePath.trim();
  if (!trimmed) {
    return DEFAULT_I2C_DEVICE;
  }

  if (/^\/dev\/i2c-\d+$/.test(trimmed)) {
    return trimmed;
  }

  throw new Error("`devicePath` must be a /dev/i2c-X path.");
}

function parseI2cAddress(value: string): { address: string; addressValue: number } {
  const trimmed = value.trim();
  if (!trimmed) {
    throw new Error("`address` is required.");
  }

  const normalized = trimmed.toLowerCase().startsWith("0x") ? trimmed.slice(2) : trimmed;
  if (!/^[0-9a-fA-F]{1,2}$/.test(normalized)) {
    throw new Error("`address` must be a 7-bit hex value like 0x3C.");
  }

  const addressValue = Number.parseInt(normalized, 16);
  if (addressValue < 0x03 || addressValue > 0x77) {
    throw new Error("`address` must be between 0x03 and 0x77.");
  }

  return {
    address: `0x${addressValue.toString(16).toUpperCase().padStart(2, "0")}`,
    addressValue,
  };
}

function readI2cConfig(body: I2cRequest): I2cConfig {
  const devicePath = resolveI2cDevice(typeof body.devicePath === "string" ? body.devicePath : "");
  const { address, addressValue } = parseI2cAddress(typeof body.address === "string" ? body.address : "");
  const speed = parseNumber(body.speed, "speed", 100_000);
  const readLength = parseNumber(body.readLength, "readLength", 0);
  const timeout = parseNumber(body.timeout, "timeout", 0);

  if (!Number.isInteger(speed) || speed <= 0) {
    throw new Error("`speed` must be a positive integer.");
  }

  if (!Number.isInteger(readLength) || readLength < 0 || readLength > 4096) {
    throw new Error("`readLength` must be an integer between 0 and 4096.");
  }

  if (!Number.isInteger(timeout) || timeout < 0 || timeout > 60_000) {
    throw new Error("`timeout` must be an integer between 0 and 60000.");
  }

  return {
    devicePath,
    address,
    addressValue,
    speed,
    readLength,
    timeout,
  };
}

export async function transferOverI2c(body: I2cRequest): Promise<I2cTransferResult> {
  if (typeof body.input !== "string") {
    throw new Error("`input` must be a string.");
  }

  const txBytes = parseHexInput(body.input);
  const config = readI2cConfig(body);
  const payload = await runPythonHelper<I2cHelperResult>(
    "i2c_transfer.py",
    {
      devicePath: config.devicePath,
      address: config.addressValue,
      speed: config.speed,
      readLength: config.readLength,
      timeout: config.timeout,
      txBytes,
    },
    "I2C transfer failed.",
  );

  return {
    ...payload,
    config: {
      ...payload.config,
      address: config.address,
    },
    tx: {
      bytes: txBytes,
      hex: formatHex(txBytes),
      length: txBytes.length,
    },
  };
}
