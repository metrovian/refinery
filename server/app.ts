import express from "express";
import { endpointRouter } from "../endpoint/router";
import { hexParserApiRouter } from "../hex/api-router";
import { hexPageRouter } from "../hex/page-router";

export function createApp() {
  const app = express();
  app.use(express.json());

  app.use("/api/endpoint", endpointRouter);
  app.use("/api/hex", hexParserApiRouter);
  app.use("/hex", hexPageRouter);

  app.get("/", (_req, res) => {
    res.redirect("/hex");
  });

  return app;
}
