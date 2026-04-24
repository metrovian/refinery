export function toHexByte(value: number): string {
  return value.toString(16).toUpperCase().padStart(2, "0");
}

export function formatHex(bytes: number[]): string {
  if (bytes.length === 0) {
    return "";
  }

  return bytes.map(toHexByte).join(" ");
}

function normalizeHexToken(token: string): string {
  if (!token) {
    throw new Error("Empty token is not allowed.");
  }

  if (!/^[0-9a-fA-F]+$/.test(token)) {
    throw new Error(`Invalid hex token: ${token}`);
  }

  return token.toUpperCase();
}

export function parseHexTokenToBytes(token: string): number[] {
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
    .split(/[\s,;:-]+/)
    .filter(Boolean)
    .flatMap(parseHexTokenToBytes);
}
