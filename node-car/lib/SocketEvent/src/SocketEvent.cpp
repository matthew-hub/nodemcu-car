#include "SocketEvent.h"

SocketEvent::SocketEvent(CarControl& car, WebSocketsClient& webSocket)
    : car(car), webSocket(webSocket){};  // AAAAAAAA !!! :( member initializer lists

void SocketEvent::sendMessage(String& msg) {
    webSocket.sendTXT(msg.c_str(), msg.length() + 1);
}

void SocketEvent::handleEvent(String payload) {
    if (payload.startsWith("V")) {  // velocity
        String getVelocity = (payload.substring(payload.indexOf("V") + 1, payload.length()));
        int velocity = getVelocity.toInt();
        car.setVelocity(velocity);
    }

    if (payload.startsWith("F")) {  // forward
        car.forward();
    }

    if (payload.startsWith("R")) {  // reverse
        car.reverse();
    }

    if (payload.startsWith("B")) {  // brake
        car.brake();
    }

    if (payload.startsWith("H")) {  // handbrake
        car.handbrake();
    }

    if (payload.startsWith("C")) {  // set car servo position
        String getSetPoint = (payload.substring(payload.indexOf("C") + 1, payload.length()));
        int setPoint = getSetPoint.toInt();
        car.controlCarServo(setPoint);
    }

    if (payload.startsWith("S")) {  // set sensor servo position
        String getSetPoint = (payload.substring(payload.indexOf("S") + 1, payload.length()));
        int setPoint = getSetPoint.toInt();
        car.controlSensorServo(setPoint);
    }
}

void SocketEvent::webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("[WSc] Disconnected!\n");
            break;
        case WStype_CONNECTED: {
            String text = (char*)payload;

            String msg = "CONNECT\r\naccept-version:1.1,1.0\r\nheart-beat:10000,10000\r\n\r\n[WSc] Connected to url: " + text;
            sendMessage(msg);
            break;
        }
        case WStype_TEXT: {
            String text = (char*)payload;  // convert to string (-_-)
            String msg = "[WSc] get text: " + text;
            handleEvent(text);
            sendMessage(msg);

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