// Carrinho 2WD — ESP32 + Ponte H L298N, controle pelo celular (Wi-Fi do próprio ESP32)
//
// Como funciona o controle (e por que não trava mais):
//  - Enquanto você SEGURA um botão, o celular reenvia o comando a cada ~100 ms.
//  - Se o ESP32 ficar 700 ms sem receber comando (dedo solto, Wi-Fi caiu, página
//    fechou), ele PARA os motores sozinho. O carro nunca mais fica "preso andando".
//  - O celular só manda um pedido por vez, em ordem. Não existe mais o caso do
//    "PARAR" chegar antes do "FRENTE" e o carro sair andando pra sempre.
//  - A página mostra se está conectada e há quanto tempo o ESP32 está ligado.
//    Se esse tempo voltar pra zero, o ESP32 reiniciou (= problema de alimentação).
//
// Ligações:  IN1 -> GPIO 32 | IN2 -> GPIO 33 | IN3 -> GPIO 26 | IN4 -> GPIO 27
//            GND da ponte H ligado num GND do ESP32 | ENA e ENB com o jumper

#include <WiFi.h>
#include <WebServer.h>
#include <Ticker.h>
#include "esp_system.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

const char* SSID  = "Carrinho_MakerLab";
const char* SENHA = "12345678";   // mínimo 8 caracteres
const int   CANAL = 6;            // se o laboratório tiver muita rede, teste 1 ou 11

const int IN1 = 32;  // motor esquerdo
const int IN2 = 33;  // motor esquerdo
const int IN3 = 26;  // motor direito
const int IN4 = 27;  // motor direito

// Se o botão "Só Esq" ou "Só Dir" girar a roda PRA TRÁS, troque o false por true
// daquela roda (ou inverta os dois fios do motor nos bornes OUT da ponte H).
const bool INVERTE_ESQ = false;
const bool INVERTE_DIR = false;

// Remendo pra alimentação fraca: impede o ESP32 de reiniciar quando a tensão cai.
// O certo é alimentar o ESP32 separado dos motores (ver explicação). Com a
// alimentação resolvida, pode deixar false.
const bool DESLIGA_BROWNOUT = true;

const uint32_t FAILSAFE_MS = 700;  // para os motores se ficar esse tempo sem comando

WebServer server(80);
Ticker vigia;

volatile uint32_t ultimoComando = 0;
volatile bool andando = false;
const char* motivoReset = "?";

