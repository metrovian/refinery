import { createApp } from "./app";

const host = process.env.HOST ?? "0.0.0.0";
const port = Number(process.env.PORT ?? 3000);

const app = createApp();
app.listen(port, host, () => {
  console.log(`Server running on http://${host}:${port}`);
});
