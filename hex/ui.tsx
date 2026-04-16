export function renderHexUiPage(): string {
  return `<!doctype html>
<html lang="ko">
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>Hex</title>
    <link rel="stylesheet" href="/hex/assets/styles.css" />
  </head>
  <body>
    <main class="layout">
      <header class="hero">
        <h1>Hex Parser</h1>
      </header>
      <section class="panel">
        <div class="control-row">
          <select id="parser-type">
            <option value="raw">raw bytes</option>
            <option value="can">can frame</option>
            <option value="modbus">modbus rtu</option>
          </select>
        </div>
        <textarea id="hex-input" spellcheck="false" placeholder="0A FF 1C"></textarea>
      </section>
      <section class="panel">
        <div class="result-grid">
          <div class="result-item">
            <p class="result-label">type</p>
            <p id="result-type" class="result-value">-</p>
          </div>
          <div class="result-item">
            <p class="result-label">length</p>
            <p id="result-length" class="result-value">0</p>
          </div>
          <div class="result-item result-item-wide">
            <p class="result-label">bytes</p>
            <pre id="result-bytes" class="result">[]</pre>
          </div>
          <div class="result-item result-item-wide">
            <p class="result-label">hex</p>
            <pre id="result-hex" class="result">[]</pre>
          </div>
          <div class="result-item result-item-wide">
            <p class="result-label">parsed</p>
            <pre id="result-parsed" class="result">{}</pre>
          </div>
        </div>
        <p id="error" class="error"></p>
      </section>
    </main>
    <script src="/hex/assets/app.js" defer></script>
  </body>
</html>`;
}
