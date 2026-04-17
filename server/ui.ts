import { renderEndpointPanel } from "../endpoint/ui";
import { renderHexPanel } from "../hex/ui";

export function renderUiPage(): string {
  return `<!doctype html>
<html lang="ko">
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <link rel="stylesheet" href="/shared/assets/styles.css" />
    <link rel="stylesheet" href="/endpoint/assets/styles.css" />
    <link rel="stylesheet" href="/hex/assets/styles.css" />
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
