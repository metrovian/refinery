import { Router } from "express";
import { renderEndpointUiPage } from "./ui";

export const endpointPageRouter = Router();

endpointPageRouter.get("/", (_req, res) => {
  res.type("html").send(renderEndpointUiPage());
});
