import { Router } from "express";
import { listParsers, parserRegistry } from "./registry";
import { ParseType } from "./types";

type ParseBody = {
  input?: string;
  type?: ParseType;
};

export const hexRouter = Router();

function isParseType(value: unknown): value is ParseType {
  return typeof value === "string" && value in parserRegistry;
}

function readParseRequest(body: ParseBody): { input: string; type: ParseType } {
  if (typeof body.input !== "string") {
    throw new Error("`input` must be a string.");
  }

  const type = body.type ?? "modbus-rtu";
  if (!isParseType(type)) {
    throw new Error(`Unsupported parser type: ${String(type)}`);
  }

  return { input: body.input, type };
}

hexRouter.get("/parsers", (_req, res) => {
  return res.json({ parsers: listParsers() });
});

hexRouter.post("/parser", (req, res) => {
  let input: string;
  let type: ParseType;

  try {
    ({ input, type } = readParseRequest(req.body as ParseBody));
  } catch (error) {
    const message = error instanceof Error ? error.message : "Invalid parse request.";
    return res.status(400).json({ error: message });
  }

  const result = parserRegistry[type].parse(input);
  if (!result.ok) return res.status(400).json(result);
  return res.json(result);
});
