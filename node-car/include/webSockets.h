#include <Hash.h>
#include <WebSocketsClient.h>
#define USE_SERIAL Serial

/** WEBSOCKETS CONNECTION PARAMETERS **/
const char* path = "/echo";
const char* host = "192.168.0.2";
const int port = 3000;

// WEBSOCKETS
WebSocketsClient webSocket;

void sendMessage(String& msg) {
    webSocket.sendTXT(msg.c_str(), msg.length() + 1);
}

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("[WSc] Disconnected!\n");
            break;
        case WStype_CONNECTED: {
            USE_SERIAL.printf("[WSc] Connected to url: %s\n", payload);

            String msg = "CONNECT\r\naccept-version:1.1,1.0\r\nheart-beat:10000,10000\r\n\r\n";
            sendMessage(msg);
            break;
        }
        case WStype_TEXT: {
            // #####################
            // handle protocol
            // #####################

            String text = (char*)payload;
            USE_SERIAL.printf("[WSc] get text: %s\n", payload);

            if (text.startsWith("s")) {
                USE_SERIAL.println("s");
                String xVal = (text.substring(text.indexOf("s") + 1, text.length()));
            }

            break;
        }
        case WStype_BIN:
            USE_SERIAL.printf("[WSc] get binary length: %u\n", length);
            hexdump(payload, length);

            // send data to server
            // webSocket.sendBIN(payload, length);
            break;
        default:
            break;
    }
}
