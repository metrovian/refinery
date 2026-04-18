import express from "express";
import path from "path";
import { endpointRouter } from "../endpoint/router";
import { hexRouter } from "../hex/router";
import { renderUiPage } from "./ui";

const apiRouters = [
  ["/api/endpoint", endpointRouter],
  ["/api/hex", hexRouter],
] as const;

const staticAssetDirs = [
  ["/server/assets", ["server", "assets"]],
  ["/endpoint/assets", ["endpoint", "assets"]],
  ["/hex/assets", ["hex", "assets"]],
] as const;

export function createApp() {
  const app = express();
  app.use(express.json());

  apiRouters.forEach(([route, router]) => {
    app.use(route, router);
  });

  staticAssetDirs.forEach(([route, segments]) => {
    app.use(route, express.static(path.join(process.cwd(), ...segments)));
  });

  app.get("/", (_req, res) => {
    res.type("html").send(renderUiPage());
  });

  return app;
}
