export type ParseType =
  | "modbus-rtu"
  | "midi"
  // [PARSER:PARSE_TYPE]
  ;

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
  parse: (input: string) => ParseResult;
};
