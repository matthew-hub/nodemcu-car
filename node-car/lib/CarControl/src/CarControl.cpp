/* ************************************************* */
/* FOR CONTROLING SERVOS AND TA6586 5A DRIVER MODULE */
/*      ONLY ONE DC MOTOR AND 2 SERVO MOTOR          */
/* ************************************************* */

/* REMOTE CAR: X-FLASH 4.0 */

#include "CarControl.h"

CarControl::CarControl(uint8_t pin_d0, uint8_t pin_d1, uint8_t car_servo_pin, uint8_t hc_servo_pin) {
    this->pin_d0 = pin_d0;
    this->pin_d1 = pin_d1;
    this->car_servo_pin = car_servo_pin;
    this->hc_servo_pin = hc_servo_pin;

    // set gpio mode
    pinMode(pin_d0, OUTPUT);
    pinMode(pin_d1, OUTPUT);
    pinMode(car_servo_pin, OUTPUT);
    pinMode(hc_servo_pin, OUTPUT);

    // attach servos
    carServo.attach(car_servo_pin);
    hcServo.attach(hc_servo_pin);
    // set start point of servo
    carServo.write(start_point);
    hcServo.write(start_point);
}

void CarControl::setVelocity(int car_speed) {
    this->car_speed = car_speed;
}

void CarControl::forward() {
    analogWrite(pin_d0, car_speed);
    digitalWrite(pin_d1, LOW);
}

void CarControl::reverse() {
    digitalWrite(pin_d0, LOW);
    analogWrite(pin_d1, car_speed);
}

void CarControl::brake() {
    digitalWrite(pin_d0, LOW);
    digitalWrite(pin_d1, LOW);
}

void CarControl::handbrake() {
    digitalWrite(pin_d0, HIGH);
    digitalWrite(pin_d1, HIGH);
}

void CarControl::controlCarServo(int set_point) {
    carServo.write(set_point);
}
void CarControl::controlSensorServo(int set_point) {
    hcServo.write(set_point);
}