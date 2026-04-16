export function renderHexUiPage(): string {
  return `<!doctype html>
<html lang="ko">
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>Hex Parser</title>
    <style>
      :root{--bg:#f3f5f7;--panel:#fff;--text:#111827;--muted:#6b7280;--line:#d7dde5;--accent:#0f766e;--accent2:#115e59;--bad:#b91c1c;--goodBg:#dcfce7;--badBg:#fee2e2}
      *{box-sizing:border-box} body{margin:0;font-family:"Segoe UI","Noto Sans KR",sans-serif;color:var(--text);background:radial-gradient(circle at top right,#dbeafe,transparent 45%),var(--bg)}
      .layout{max-width:1000px;margin:0 auto;padding:24px 16px 40px;display:grid;gap:16px}
      .panel{background:var(--panel);border:1px solid var(--line);border-radius:14px;padding:16px}
      .hero{background:linear-gradient(120deg,#0f766e,#0ea5e9);color:#f8fafc;border:0}
      .row{display:flex;gap:8px;margin-top:12px;align-items:center;flex-wrap:wrap}
      .space{justify-content:space-between;margin-top:0}
      .chip,.btn{border:1px solid var(--line);background:#fff;color:var(--text);border-radius:999px;padding:8px 12px;font-weight:600;cursor:pointer}
      .btn{border-radius:8px;background:var(--accent);color:#fff;border-color:var(--accent)}
      .btn:hover{background:var(--accent2)} .btn-light{background:#fff;color:var(--text);border-color:var(--line)}
      textarea{width:100%;min-height:140px;margin-top:12px;border:1px solid var(--line);border-radius:10px;padding:12px;font-size:14px;font-family:ui-monospace,SFMono-Regular,Menlo,Consolas,monospace;resize:vertical}
      .grid{display:grid;grid-template-columns:1fr;gap:16px}.muted{color:var(--muted);margin-top:8px}
      .token-list{margin-top:12px;display:flex;flex-wrap:wrap;gap:8px;min-height:32px}.token{padding:4px 8px;border-radius:999px;font-family:ui-monospace,SFMono-Regular,Menlo,Consolas,monospace;font-size:12px}
      .ok{background:var(--goodBg)} .bad{background:var(--badBg);color:var(--bad)}
      .cards{margin-top:12px;display:grid;gap:8px}.card{border:1px solid var(--line);border-radius:10px;padding:10px}
      .label{font-size:12px;color:var(--muted)} .value{margin-top:4px;font-size:24px;font-weight:700}
      pre{margin:6px 0 0;white-space:pre-wrap;word-break:break-word}.error{margin-top:10px;color:var(--bad);min-height:20px}
      @media (min-width:860px){.grid{grid-template-columns:1fr 1fr}}
    </style>
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
            <button class="chip" data-sample="can">CAN</button>
            <button class="chip" data-sample="modbus">MODBUS</button>
            <button class="chip" data-sample="raw">RAW</button>
          </div>
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
          </div>
          <p id="error" class="error"></p>
        </section>
      </section>
    </main>
    <script>
      const samples={can:"18FEF100 01 0A FF 1C",modbus:"01 03 00 6B 00 03 76 87",raw:"A55A01020408FFEE00112233445566778899AABBCCDDEEFF"};
      const inputEl=document.getElementById("hex-input"), parseBtn=document.getElementById("parse-btn"), clearBtn=document.getElementById("clear-btn");
      const tokensEl=document.getElementById("tokens"), summaryEl=document.getElementById("summary"), lengthEl=document.getElementById("length");
      const bytesEl=document.getElementById("bytes"), hexEl=document.getElementById("hex"), errorEl=document.getElementById("error");
      const tokenize=(v)=>v.trim().split(/[\\s,;:-]+/).filter(Boolean);
      const valid=(t)=>{const n=t.replace(/^0x/i,""); return !!n && /^[0-9a-fA-F]+$/.test(n) && (n.length<=2 || n.length%2===0);};
      function renderValidation(){
        const tokens=tokenize(inputEl.value); tokensEl.innerHTML="";
        if(!tokens.length){summaryEl.textContent="입력 대기 중"; return;}
        let bad=0; tokens.forEach((t)=>{const ok=valid(t); if(!ok) bad++; const e=document.createElement("span"); e.className="token "+(ok?"ok":"bad"); e.textContent=t; tokensEl.appendChild(e);});
        summaryEl.textContent=bad===0?("정상 토큰 "+tokens.length+"개"):("오류 토큰 "+bad+"개 / 전체 "+tokens.length+"개");
      }
      function reset(){lengthEl.textContent="0"; bytesEl.textContent="[]"; hexEl.textContent="[]"; errorEl.textContent="";}
      async function parse(){
        errorEl.textContent="";
        try{
          const res=await fetch("/api/hex/parse",{method:"POST",headers:{"Content-Type":"application/json"},body:JSON.stringify({input:inputEl.value})});
          const data=await res.json(); if(!res.ok) throw new Error(data.error||"Parse failed");
          lengthEl.textContent=String(data.length??0); bytesEl.textContent=JSON.stringify(data.bytes??[],null,2); hexEl.textContent=JSON.stringify(data.hex??[],null,2);
        }catch(e){reset(); errorEl.textContent=String(e && e.message ? e.message : e);}
      }
      document.querySelectorAll("[data-sample]").forEach((btn)=>btn.addEventListener("click",()=>{inputEl.value=samples[btn.getAttribute("data-sample")]||""; renderValidation(); parse();}));
      parseBtn.addEventListener("click",parse); clearBtn.addEventListener("click",()=>{inputEl.value=""; tokensEl.innerHTML=""; summaryEl.textContent="입력 대기 중"; reset();});
      inputEl.addEventListener("input",renderValidation); renderValidation(); reset();
    </script>
  </body>
</html>`;
}
