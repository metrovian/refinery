import { Router } from "express";
import { listParsers, parserRegistry } from "./registry";
import { ParseType } from "./types";

type ParseBody = {
  input?: string;
  type?: ParseType;
};

export const hexRouter = Router();

hexRouter.get("/parsers", (_req, res) => {
  return res.json({ parsers: listParsers() });
});

hexRouter.post("/parser", (req, res) => {
  const body = req.body as ParseBody;
  const input = body.input;
  const type = body.type ?? "modbus-rtu";

  if (typeof input !== "string") {
    return res.status(400).json({ error: "`input` must be a string." });
  }

  if (!(type in parserRegistry)) {
    return res.status(400).json({ error: `Unsupported parser type: ${String(type)}` });
  }

  const result = parserRegistry[type].parse(input);
  if (!result.ok) return res.status(400).json(result);
  return res.json(result);
});
