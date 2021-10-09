#include <Arduino.h>
#include <CarControl.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <HCSR04.h>
#include <Hash.h>
#include <OneWire.h>
#include <SocketEvent.h>
#include <WebSocketsClient.h>

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

/** DALLAS TEMPERATURE PARAMETERS **/
int resolution = 12;  // set bits resolution for more accurate measure  | (9,10,11,12)
unsigned long last_temp_request = 0;
unsigned long delay_in_millis = 0;
int temperature = 0;  // need only integers

// WEBSOCKETS CLIENT
WebSocketsClient webSocket;

// CONTROLING CAR AND SERVOS
CarControl XFLASH(MOTORIN0, MOTORIN1, CARSERVO, HCSERVO);

// SOCKETEVENT HANDLE FOR CAR
SocketEvent SOCKET(XFLASH, webSocket);

// CRATE ONEWIRE FOR DS18B20 SENSOR
OneWire DS_WIRE(ONEWIRE_DS);

// DS18B20 LIB
DallasTemperature SENSOR(&DS_WIRE);

// DS18B20 SENSOR ADDRES
DeviceAddress tempDeviceAddress = {0x28, 0xAB, 0x0E, 0x9A, 0x0A, 0x00, 0x00, 0x88};

// HCSR04 ULTRASONIC DISTANCE SENSOR
HCSR04 HC_SENSOR(TRIGGER, ECHO);

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
    SENSOR.begin();
    SENSOR.setResolution(tempDeviceAddress, resolution);  // set sensor addres and resolution
    SENSOR.setWaitForConversion(false);                   //
    SENSOR.requestTemperatures();                         // the first requests sensor for measurement
    delay_in_millis = 750 / (1 << (12 - resolution));     // 93.75ms if resolution 9, 750ms if resolution 12
    last_temp_request = millis();                         // set the time of the last temperature request

    webSocket.begin(host, port, path);
    // event handler
    webSocket.onEvent([&](WStype_t typ, uint8_t* payload, size_t length) {
        SOCKET.webSocket_event(typ, payload, length);
    });

    // try ever 1000 again if connection has failed
    webSocket.setReconnectInterval(1000);

    // start heartbeat (optional)
    // ping server every 1000 ms
    // expect pong from server within 300 ms
    // consider connection disconnected if pong is not received 1 times
    webSocket.enableHeartbeat(1000, 300, 1);

    /** PINMODE **/
    pinMode(16, OUTPUT);  // built-in led signaling start
}

void loop() {
    webSocket.loop();

    /** READ TEMPERATURE DS18B20 **/

    if (millis() - last_temp_request >= delay_in_millis)  // waited long enough??
    {
        int to_compare = SENSOR.getTempCByIndex(0);  // to compare temperatures
        if (abs(to_compare - temperature) >= 1) {    // get absolute values, check temperature changes one degree
            temperature = SENSOR.getTempCByIndex(0);

            /* creating a string to be parsed by JSON */
            String temp("{\"sensor\":{\"name\":\"DS18B20\",\"data\":" + String(temperature) + "}}");
            webSocket.sendTXT(temp);  // send string to websocket server
        }

        SENSOR.requestTemperatures();
        last_temp_request = millis();
    }

    /* ***************** */

    // TODO: ping HCSR04 every 29ms for better measure
    /** MEASURE DISTANCE HCSR04 **/

    int uS = HC_SENSOR.ping(150);
    String pingString = "PING_uS:" + String(uS);
    // SOCKET.send_message(pingString);

    // EMERGENCY BRAKING IF THE DISTANCE IS APPROXIMATELY < 30CM
    // TODO: forward drive lock, set the emergency braking distance dependent on car velocity
    if ((uS > 0 && uS < 1710) && XFLASH._car_move == FORWARD) {
        webSocket.sendTXT("{\"WALL\": true}");
        XFLASH.handbrake();
    }

    /* ***************** */
}
