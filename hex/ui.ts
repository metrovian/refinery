export function renderHexUiPage(): string {
  return `<!doctype html>
<html lang="ko">
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <link rel="stylesheet" href="/hex/assets/styles.css" />
  </head>
  <body>
    <main class="layout">
      <section class="panel">
        <div class="input-box">
          <div class="control-row">
            <select id="parser-type">
              <option value="modbus-rtu">MODBUS-RTU</option>
              <!-- [PARSER:UI_OPTION] -->
            </select>
          </div>
          <pre id="result-output" class="parsed-output">-</pre>
        </div>
        <textarea id="hex-input" class="raw-input" spellcheck="false" placeholder="0A FF 1C"></textarea>
      </section>
    </main>
    <script src="/hex/assets/app.js" defer></script>
  </body>
</html>`;
}
