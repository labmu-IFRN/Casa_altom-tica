#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>

const char* ssid = "Sala_de_Robotica_n10";
const char* password = "12345678";

WebServer server(80);

int mensagem = 0;
int posicao_varal = 0;
static int last_varal = -1;



const char html_home[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html lang='pt-br'>
<head>
<meta charset='UTF-8'>
<meta name='viewport' content='width=device-width, initial-scale=1.0'>
<title>Controle ESP32</title>
<style>
  body {
    font-family: Arial, sans-serif;
    background: linear-gradient(135deg, #072655, #400a61);
    color: white;
    text-align: center;
    margin: 0;
    padding: 20px;
  }
  h1 { font-size: 1.6em; margin-top: 20px; }
  form { margin: 20px auto; }
  button {
    width: 90%;
    max-width: 280px;
    background: #6a00b8;
    color: white;
    border: none;
    padding: 14px;
    border-radius: 12px;
    font-size: 1.1em;
    font-weight: bold;
    margin: 8px 0;
    cursor: pointer;
    transition: all 0.3s;
  }
  button:hover { background: #400a61; transform: scale(1.05); }
</style>
</head>
<body>
  <h1>Controle da Casa Automática</h1>
  <form action='/luzes' method='GET'><button type='submit'>💡 Luzes</button></form>
  <form action='/sensores' method='GET'><button type='submit'>📡 Sensores</button></form>
  <form action='/perifericos' method='GET'><button type='submit'>⚙️ Periféricos</button></form>
</body></html>
)rawliteral";



const char html_controle_luzes[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html lang='pt-br'>
<head>
<meta charset='UTF-8'>
<meta name='viewport' content='width=device-width, initial-scale=1.0'>
<title>Luzes</title>
<style>
  body {
    font-family: Arial, sans-serif;
    background: linear-gradient(135deg, #072655, #400a61);
    color: white;
    text-align: center;
    margin: 0;
    padding: 20px;
  }
  h1 { font-size: 1.5em; margin-bottom: 10px; }
  h2 { font-size: 1.2em; margin-top: 20px; }
  button {
    width: 40%;
    min-width: 120px;
    background: #6a00b8;
    color: white;
    border: none;
    padding: 12px;
    border-radius: 10px;
    font-size: 1em;
    margin: 5px;
    cursor: pointer;
  }
  button:hover { background: #400a61; }
  a {
    display: inline-block;
    margin-top: 20px;
    color: #a0dfff;
    text-decoration: none;
    font-weight: bold;
  }
  a:hover { text-decoration: underline; }
</style>
</head>
<body>
<h1>Controle de Luzes</h1>
<h2>Térreo</h2>
<form action='/luzes' method='GET'>
  <button name='btnl' value='andar_1_on'>Ligar</button>
  <button name='btnl' value='andar_1_off'>Desligar</button>
</form>

<h2>Primeiro Andar</h2>
<form action='/luzes' method='GET'>
  <button name='btnl' value='andar_2_on'>Ligar</button>
  <button name='btnl' value='andar_2_off'>Desligar</button>
</form>

<h2>Frente</h2>
<form action='/luzes' method='GET'>
  <button name='btnl' value='frente_on'>Ligar</button>
  <button name='btnl' value='frente_off'>Desligar</button>
</form>

<h2>Garagem</h2>
<form action='/luzes' method='GET'>
  <button name='btnl' value='garagem_on'>Ligar</button>
  <button name='btnl' value='garagem_off'>Desligar</button>
</form>

<a href='/'>⬅ Voltar</a>
</body></html>
)rawliteral";



const char html_controle_sensores[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html lang='pt-br'>
<head>
<meta charset='UTF-8'>
<meta name='viewport' content='width=device-width, initial-scale=1.0'>
<title>Sensores</title>
<style>
  body {
    font-family: Arial, sans-serif;
    background: linear-gradient(135deg, #072655, #400a61);
    color: white;
    text-align: center;
    margin: 0;
    padding: 20px;
  }
  h1 { font-size: 1.5em; margin-bottom: 10px; }
  h2 { font-size: 1.2em; margin-top: 20px; }
  button {
    width: 40%;
    min-width: 120px;
    background: #6a00b8;
    color: white;
    border: none;
    padding: 12px;
    border-radius: 10px;
    font-size: 1em;
    margin: 5px;
    cursor: pointer;
  }
  button:hover { background: #400a61; }
  a {
    display: inline-block;
    margin-top: 20px;
    color: #a0dfff;
    text-decoration: none;
    font-weight: bold;
  }
</style>
</head>
<body>
<h1>Controle de Sensores</h1>
<h2>Sensor de Chuva</h2>
<form action='/sensores' method='GET'>
  <button name='btns' value='chuva_on'>Ativar</button>
  <button name='btns' value='chuva_off'>Desativar</button>
</form>
<a href='/'>⬅ Voltar</a>
</body></html>
)rawliteral";



const char html_controle_perifericos[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html lang='pt-br'>
<head>
<meta charset='UTF-8'>
<meta name='viewport' content='width=device-width, initial-scale=1.0'>
<title>Periféricos</title>
<style>
  body {
    font-family: Arial, sans-serif;
    background: linear-gradient(135deg, #072655, #400a61);
    color: white;
    text-align: center;
    margin: 0;
    padding: 20px;
  }
  h1 { font-size: 1.5em; margin-bottom: 10px; }
  h2 { font-size: 1.2em; margin-top: 20px; }
  button {
    width: 40%;
    min-width: 120px;
    background: #6a00b8;
    color: white;
    border: none;
    padding: 12px;
    border-radius: 10px;
    font-size: 1em;
    margin: 5px;
    cursor: pointer;
  }
  button:hover { background: #400a61; }
  a {
    display: inline-block;
    margin-top: 20px;
    color: #a0dfff;
    text-decoration: none;
    font-weight: bold;
  }
</style>
</head>
<body>
<h1>Controle de Periféricos</h1>

<h2>Porta</h2>
<form action='/perifericos' method='GET'>
  <button name='btnp' value='porta_aberta'>Abrir</button>
  <button name='btnp' value='porta_fechada'>Fechar</button>
</form>

<h2>Varal</h2>
<form action='/perifericos' method='GET'>
  <button name='btnp' value='varal_aberto'>Abrir</button>
  <button name='btnp' value='varal_fechado'>Fechar</button>
</form>

<a href='/'>⬅ Voltar</a>
</body></html>
)rawliteral";



void home() {
  server.send(200, "text/html", html_home);
}

void controle_luzes() {
  if (server.hasArg("btnl")) {
    String botao = server.arg("btnl");

    if (botao == "andar_1_on") mensagem = 181;
    else if (botao == "andar_1_off") mensagem = 182;
    else if (botao == "andar_2_on") mensagem = 183;
    else if (botao == "andar_2_off") mensagem = 184;
    else if (botao == "frente_on") mensagem = 185;
    else if (botao == "frente_off") mensagem = 186;
    else if (botao == "garagem_on") mensagem = 187;
    else if (botao == "garagem_off") mensagem = 188;
  }
  server.send(200, "text/html", html_controle_luzes);
}

void controle_sensores() {
  if (server.hasArg("btns")) {
    String botao = server.arg("btns");

    if (botao == "temperatura_on") mensagem = 189;
    else if (botao == "temperatura_off") mensagem = 190;
    else if (botao == "chuva_on") mensagem = 191;
    else if (botao == "chuva_off") mensagem = 192;
  }
  server.send(200, "text/html", html_controle_sensores);
}


void controle_perifericos() {
  if (server.hasArg("btnp")) {
    String botao = server.arg("btnp");

    if (botao == "porta_aberta") mensagem = 195;
    else if (botao == "porta_fechada") mensagem = 196;
    else if (botao == "varal_aberto") mensagem = 193;
    else if (botao == "varal_fechado") mensagem = 194;
  }

  /*if (server.hasArg("value")) {
    posicao_varal = server.arg("value").toInt();
  }*/

  server.send(200, "text/html", html_controle_perifericos);
}

void setup() {
  Wire.begin();
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", home);
  server.on("/luzes", controle_luzes);
  server.on("/sensores", controle_sensores);
  server.on("/perifericos", controle_perifericos);

  server.begin();
  Serial.println("Servidor Web iniciado!");
}

void loop() {
  server.handleClient();

  if (mensagem != 0) {
      Wire.beginTransmission(8);
      Wire.write(mensagem);
      Wire.endTransmission();
      mensagem = 0;
  }
  /*
  if (posicao_varal != last_varal) {
      Wire.beginTransmission(8);
      Wire.write(posicao_varal);
      Wire.endTransmission();
      last_varal = posicao_varal;
  }*/
}
