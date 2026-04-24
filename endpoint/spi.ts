import { parseHexInput, parseNumber } from "./base";
import { runPythonHelper } from "./helper";

const DEFAULT_SPI_DEVICE = "/dev/spidev0.0";

export type SpiRequest = {
  input: string;
  devicePath?: string;
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

function resolveSpiDevice(devicePath: string): string {
  const trimmed = devicePath.trim();
  if (!trimmed) {
    return DEFAULT_SPI_DEVICE;
  }

  if (/^\/dev\/spidev\d+\.\d+$/.test(trimmed)) {
    return trimmed;
  }

  throw new Error("`devicePath` must be a /dev/spidevX.Y path.");
}

function readSpiConfig(body: SpiRequest): SpiConfig {
  const devicePath = typeof body.devicePath === "string" ? body.devicePath : "";
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
    devicePath: resolveSpiDevice(devicePath),
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

  return runPythonHelper<SpiHelperResult>(
    "spi_transfer.py",
    {
      devicePath: config.devicePath,
      speed: config.speed,
      mode: config.mode,
      bitsPerWord: config.bitsPerWord,
      delay: config.delay,
      txBytes,
    },
    "SPI transfer failed.",
  );
}
