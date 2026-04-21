import { execFile } from "child_process";
import path from "path";
import { promisify } from "util";

const execFileAsync = promisify(execFile);
const DEFAULT_SPI_DEVICE = "/dev/spidev0.0";

export type SpiRequest = {
  input: string;
  chipSelect?: string;
  speed?: number;
  mode?: number;
  bitsPerWord?: number;
  delay?: number;
};

export type SpiConfig = {
  devicePath: string;
  speed: number;
  mode: 0 | 1 | 2 | 3;
  bitsPerWord: number;
  delay: number;
};

export type SpiTransferResult = {
  ok: true;
  driver: "spi";
  config: SpiConfig;
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

type SpiHelperResult = {
  ok: true;
  driver: "spi";
  config: SpiConfig;
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

function normalizeHexToken(token: string): string {
  if (!/^[0-9a-fA-F]{1,2}$/.test(token)) {
    throw new Error(`Invalid hex byte: ${token}`);
  }

  return token.padStart(2, "0").toUpperCase();
}

function parseHexInput(input: string): number[] {
  const trimmed = input.trim();
  if (!trimmed) {
    throw new Error("`input` must include at least one hex byte.");
  }

  return trimmed
    .split(/[\s,]+/)
    .filter(Boolean)
    .map((token) => Number.parseInt(normalizeHexToken(token), 16));
}

function parseNumber(value: unknown, field: string, fallback: number): number {
  if (value === undefined) {
    return fallback;
  }

  if (typeof value !== "number" || Number.isNaN(value) || !Number.isFinite(value)) {
    throw new Error(`\`${field}\` must be a number.`);
  }

  return value;
}

function resolveSpiDevice(devicePath: string): string {
  const trimmed = devicePath.trim();
  if (!trimmed) {
    return DEFAULT_SPI_DEVICE;
  }

  if (/^\/dev\/spidev\d+\.\d+$/.test(trimmed)) {
    return trimmed;
  }

  throw new Error("`chipSelect` must be a /dev/spidevX.Y path.");
}

function readSpiConfig(body: SpiRequest): SpiConfig {
  const chipSelect = typeof body.chipSelect === "string" ? body.chipSelect : "";
  const speed = parseNumber(body.speed, "speed", 1_000_000);
  const mode = parseNumber(body.mode, "mode", 0);
  const bitsPerWord = parseNumber(body.bitsPerWord, "bitsPerWord", 8);
  const delay = parseNumber(body.delay, "delay", 0);

  if (!Number.isInteger(speed) || speed <= 0) {
    throw new Error("`speed` must be a positive integer.");
  }

  if (mode !== 0 && mode !== 1 && mode !== 2 && mode !== 3) {
    throw new Error("`mode` must be 0, 1, 2, or 3.");
  }

  if (!Number.isInteger(bitsPerWord) || bitsPerWord < 4 || bitsPerWord > 16) {
    throw new Error("`bitsPerWord` must be an integer between 4 and 16.");
  }

  if (!Number.isInteger(delay) || delay < 0 || delay > 65_535) {
    throw new Error("`delay` must be an integer between 0 and 65535.");
  }

  return {
    devicePath: resolveSpiDevice(chipSelect),
    speed,
    mode,
    bitsPerWord,
    delay,
  };
}

export async function transferOverSpi(body: SpiRequest): Promise<SpiTransferResult> {
  if (typeof body.input !== "string") {
    throw new Error("`input` must be a string.");
  }

  const txBytes = parseHexInput(body.input);
  const config = readSpiConfig(body);
  const helperPath = path.join(process.cwd(), "endpoint", "scripts", "spi_transfer.py");

  try {
    const { stdout, stderr } = await execFileAsync("python3", [
      helperPath,
      JSON.stringify({
        devicePath: config.devicePath,
        speed: config.speed,
        mode: config.mode,
        bitsPerWord: config.bitsPerWord,
        delay: config.delay,
        txBytes,
      }),
    ]);
    const payload = JSON.parse(stdout) as SpiHelperResult | { ok: false; error?: string };

    if (!payload || payload.ok !== true) {
      const message = !payload || typeof payload.error !== "string" ? stderr.trim() || "SPI transfer failed." : payload.error;
      throw new Error(message);
    }

    return payload;
  } catch (error) {
    const stderr = error instanceof Error && "stderr" in error ? String(error.stderr ?? "") : "";
    const detail = stderr.trim();
    const message = error instanceof Error ? error.message : "SPI transfer failed.";
    throw new Error(detail || message);
  }
}
