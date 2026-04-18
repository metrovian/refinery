import { renderEndpointPanel } from "../endpoint/ui";
import { renderHexPanel } from "../hex/ui";

const stylesheetUrls = ["/server/assets/styles.css", "/endpoint/assets/styles.css"];
const scriptUrls = ["/endpoint/assets/app.js", "/hex/assets/app.js"];

function renderStylesheets(): string {
  return stylesheetUrls.map((href) => `    <link rel="stylesheet" href="${href}" />`).join("\n");
}

function renderScripts(): string {
  return scriptUrls.map((src) => `    <script src="${src}" defer></script>`).join("\n");
}

export function renderUiPage(): string {
  return `<!doctype html>
<html lang="ko">
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
${renderStylesheets()}
  </head>
  <body>
    <main class="page-shell">
      <div class="page-stack">
        ${renderEndpointPanel()}
        ${renderHexPanel()}
      </div>
    </main>
${renderScripts()}
  </body>
</html>`;
}
