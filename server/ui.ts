import { renderEndpointPanel } from "../endpoint/ui";
import { renderHexPanel } from "../hex/ui";

export function renderUiPage(): string {
  return `<!doctype html>
<html lang="ko">
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <link rel="stylesheet" href="/endpoint/assets/styles.css" />
    <link rel="stylesheet" href="/hex/assets/styles.css" />
    <style>
      :root {
        --bg: #0a0d12;
        --bg-soft: #121720;
        --panel: #11161f;
        --text: #e5e7eb;
        --line: #252b37;
        --muted: #9ba7bb;
      }

      * {
        box-sizing: border-box;
      }

      html,
      body {
        min-height: 100%;
      }

      body {
        margin: 0;
        font-family: "Segoe UI", "Noto Sans KR", sans-serif;
        color: var(--text);
        min-height: 100vh;
        background-color: var(--bg);
        background-image: radial-gradient(circle at top center, #1c2431 0%, var(--bg-soft) 45%, var(--bg) 100%);
        background-repeat: no-repeat;
        background-attachment: fixed;
      }

      .page-shell {
        max-width: 760px;
        margin: 0 auto;
        padding: 32px 16px 48px;
      }

      .page-stack {
        display: grid;
        gap: 18px;
      }

      .page-panel {
        background: var(--panel);
        border: 1px solid var(--line);
        border-radius: 14px;
        padding: 16px;
      }
    </style>
  </head>
  <body>
    <main class="page-shell">
      <div class="page-stack">
        ${renderEndpointPanel()}
        ${renderHexPanel()}
      </div>
    </main>
    <script src="/endpoint/assets/app.js" defer></script>
    <script src="/hex/assets/app.js" defer></script>
  </body>
</html>`;
}
