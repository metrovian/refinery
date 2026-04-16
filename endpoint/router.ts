import { Router } from "express";

export const endpointRouter = Router();

endpointRouter.get("/health", (_req, res) => {
  res.json({
    service: "endpoint",
    status: "ok",
    message: "Communication endpoint scaffold is ready.",
  });
});
