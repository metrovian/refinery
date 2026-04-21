import { RequestHandler, Router } from "express";
import { I2cRequest, transferOverI2c } from "./i2c";
import { SpiRequest, transferOverSpi } from "./spi";
import { transferOverUart, UartRequest } from "./uart";

export const endpointRouter = Router();

function createDriverHandler<RequestBody, Result>(
  driver: "uart" | "spi" | "i2c",
  transfer: (body: RequestBody) => Promise<Result>,
  fallbackMessage: string,
): RequestHandler {
  return async (req, res) => {
    try {
      const result = await transfer(req.body as RequestBody);
      return res.json(result);
    } catch (error) {
      const message = error instanceof Error ? error.message : fallbackMessage;
      return res.status(400).json({
        ok: false,
        driver,
        error: message,
      });
    }
  };
}

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

endpointRouter.post("/uart/send", createDriverHandler<UartRequest, Awaited<ReturnType<typeof transferOverUart>>>("uart", transferOverUart, "UART transfer failed."));
endpointRouter.post("/spi/send", createDriverHandler<SpiRequest, Awaited<ReturnType<typeof transferOverSpi>>>("spi", transferOverSpi, "SPI transfer failed."));
endpointRouter.post("/i2c/send", createDriverHandler<I2cRequest, Awaited<ReturnType<typeof transferOverI2c>>>("i2c", transferOverI2c, "I2C transfer failed."));
