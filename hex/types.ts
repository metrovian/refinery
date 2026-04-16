export type ParseType = "raw" | "can" | "modbus";

export type ParseSuccess = {
  ok: true;
  type: ParseType;
  bytes: number[];
  hex: string[];
  length: number;
  parsed: Record<string, unknown>;
};

export type ParseFailure = {
  ok: false;
  type: ParseType;
  error: string;
};

export type ParseResult = ParseSuccess | ParseFailure;

export type Parser = {
  type: ParseType;
  title: string;
  description: string;
  parse: (input: string) => ParseResult;
};
