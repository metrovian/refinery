export { parseHexInput as parseBytes, parseHexTokenToBytes as parseTokenToBytes, toHexByte } from "../../server/hex";

export function tokenize(input: string): string[] {
  return input.trim().split(/[\s,;:-]+/).filter(Boolean);
}
