import { Router } from "express";
import { renderHexUiPage } from "./ui";

export const hexPageRouter = Router();

hexPageRouter.get("/", (_req, res) => {
  res.type("html").send(renderHexUiPage());
});
