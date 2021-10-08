#include "SocketEvent.h"

SocketEvent::SocketEvent(CarControl& car, WebSocketsClient& webSocket)
    : car(car), webSocket(webSocket){};  // AAAAAAAA !!! :( member initializer lists

void SocketEvent::send_message(String& msg) {
    webSocket.sendTXT(msg.c_str(), msg.length() + 1);
}

void SocketEvent::handle_event(String payload) {
    /* CAR EVENTS */
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
        String getSetPoint = (payload.substring(1, payload.length()));
        int set_point = getSetPoint.toInt();
        car.control_car_servo(set_point);
    }

    if (payload.startsWith("S")) {  // set sensor servo position
        String getSetPoint = (payload.substring(1, payload.length()));
        int set_point = getSetPoint.toInt();
        car.control_sensor_servo(set_point);
    }

    if (payload.startsWith("V")) {  // velocity
        String getVelocity = (payload.substring(1, payload.length()));
        int velocity = getVelocity.toInt();
        car.set_velocity(velocity);
    }

    if (payload.startsWith("M")) {  // set car mode
        String getMode = (payload.substring(5, payload.length()));
        int mode = getMode.toInt();
        car.update_car_mode(mode);
    }
}

void SocketEvent::webSocket_event(WStype_t type, uint8_t* payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("[WSc] Disconnected!\n");

            car.handbrake();  //  EMERGENCY BRAKING IF DISCONNECTED
            car.update_car_status(WAITING);
            break;
        case WStype_CONNECTED: {
            String text = (char*)payload;
            String msg = "CONNECT\r\n[WSc] Connected to url: " + text + "\r\nNODE_CAR";

            send_message(msg);
            car.update_car_status(READY);
            break;
        }
        case WStype_TEXT: {
            String text = (char*)payload;  // convert to string (-_-)
            handle_event(text);
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