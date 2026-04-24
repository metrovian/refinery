export { formatHex, parseHexInput } from "../server/hex";

export function parseNumber(value: unknown, field: string, fallback: number): number {
  if (value === undefined) {
    return fallback;
  }

  if (typeof value !== "number" || Number.isNaN(value) || !Number.isFinite(value)) {
    throw new Error(`\`${field}\` must be a number.`);
  }

  return value;
}