const char PAGINA_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
<title>Carrinho 2WD</title>
<style>
  * { box-sizing: border-box; -webkit-tap-highlight-color: transparent; }
  body { background: #121212; color: #fff; font-family: sans-serif; text-align: center; margin: 0; padding: 12px;
         user-select: none; -webkit-user-select: none; -webkit-touch-callout: none; overscroll-behavior: none; }
  h2 { color: #ed145b; margin: 4px 0; }
  #st { margin: 8px auto; padding: 8px; border-radius: 10px; font-size: 14px; max-width: 340px; background: #333; }
  #st.ok { background: #1b5e20; } #st.erro { background: #b71c1c; }
  #rst { font-size: 12px; color: #aaa; min-height: 16px; }
  .grid { display: grid; grid-template-columns: repeat(3, 95px); gap: 12px; justify-content: center; margin-top: 14px; }
  button { background: #222; color: #fff; border: 2px solid #ed145b; border-radius: 16px; height: 85px;
           font-size: 30px; font-weight: bold; touch-action: none; }
  button.on { background: #ed145b; }
  .stop { background: #b71c1c; border-color: #ff5252; font-size: 16px; }
  .diag { margin-top: 22px; display: flex; gap: 10px; justify-content: center; }
  .diag button { height: 52px; font-size: 14px; padding: 0 14px; border-color: #00e676; }
</style>
</head>
<body>
<h2>Carrinho 2WD — FIAP</h2>
<div id="st">conectando...</div>
<div id="rst"></div>
<div class="grid">
  <div></div><button data-c="F">▲</button><div></div>
  <button data-c="L">◄</button><button class="stop" id="parar">PARAR</button><button data-c="R">►</button>
  <div></div><button data-c="B">▼</button><div></div>
</div>
<div class="diag">
  <button data-c="E">Só Motor Esq</button>
  <button data-c="D">Só Motor Dir</button>
</div>
<script>
  const st = document.getElementById('st'), rst = document.getElementById('rst');
  const dedos = new Map();          // dedo (pointerId) -> comando que ele está segurando
  let atual = 'S', enviado = '', ocupado = false, ultimo = 0, falhas = 0;
  let upAnterior = -1, reinicios = 0;

  function recalcular() {
    let c = 'S';
    for (const v of dedos.values()) c = v;     // vale o último botão apertado
    atual = c;
    document.querySelectorAll('[data-c]').forEach(b => b.classList.toggle('on', b.dataset.c === c));
    enviar();
  }
  function soltarTudo() { dedos.clear(); recalcular(); }

  document.querySelectorAll('[data-c]').forEach(b => {
    b.addEventListener('pointerdown', e => {
      e.preventDefault();
      try { b.setPointerCapture(e.pointerId); } catch (_) {}
      dedos.set(e.pointerId, b.dataset.c);
      recalcular();
    });
    const solta = e => { if (dedos.delete(e.pointerId)) recalcular(); };
    b.addEventListener('pointerup', solta);
    b.addEventListener('pointercancel', solta);
    b.addEventListener('lostpointercapture', solta);
  });
  document.getElementById('parar').addEventListener('pointerdown', e => { e.preventDefault(); soltarTudo(); });
  document.addEventListener('contextmenu', e => e.preventDefault());
  window.addEventListener('blur', soltarTudo);
  document.addEventListener('visibilitychange', () => { if (document.hidden) soltarTudo(); });

  // Um pedido por vez. Segurando: reenvia a cada 100 ms. Parado: avisa a cada 1 s.
  function enviar() {
    if (ocupado) return;
    const agora = performance.now(), c = atual;
    const espera = falhas >= 2 ? 300 : (c === 'S' ? 1000 : 100);
    if (c === enviado && agora - ultimo < espera) return;
    ocupado = true; ultimo = agora;
    const ctrl = new AbortController();
    const t = setTimeout(() => ctrl.abort(), 450);   // pedido perdido: desiste e reenvia antes do failsafe
    fetch('/cmd?c=' + c, { signal: ctrl.signal, cache: 'no-store' })
      .then(r => r.text())
      .then(txt => { enviado = c; falhas = 0; online(txt); })
      .catch(() => { enviado = c; falhas++; if (falhas >= 2) offline(); })
      .finally(() => { clearTimeout(t); ocupado = false; });
  }
  setInterval(enviar, 30);

  function online(txt) {
    const [up, motivo] = txt.split(' ');
    const s = +up;
    if (upAnterior >= 0 && s + 2 < upAnterior) reinicios++;
    upAnterior = s;
    st.className = 'ok';
    st.textContent = 'Conectado · ESP32 ligado há ' + s + ' s';
    rst.textContent = 'reinícios vistos: ' + reinicios + ' · último reset: ' + motivo;
  }
  function offline() {
    st.className = 'erro';
    st.textContent = 'SEM CONEXÃO com o carrinho (o carro para sozinho)';
  }
</script>
</body>
</html>
)rawliteral";

// dir: +1 frente, -1 ré, 0 parado
void motor(int pinFrente, int pinRe, int dir, bool inverte) {
  if (inverte) dir = -dir;
  digitalWrite(pinFrente, dir > 0 ? HIGH : LOW);
  digitalWrite(pinRe,     dir < 0 ? HIGH : LOW);
}

void mover(int esq, int dir) {
  motor(IN1, IN2, esq, INVERTE_ESQ);  // esquerdo: frente = IN1 HIGH
  motor(IN4, IN3, dir, INVERTE_DIR);  // direito:  frente = IN4 HIGH
  andando = (esq != 0 || dir != 0);
}

void parar() { mover(0, 0); }

void aplicar(char c) {
  switch (c) {
    case 'F': mover( 1,  1); break;
    case 'B': mover(-1, -1); break;
    case 'L': mover(-1,  1); break;
    case 'R': mover( 1, -1); break;
    case 'E': mover( 1,  0); break;
    case 'D': mover( 0,  1); break;
    default:  parar();       break;
  }
}

// Roda no timer do sistema, independente do loop(): mesmo se o servidor web
// engasgar, o carro para quando os comandos param de chegar.
void checarFailsafe() {
  if (andando && millis() - ultimoComando > FAILSAFE_MS) parar();
}

const char* nomeReset(esp_reset_reason_t r) {
  switch (r) {
    case ESP_RST_POWERON:  return "ligou_normal";
    case ESP_RST_BROWNOUT: return "QUEDA_DE_TENSAO";
    case ESP_RST_PANIC:    return "TRAVOU(panic)";
    case ESP_RST_INT_WDT:
    case ESP_RST_TASK_WDT:
    case ESP_RST_WDT:      return "TRAVOU(watchdog)";
    case ESP_RST_SW:       return "reset_software";
    case ESP_RST_EXT:      return "botao_EN";
    default:               return "outro";
  }
}

void handleCmd() {
  String c = server.arg("c");
  aplicar(c.length() ? c[0] : 'S');
  ultimoComando = millis();

  char resp[48];
  snprintf(resp, sizeof(resp), "%lu %s", (unsigned long)(millis() / 1000), motivoReset);
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "text/plain", resp);
}

void setup() {
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  parar();

  if (DESLIGA_BROWNOUT) WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  Serial.begin(115200);
  motivoReset = nomeReset(esp_reset_reason());
  Serial.printf("\nCarrinho iniciando. Motivo do ultimo reset: %s\n", motivoReset);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID, SENHA, CANAL);
  WiFi.setTxPower(WIFI_POWER_15dBm);  // alcance de sobra pra uma sala, pico de corrente menor
  Serial.print("Abra no celular: http://");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() { server.send(200, "text/html", PAGINA_HTML); });
  server.on("/cmd", handleCmd);
  server.onNotFound([]() {
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
  server.begin();

  vigia.attach_ms(50, checarFailsafe);
}

void loop() {
  server.handleClient();
  delay(1);
}
