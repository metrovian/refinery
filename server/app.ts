import express from "express";
import path from "path";
import { endpointRouter } from "../endpoint/router";
import { hexRouter } from "../hex/router";
import { renderUiPage } from "./ui";

export function createApp() {
  const app = express();
  app.use(express.json());

  app.use("/api/endpoint", endpointRouter);
  app.use("/api/hex", hexRouter);
  app.use("/endpoint/assets", express.static(path.join(process.cwd(), "endpoint", "assets")));
  app.use("/hex/assets", express.static(path.join(process.cwd(), "hex", "assets")));

  app.get("/", (_req, res) => {
    res.type("html").send(renderUiPage());
  });

  return app;
}
