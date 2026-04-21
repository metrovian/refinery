import { execFile } from "child_process";
import { open } from "fs/promises";
import { promisify } from "util";
import { formatHex, parseHexInput, parseNumber } from "./base";

const execFileAsync = promisify(execFile);
const DEFAULT_UART_DEVICE = "/dev/serial0";
const MAX_TIMEOUT_MS = 25_500;
const MAX_READ_BYTES = 4096;

export type UartParity = "none" | "even" | "odd";
export type UartDataBits = 7 | 8;
export type UartStopBits = 1 | 2;

export type UartRequest = {
  input: string;
  devicePath?: string;
  baudRate?: number;
  dataBits?: number;
  parity?: string;
  stopBits?: number;
  timeout?: number;
};

export type UartConfig = {
  devicePath: string;
  baudRate: number;
  dataBits: UartDataBits;
  parity: UartParity;
  stopBits: UartStopBits;
  timeout: number;
};

export type UartTransferResult = {
  ok: true;
  driver: "uart";
  config: UartConfig;
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

function readUartConfig(body: UartRequest): UartConfig {
  const devicePath = typeof body.devicePath === "string" && body.devicePath.trim() ? body.devicePath.trim() : DEFAULT_UART_DEVICE;
  const baudRate = parseNumber(body.baudRate, "baudRate", 115200);
  const dataBits = parseNumber(body.dataBits, "dataBits", 8);
  const stopBits = parseNumber(body.stopBits, "stopBits", 1);
  const timeout = parseNumber(body.timeout, "timeout", 1000);
  const parity = body.parity ?? "none";

  if (!Number.isInteger(baudRate) || baudRate <= 0) {
    throw new Error("`baudRate` must be a positive integer.");
  }

  if (dataBits !== 7 && dataBits !== 8) {
    throw new Error("`dataBits` must be 7 or 8.");
  }

  if (parity !== "none" && parity !== "even" && parity !== "odd") {
    throw new Error("`parity` must be one of: none, even, odd.");
  }

  if (stopBits !== 1 && stopBits !== 2) {
    throw new Error("`stopBits` must be 1 or 2.");
  }

  if (!Number.isInteger(timeout) || timeout < 0 || timeout > MAX_TIMEOUT_MS) {
    throw new Error("`timeout` must be an integer between 0 and 25500.");
  }

  return {
    devicePath,
    baudRate,
    dataBits,
    parity,
    stopBits,
    timeout,
  };
}

function buildSttyArgs(config: UartConfig): string[] {
  const parityArgs =
    config.parity === "none"
      ? ["-parenb", "-parodd"]
      : config.parity === "even"
        ? ["parenb", "-parodd"]
        : ["parenb", "parodd"];

  const stopBitArg = config.stopBits === 2 ? "cstopb" : "-cstopb";
  const charSizeArg = config.dataBits === 7 ? "cs7" : "cs8";
  const readTimeoutDeciseconds = Math.max(0, Math.min(255, Math.ceil(config.timeout / 100)));

  return [
    "-F",
    config.devicePath,
    String(config.baudRate),
    "raw",
    "-echo",
    "-echoe",
    "-echok",
    "-echonl",
    "-icanon",
    "-isig",
    "-iexten",
    "-ixon",
    "-ixoff",
    "-icrnl",
    "-inlcr",
    "-igncr",
    "-opost",
    "-onlcr",
    charSizeArg,
    ...parityArgs,
    stopBitArg,
    "min",
    "0",
    "time",
    String(readTimeoutDeciseconds),
  ];
}

async function configureUart(config: UartConfig): Promise<void> {
  try {
    await execFileAsync("stty", buildSttyArgs(config));
  } catch (error) {
    const stderr = error instanceof Error && "stderr" in error ? String(error.stderr ?? "") : "";
    const detail = stderr.trim();
    throw new Error(detail || `Failed to configure UART device: ${config.devicePath}`);
  }
}

export async function transferOverUart(body: UartRequest): Promise<UartTransferResult> {
  if (typeof body.input !== "string") {
    throw new Error("`input` must be a string.");
  }

  const config = readUartConfig(body);
  const txBytes = parseHexInput(body.input);
  const txBuffer = Buffer.from(txBytes);

  await configureUart(config);

  const device = await open(config.devicePath, "r+");

  try {
    await device.write(txBuffer, 0, txBuffer.length, null);

    const chunks: Buffer[] = [];
    let totalLength = 0;

    while (totalLength < MAX_READ_BYTES) {
      const readBuffer = Buffer.alloc(Math.min(256, MAX_READ_BYTES - totalLength));
      const { bytesRead } = await device.read(readBuffer, 0, readBuffer.length, null);

      if (bytesRead === 0) {
        break;
      }

      chunks.push(readBuffer.subarray(0, bytesRead));
      totalLength += bytesRead;
    }

    const rxBuffer = chunks.length > 0 ? Buffer.concat(chunks, totalLength) : Buffer.alloc(0);
    const rxBytes = Array.from(rxBuffer.values());

    return {
      ok: true,
      driver: "uart",
      config,
      tx: {
        bytes: txBytes,
        hex: formatHex(txBytes),
        length: txBytes.length,
      },
      rx: {
        bytes: rxBytes,
        hex: formatHex(rxBytes),
        length: rxBytes.length,
      },
    };
  } finally {
    await device.close();
  }
}
