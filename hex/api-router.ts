import { Router } from "express";
import { parseHex } from "./parser";

type ParseBody = {
  input?: string;
};

export const hexParserApiRouter = Router();

hexParserApiRouter.post("/parse", (req, res) => {
  const body = req.body as ParseBody;
  const input = body.input;

  if (typeof input !== "string") {
    return res.status(400).json({ error: "`input` must be a string." });
  }

  try {
    return res.json(parseHex(input));
  } catch (error) {
    const message = error instanceof Error ? error.message : "Unknown parse error";
    return res.status(400).json({ error: message });
  }
});
