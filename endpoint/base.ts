function normalizeHexToken(token: string): string {
  if (!/^[0-9a-fA-F]{1,2}$/.test(token)) {
    throw new Error(`Invalid hex byte: ${token}`);
  }

  return token.padStart(2, "0").toUpperCase();
}

export function parseHexInput(input: string): number[] {
  const trimmed = input.trim();
  if (!trimmed) {
    return [];
  }

  return trimmed
    .split(/[\s,]+/)
    .filter(Boolean)
    .map((token) => Number.parseInt(normalizeHexToken(token), 16));
}

export function formatHex(bytes: number[]): string {
  if (bytes.length === 0) {
    return "";
  }

  return bytes.map((value) => value.toString(16).padStart(2, "0").toUpperCase()).join(" ");
}

export function parseNumber(value: unknown, field: string, fallback: number): number {
  if (value === undefined) {
    return fallback;
  }

  if (typeof value !== "number" || Number.isNaN(value) || !Number.isFinite(value)) {
    throw new Error(`\`${field}\` must be a number.`);
  }

  return value;
}
