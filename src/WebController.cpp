#include "WebController.h"
#include "config.h"

WebController::WebController() : server(HTTP_PORT), ws(TCP_PORT) {}

void WebController::setup(){ 
    WiFi.softAP(SSID, PASSWORD);
    Serial.println("AP running");
    Serial.printf("My IP Address: %s", WiFi.softAPIP().toString());
    server.on("/", HTTP_GET, [&](AsyncWebServerRequest *request){
        onIndexRequest(request);
    });
    server.begin();
    ws.begin();
    ws.onEvent([&]
        (uint8_t clientNum, WStype_t type, unsigned char *payload, size_t length) {
            onWebSocketEvent(clientNum, type, payload, length);
        }
    );

    xTaskCreatePinnedToCore(loopTask, "WebController loop", 8192, this, 1, NULL, 1);
}

void WebController::loop() {
    ws.loop();
}

void WebController::loopTask(void *params) {
    WebController *self = static_cast<WebController*>(params);
    while (true) {
        self->loop();
        vTaskDelay(1);
    }
}

void WebController::broadcastFPS(float fps) {
    sprintf(msgBuf, "FPS : %.2f", fps);
    ws.broadcastTXT(msgBuf);
}

void WebController::broadcastNewhi(int newhi, int oldhi) {
    sprintf(msgBuf, "newhi %d %d", newhi, oldhi);
    ws.broadcastTXT(msgBuf);
}

void WebController::broadcastGameover() {
    sprintf(msgBuf, "gameover");
    ws.broadcastTXT(msgBuf);
}

void WebController::onWebSocketEvent(uint8_t clientNum, WStype_t type, unsigned char *payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", clientNum);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = ws.remoteIP(clientNum);
            Serial.printf("[%u] Connection from %s\n", clientNum, ip.toString());
            break;
        }

        case WStype_TEXT:
            Serial.printf("[%u] Received text: %s\n", clientNum, payload);
            if (strcmp((char *)payload, "trigger") == 0) {
                Serial.println("Remote trigger called");
                remoteTrigger = true;
            } else {
                Serial.printf("[%u] Connection not recognised", clientNum);
            }
            break;
        default:
            break;
    }
}

void WebController::onIndexRequest(AsyncWebServerRequest* request) {
    IPAddress remoteIP = request->client()->remoteIP();
    Serial.printf("[%s] HTTP GET request of %s", remoteIP.toString(), request->url());
    request->send(200, "text/html", generateHTML());
}

String WebController::generateHTML() {
    String html = R"rawliteral(
    <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>flappybird32 remote control</title>
            <script language="javascript" type="text/javascript">
                var url="ws://)rawliteral" + WiFi.softAPIP().toString() + ":" + String(TCP_PORT) + R"rawliteral("
                var high
                var fps
                var button
                var ws
                var topText
                var newhi

                function init() {
                    high = document.getElementById("high")
                    fps = document.getElementById("fps")
                    button = document.getElementById("trigger")
                    topText = document.getElementById("topText")
                    newhi = document.getElementById("newhi")
                    wsConnect(url)
                }

                function wsConnect(url) {
                    ws = new WebSocket(url)
                    ws.onopen = function(evt) { onOpen(evt) }
                    ws.onclose = function(evt) { onClose(evt) }
                    ws.onmessage = function(evt) { onMessage(evt) }
                    ws.onerror = function(evt) { onError(evt) }
                }

                function onOpen(evt) {
                    console.log("Connected")
                    button.disabled = false
                }
                
                function onClose(evt) {
                    console.log("Disconnected")
                    button.disabled = true
                    setTimeout(function() { wsConnect(url) }, 2000)
                }

                function onMessage(evt) {
                    console.log("Received " + evt.data)
                    if (evt.data.startsWith("FPS")) {
                        fps.innerText = evt.data
                    } else if (evt.data.startsWith("gameover")) {
                        topText.innerText = "GAMEOVER !"
                        topText.style.display = "block"
                    } else if (evt.data.startsWith("newhi")) {
                        const s = evt.data.split(" ")
                        topText.innerText = "NEW HI !"
                        topText.style.display = "block"
                        newhi.innerText = "Your new highscore of " + s[1] + " beat your old highscore of " + s[2] + " by " + (Number(s[1]) - Number(s[2])) + " points!" 
                        newhi.style.display = "block"
                        high.innerText = "YOUR HIGHSCORE : " + s[1];
                    }
                }

                function onError(evt) {
                    console.log("ERROR: " + evt.data)
                }

                function doSend(message) {
                    console.log("Sending: " + message)
                    ws.send(message)
                }
                
                function onPress() {
                    doSend("trigger")
                    topText.style.display = "none"
                    newhi.style.display = "none"
                }

                window.addEventListener("load", init, false)
            </script>
        </head>
        <body>
            <div style="text-align:center">
                <h1 id="topText" style="display:none"></h1>
                <h2 id="newhi" style="display:none"></h2>
                <button id="trigger" onclick="onPress()" style="padding:10%;border-radius:100px;">big round button</button>
                <h2 id="high">YOUR HIGHSCORE : 0</h2>
                <p id="fps">FPS : 0</p>
            </div>
        </body>
    </html>
    )rawliteral";
    
    return html;
}