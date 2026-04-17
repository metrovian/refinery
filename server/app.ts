import express from "express";
import path from "path";
import { endpointRouter } from "../endpoint/router";
import { endpointPageRouter } from "../endpoint/page-router";
import { hexParserApiRouter } from "../hex/api-router";
import { hexPageRouter } from "../hex/page-router";

export function createApp() {
  const app = express();
  app.use(express.json());

  app.use("/api/endpoint", endpointRouter);
  app.use("/endpoint/assets", express.static(path.join(process.cwd(), "endpoint", "assets")));
  app.use("/endpoint", endpointPageRouter);
  app.use("/api/hex", hexParserApiRouter);
  app.use("/hex/assets", express.static(path.join(process.cwd(), "hex", "assets")));
  app.use("/hex", hexPageRouter);

  app.get("/", (_req, res) => {
    res.redirect("/hex");
  });

  return app;
}
