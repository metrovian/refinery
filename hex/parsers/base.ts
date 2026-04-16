export function toHexByte(value: number): string {
  return value.toString(16).toUpperCase().padStart(2, "0");
}

export function tokenize(input: string): string[] {
  return input.trim().split(/[\s,;:-]+/).filter(Boolean);
}

export function parseTokenToBytes(token: string): number[] {
  const normalized = token.replace(/^0x/i, "");
  if (!normalized) {
    throw new Error("Empty token is not allowed.");
  }
  if (!/^[0-9a-fA-F]+$/.test(normalized)) {
    throw new Error(`Invalid hex token: ${token}`);
  }
  if (normalized.length <= 2) {
    return [parseInt(normalized, 16)];
  }
  if (normalized.length % 2 !== 0) {
    throw new Error(`Token must have even length when longer than 2: ${token}`);
  }

  const bytes: number[] = [];
  for (let i = 0; i < normalized.length; i += 2) {
    bytes.push(parseInt(normalized.slice(i, i + 2), 16));
  }
  return bytes;
}

export function parseBytes(input: string): number[] {
  const trimmed = input.trim();
  if (!trimmed) return [];
  return tokenize(trimmed).flatMap(parseTokenToBytes);
}
