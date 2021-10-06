#include <Arduino.h>
#include <CarControl.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <SocketEvent.h>
#include <WebSocketsClient.h>

#include "ds.h"
#include "hcsr04.h"

/* LIST OF USED GPIOs */
#include "carGPIO.h"

#define USE_SERIAL Serial

/** PARAMETERS FOR CONNECTING TO WIFI NETWORK **/
const char* ssid = "Home";
const char* pwd = "genesiswifi";

/** WEBSOCKETS CONNECTION PARAMETERS **/
const char* path = "/echo";
const char* host = "192.168.0.2";
const int port = 3000;

// Car flash;

// WEBSOCKETS CLIENT
WebSocketsClient webSocket;

HCSR04 hcsr04;

// CONTROLING CAR AND SERVO
CarControl XFLASH(MOTORIN0, MOTORIN1, CARSERVO, HCSERVO);

// SOCKETEVENT HANDLE FOR CAR
SocketEvent SOCKET(XFLASH, webSocket);

void setup() {
    Serial.begin(115200);
    delay(10);

    Serial.println("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, pwd);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    };
    Serial.println("");
    Serial.print("Connected, IP Adress: ");
    Serial.println(WiFi.localIP());

    // DS18B20 sensors setup
    sensor.begin();

    // HCSR04 sesnsor setup
    hcsr04.start();

    SOCKET.carGo();
    // The first requests sensor for measurement
    sensor.requestTemperatures();

    webSocket.begin(host, port, path);
    // event handler
    webSocket.onEvent([&](WStype_t typ, uint8_t* payload, size_t length) {
        SOCKET.webSocketEvent(typ, payload, length);
    });

    // webSocket.onEvent(SOCKET.webSocketEvent());

    // try ever 5000 again if connection has failed
    webSocket.setReconnectInterval(5000);

    // start heartbeat (optional)
    // ping server every 15000 ms
    // expect pong from server within 3000 ms
    // consider connection disconnected if pong is not received 2 times
    webSocket.enableHeartbeat(15000, 5000, 2);

    /** PINMODE **/
    pinMode(TRIGGER, OUTPUT);
    pinMode(ECHO, INPUT);
}

void loop() {
    webSocket.loop();
    /** READ TEMPERATURE DS18B20 **/
    /** HANDLE THIS LATER **/
    // readTemperature();
    // delay(100);
    /** *****************  **/

    /** MASURE DISTANCE HCSR04 **/
    /** HANDLE THIS LATER **/
    // hc_sensor.MEASURE_DISTANCE();
    // Serial.println(hc_sensor.getDistance());
    /** *****************  **/

    delay(50);
}