#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "ds.h"
#include "webSockets.h"

/** PARAMETERS FOR CONNECTING TO WIFI NETWORK **/
const char* ssid = "Home";
const char* pwd = "genesiswifi";

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

    // The first requests sensor for measurement
    sensor.requestTemperatures();

    webSocket.begin(host, port, path);
    // event handler
    webSocket.onEvent(webSocketEvent);

    // try ever 5000 again if connection has failed
    webSocket.setReconnectInterval(5000);

    // start heartbeat (optional)
    // ping server every 15000 ms
    // expect pong from server within 3000 ms
    // consider connection disconnected if pong is not received 2 times
    webSocket.enableHeartbeat(15000, 5000, 2);
}

void loop() {
    webSocket.loop();
    /** READ TEMPERATURE DS18B20 **/
    /** HANDLE THIS LAYTER **/
    /** readTemperature(); **/
    /** *****************  **/
    // put your main code here, to run repeatedly:
}