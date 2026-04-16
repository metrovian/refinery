export function renderHexUiPage(): string {
  return `<!doctype html>
<html lang="ko">
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>Hex Parser</title>
    <link rel="stylesheet" href="/hex/assets/styles.css" />
  </head>
  <body>
    <main class="layout">
      <section class="panel hero">
        <h1>Hex Parser Console</h1>
        <p>CAN, MODBUS 프레임 입력을 빠르게 검증/파싱합니다.</p>
      </section>
      <section class="panel">
        <div class="row space">
          <h2>Input</h2>
          <div class="row">
            <button class="chip" data-sample="can">CAN Sample</button>
            <button class="chip" data-sample="modbus">MODBUS Sample</button>
            <button class="chip" data-sample="raw">Raw Sample</button>
          </div>
        </div>
        <div class="row">
          <label for="parser-type">Parser</label>
          <select id="parser-type">
            <option value="raw">Raw Bytes</option>
            <option value="can">CAN Frame</option>
            <option value="modbus">MODBUS RTU</option>
          </select>
        </div>
        <textarea id="hex-input" spellcheck="false" placeholder="0A FF 1C"></textarea>
        <div class="row">
          <button id="parse-btn" class="btn">Parse</button>
          <button id="clear-btn" class="btn btn-light">Clear</button>
        </div>
      </section>
      <section class="grid">
        <section class="panel">
          <h2>Validation</h2>
          <p id="summary" class="muted">입력 대기 중</p>
          <div id="tokens" class="token-list"></div>
        </section>
        <section class="panel">
          <h2>Result</h2>
          <div class="cards">
            <article class="card"><p class="label">Length</p><p id="length" class="value">0</p></article>
            <article class="card"><p class="label">Bytes</p><pre id="bytes">[]</pre></article>
            <article class="card"><p class="label">Hex</p><pre id="hex">[]</pre></article>
            <article class="card"><p class="label">Parsed</p><pre id="parsed">{}</pre></article>
          </div>
          <p id="error" class="error"></p>
        </section>
      </section>
    </main>
    <script src="/hex/assets/app.js" defer></script>
  </body>
</html>`;
}
