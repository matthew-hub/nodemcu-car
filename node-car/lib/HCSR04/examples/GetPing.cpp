#include <Arduino.h>
#include <HCSR04.h>

#define USE_SERIAL Serial

HCSR04 SONAR(D3, D4);
void setup() {
    Serial.begin(115200);
    delay(10);
    SONAR.start();
}

void loop() {
    unsigned int uS = SONAR.ping(150);
    Serial.print("Ping: ");
    Serial.print(uS / US_ROUNDTRIP_CM);  // Convert ping time to distance in cm and print result (0 = outside set distance range)
    Serial.println("cm");

    delay(50);
}
