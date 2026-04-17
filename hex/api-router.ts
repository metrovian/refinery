import { Router } from "express";
import { listParsers, parserRegistry } from "./registry";
import { ParseType } from "./types";

type ParseBody = {
  input?: string;
  type?: ParseType;
};

export const hexParserApiRouter = Router();

hexParserApiRouter.get("/parsers", (_req, res) => {
  return res.json({ parsers: listParsers() });
});

hexParserApiRouter.post("/parser", (req, res) => {
  const body = req.body as ParseBody;
  const input = body.input;
  const type = body.type ?? "can";

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
