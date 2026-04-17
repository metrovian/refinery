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
        <div class="input-box">
          <div class="control-row">
            <select id="parser-type">
              <option value="can">CAN</option>
            </select>
          </div>
          <textarea id="hex-input" spellcheck="false" placeholder="0A FF 1C"></textarea>
        </div>
        <pre id="result-output" class="parsed-box">-</pre>
        <p id="error" class="error"></p>
      </section>
    </main>
    <script src="/hex/assets/app.js" defer></script>
  </body>
</html>`;
}
