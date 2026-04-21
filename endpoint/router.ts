import { Router } from "express";
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
        board: "raspberry-pi-zero-2w",
        defaultDevice: "/dev/serial0",
        pins: {
          tx: "GPIO14 / pin 8",
          rx: "GPIO15 / pin 10",
        },
      },
      spi: {
        status: "ready",
        board: "raspberry-pi-zero-2w",
        defaultDevice: "/dev/spidev0.0",
      },
      i2c: {
        status: "scaffold",
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
