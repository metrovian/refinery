import { Router } from "express";
import { I2cRequest, transferOverI2c } from "./i2c";
import { SpiRequest, transferOverSpi } from "./spi";
import { transferOverUart, UartRequest } from "./uart";

export const endpointRouter = Router();

endpointRouter.get("/health", (_req, res) => {
  res.json({
    service: "endpoint",
    status: "ok",
    message: "Communication endpoint is ready.",
    drivers: {
      uart: {
        status: "ready",
        defaultDevice: "/dev/serial0",
      },
      spi: {
        status: "ready",
        defaultDevice: "/dev/spidev0.0",
      },
      i2c: {
        status: "ready",
        defaultDevice: "/dev/i2c-1",
      },
    },
  });
});

endpointRouter.post("/uart/send", async (req, res) => {
  try {
    const result = await transferOverUart(req.body as UartRequest);
    return res.json(result);
  } catch (error) {
    const message = error instanceof Error ? error.message : "UART transfer failed.";
    return res.status(400).json({
      ok: false,
      driver: "uart",
      error: message,
    });
  }
});

endpointRouter.post("/spi/send", async (req, res) => {
  try {
    const result = await transferOverSpi(req.body as SpiRequest);
    return res.json(result);
  } catch (error) {
    const message = error instanceof Error ? error.message : "SPI transfer failed.";
    return res.status(400).json({
      ok: false,
      driver: "spi",
      error: message,
    });
  }
});

endpointRouter.post("/i2c/send", async (req, res) => {
  try {
    const result = await transferOverI2c(req.body as I2cRequest);
    return res.json(result);
  } catch (error) {
    const message = error instanceof Error ? error.message : "I2C transfer failed.";
    return res.status(400).json({
      ok: false,
      driver: "i2c",
      error: message,
    });
  }
});
