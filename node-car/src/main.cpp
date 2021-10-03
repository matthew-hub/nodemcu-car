#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>

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
    Serial.println("WiFi connected");
}

void loop() {
    // put your main code here, to run repeatedly:
}