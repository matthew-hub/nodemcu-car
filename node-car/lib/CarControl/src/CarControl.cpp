/* ************************************************* */
/* FOR CONTROLING SERVOS AND TA6586 5A DRIVER MODULE */
/*      ONLY ONE DC MOTOR AND 2 SERVO MOTOR          */
/* ************************************************* */

/* REMOTE CAR: X-FLASH 4.0 */

#include "CarControl.h"

CarControl::CarControl(uint8_t pin_d0, uint8_t pin_d1, uint8_t car_servo, uint8_t hc_servo) {
    this->pin_d0 = pin_d0;
    this->pin_d1 = pin_d1;
    this->car_servo = car_servo;
    this->hc_servo = hc_servo;

    pinMode(pin_d0, OUTPUT);
    pinMode(pin_d1, OUTPUT);
    pinMode(car_servo, OUTPUT);
    pinMode(hc_servo, OUTPUT);
}

void CarControl::forward() {
    // analogWrite(pin_d0, 100);
    // digitalWrite(pin_d1, LOW);
    Serial.println("FORWARD");
}
