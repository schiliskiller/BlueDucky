// este es el frontend basado de, sin embargo, las animaciones me ayudó la ia generativa
// https://github.com/ekv88/esp32-bluetooth-rubber-ducky/tree/main

#pragma once
#include <Arduino.h>

// me apoyé también desde https://html.onlineviewer.net/

const char PAGE_INDEX[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="es">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover" />
  <title>BlueDucky</title>
  <style>
    :root {
      --bg: #050b07;
      --card-bg: #08120b;
      --ink: #5af78e;
      --muted: #2d8a4e;
      --dim: #13381e;
      --accent: #00ff66;
      --warn: #ffd166;
      --err: #ff5555;
    }
    * { box-sizing: border-box; margin: 0; padding: 0; }
    html, body { height: 100%; font-family: "Courier New", monospace; background: var(--bg); color: var(--ink); font-size: 13px; }
    
    body::before {
      content: "";
      position: fixed;
      inset: 0;
      pointer-events: none;
      background: linear-gradient(rgba(18, 16, 16, 0) 50%, rgba(0, 0, 0, 0.25) 50%);
      background-size: 100% 3px;
      z-index: 100;
    }

    header {
      padding: 8px 12px;
      border-bottom: 1px solid var(--dim);
      display: flex;
      justify-content: space-between;
      align-items: center;
      background: #020503;
    }
    .brand { font-weight: bold; color: var(--accent); }

    .wrap {
      display: grid;
      grid-template-columns: 280px 1fr 340px;
      gap: 8px;
      padding: 8px;
      height: calc(100vh - 65px);
    }
    @media (max-width: 900px) {
      .wrap { grid-template-columns: 1fr; height: auto; }
    }

    .frame {
      background: var(--card-bg);
      border: 1px solid var(--dim);
      display: flex;
      flex-direction: column;
      overflow: hidden;
      padding: 8px;
    }
    .title {
      color: var(--muted);
      margin-bottom: 8px;
      text-transform: uppercase;
    }
    .title::before { content: "+----- [ "; }
    .title::after  { content: " ] -----+"; }

    /* listado de dispositivos que tengan bluetooth encendido */
    .net-list {
      flex: 1;
      overflow-y: auto;
      border: 1px dashed var(--dim);
      padding: 4px;
    }
    .net-item {
      display: flex;
      justify-content: space-between;
      padding: 4px;
      border-bottom: 1px solid var(--dim);
      cursor: pointer;
    }
    .net-item:hover { background: #0c2013; }
    .net-item .ssid { color: #fff; }
    .net-item .freq { color: var(--accent); }

    /* área central de steps */
    .board {
      flex: 1;
      overflow-y: auto;
      border: 1px dashed var(--dim);
      padding: 6px;
      background: #000;
    }
    .step-block {
      background: #07150c;
      border: 1px solid var(--dim);
      padding: 6px 8px;
      margin-bottom: 6px;
      display: grid;
      grid-template-columns: auto 1fr auto;
      gap: 8px;
      align-items: center;
    }
    .step-block select, .step-block input {
      background: #000;
      color: var(--ink);
      border: 1px solid var(--muted);
      padding: 4px;
      font-family: inherit;
    }
    .kill {
      background: none;
      border: none;
      color: var(--err);
      cursor: pointer;
      font-weight: bold;
    }

    /* Editor de texto / Script */
    textarea {
      flex: 1;
      background: #000;
      color: var(--ink);
      border: 1px solid var(--dim);
      padding: 6px;
      font-family: inherit;
      resize: none;
      outline: none;
    }
    textarea:focus { border-color: var(--accent); }

    .bar {
      display: flex;
      gap: 6px;
      margin-top: 8px;
      flex-wrap: wrap;
    }
    .btn {
      background: #081a0e;
      color: var(--ink);
      border: 1px solid var(--muted);
      padding: 6px 10px;
      cursor: pointer;
      font-family: inherit;
      font-size: 11px;
    }
    .btn:hover { background: var(--muted); color: #000; }
    .btn.primary { border-color: var(--accent); color: var(--accent); }
    .btn.primary:hover { background: var(--accent); color: #000; }
    .btn.warn { border-color: var(--warn); color: var(--warn); }
    .btn.err { border-color: var(--err); color: var(--err); }

    footer {
      border-top: 1px solid var(--dim);
      padding: 4px 12px;
      font-size: 11px;
      display: flex;
      justify-content: space-between;
      color: var(--muted);
      background: #020503;
    }
  </style>
</head>


<body>

  <header>
    <div class="brand"> BLUEDUCKY by RubberDAB | Pentesting | Facultad de Estadística e Informática </div>
    <div id="sd-status">SD: MONTADA [OK]</div>
  </header>

  <main class="wrap">
    <!-- PANEL IZQUIERDO: DISPOSITIVOS CON BLUETOOTH ENCENDIDO -->
    <section class="frame">
      <div class="title">DISPOSITIVOS CON BLUETOOTH</div>
      
      <div class="net-list" id="netList">
        <div class="net-item"><span class="ssid">Totalplay-2.4G</span><span class="freq">[x5]</span></div>
        <div class="net-item"><span class="ssid">INFINITUM_90A1</span><span class="freq">[x3]</span></div>
        <div class="net-item"><span class="ssid">Megacable_Wifi</span><span class="freq">[x2]</span></div>
      </div>
      
      <div class="bar">
        <button class="btn primary" onclick="escanearRedes()">[RE-SCAN]</button>
        <button class="btn" onclick="descargarLog()">LOG SD</button>
      </div>
    </section>

    <!-- PANEL CENTRAL: SECUENCIA DE PASOS DUCKY -->
    <section class="frame">
      <div class="title">INJECTION_STEPS</div>
      <div class="board" id="stepsLane"></div>
      <div class="bar">
        <button class="btn" onclick="addStep('WAIT')">+ WAIT</button>
        <button class="btn" onclick="addStep('STRING')">+ STRING</button>
        <button class="btn" onclick="addStep('KEY')">+ KEY</button>
        <button class="btn primary" onclick="guardarEnESP32()">GUARDAR -> ESP32</button>
        <button class="btn warn" onclick="ejecutarScript()">LANZAR RUN</button>
      </div>
    </section>

    <!-- PANEL DERECHO: PAYLOAD DUCKY SCRIPT / JSON -->
    <aside class="frame">
      <div class="title">PAYLOAD_RAW</div>
      <textarea id="payloadText" spellcheck="false">REM BlueDucky Autonomous Payload
DELAY 500
GUI r
DELAY 400
STRING cmd
ENTER
DELAY 500
STRING echo BlueDucky Conectado...
ENTER</textarea>
      <div class="bar">
        <button class="btn" onclick="convertirTextoAPasos()">PARSER -> PASOS</button>
        <button class="btn err" onclick="limpiar()">LIMPIAR</button>
      </div>
    </aside>
  </main>

  <footer>
    <span>MODO: AP ACTIVO (192.168.4.1)</span>
    <span id="syslog">SISTEMA LISTO</span>
  </footer>

  <script>
    const stepsLane = document.getElementById('stepsLane');
    const syslog = document.getElementById('syslog');
    const payloadText = document.getElementById('payloadText');

    function log(msg) { syslog.textContent = ">> " + msg; }

    function addStep(tipo, valor = '') {
      const row = document.createElement('div');
      row.className = 'step-block';

      let inputHtml = '';
      if(tipo === 'WAIT') {
        inputHtml = `<input type="number" value="${valor || 500}" style="width:100px" placeholder="ms"> <span>ms</span>`;
      } else if(tipo === 'STRING') {
        inputHtml = `<input type="text" value="${valor || ''}" placeholder="Texto a escribir...">`;
      } else if(tipo === 'KEY') {
        const keys = ['ENTER','GUI r','TAB','ESCAPE','SPACE','CTRL+c','CTRL+v'];
        let opts = keys.map(k => `<option value="${k}" ${valor === k ? 'selected':''}>${k}</option>`).join('');
        inputHtml = `<select>${opts}</select>`;
      }

      row.innerHTML = `<span>[${tipo}]</span> ${inputHtml} <button class="kill" onclick="this.parentElement.remove()">X</button>`;
      stepsLane.appendChild(row);
      log("Paso " + tipo + " añadido");
    }

    async function escanearRedes() {
      log("Iniciando escaneo Wi-Fi...");
      try {
        const res = await fetch('/api/scan');
        const redes = await res.json();
        const container = document.getElementById('netList');
        container.innerHTML = '';
        redes.forEach(r => {
          container.innerHTML += `<div class="net-item"><span class="ssid">${r.nombre}</span><span class="freq">[x${r.frec}]</span></div>`;
        });
        log("Escaneo finalizado.");
      } catch {
        log("Error al pedir escaneo");
      }
    }

    async function guardarEnESP32() {
      log("Guardando en la SD del ESP32...");
      try {
        await fetch('/api/save', {
          method: 'POST',
          headers: {'Content-Type': 'text/plain'},
          body: payloadText.value
        });
        log("¡Script guardado en la SD exitosamente!");
      } catch {
        log("Fallo de conexión al guardar.");
      }
    }

    async function ejecutarScript() {
      log("Enviando orden de ejecución...");
      try {
        await fetch('/api/run', { method: 'POST' });
        log("Payload en ejecución vía Bluetooth.");
      } catch {
        log("Error al disparar payload.");
      }
    }

    function convertirTextoAPasos() {
      stepsLane.innerHTML = '';
      const lineas = payloadText.value.split('\n');
      lineas.forEach(l => {
        l = l.trim();
        if(l.startsWith('DELAY')) addStep('WAIT', l.replace('DELAY', '').trim());
        else if(l.startsWith('STRING')) addStep('STRING', l.replace('STRING', '').trim());
        else if(l === 'ENTER' || l === 'GUI r') addStep('KEY', l);
      });
      log("Convertido a bloques visuales.");
    }

    // Inicializar bloques por defecto
    convertirTextoAPasos();
  </script>
</body>
</html>
)rawliteral";