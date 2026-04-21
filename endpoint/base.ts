function normalizeHexToken(token: string): string {
  if (!/^[0-9a-fA-F]+$/.test(token)) {
    throw new Error(`Invalid hex token: ${token}`);
  }

  return token.toUpperCase();
}

function parseTokenToBytes(token: string): number[] {
  const normalized = normalizeHexToken(token);

  if (normalized.length <= 2) {
    return [Number.parseInt(normalized.padStart(2, "0"), 16)];
  }

  if (normalized.length % 2 !== 0) {
    throw new Error(`Token must have even length when longer than 2: ${token}`);
  }

  const bytes: number[] = [];
  for (let index = 0; index < normalized.length; index += 2) {
    bytes.push(Number.parseInt(normalized.slice(index, index + 2), 16));
  }

  return bytes;
}

export function parseHexInput(input: string): number[] {
  const trimmed = input.trim();
  if (!trimmed) {
    return [];
  }

  return trimmed
    .split(/[\s,]+/)
    .filter(Boolean)
    .flatMap(parseTokenToBytes);
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
