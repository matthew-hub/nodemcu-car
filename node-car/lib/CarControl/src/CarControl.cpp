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
    carServo.write(_start_point);
    hcServo.write(_start_point);
}

void CarControl::forward() {
    if (_car_status == READY || _car_mode == OVERRIDE) {  // check if car can move forward
        _car_move = FORWARD;                              // set car move
        analogWrite(pin_d0, _car_speed);
        digitalWrite(pin_d1, LOW);
    }
}

void CarControl::reverse() {
    if (_car_status == READY || _car_mode == OVERRIDE) {  // check if car can move reverse
        _car_move = REVERSE;                              // set car move
        digitalWrite(pin_d0, LOW);
        analogWrite(pin_d1, _car_speed);
    }
}

void CarControl::brake() {  // slow brake
    _car_move = STOP;
    digitalWrite(pin_d0, LOW);
    digitalWrite(pin_d1, LOW);
}

void CarControl::handbrake() {  // drift  (^-^)
    _car_move = STOP;
    digitalWrite(pin_d0, HIGH);
    digitalWrite(pin_d1, HIGH);
}

void CarControl::set_velocity(int car_speed) {  // set car velocity (minimum 90, max 150)
    if (car_speed >= 90 && car_speed <= 150) {
        _car_speed = car_speed;
    } else if ((car_speed >= 0 && car_speed <= 255) && _car_mode == OVERRIDE) {  // override min and max
        _car_speed = car_speed;
    }
}

void CarControl::control_car_servo(int set_point) {
    carServo.write(set_point);
}

void CarControl::control_sensor_servo(int set_point) {
    hcServo.write(set_point);
}

void CarControl::update_car_status(CAR_STATUS status) {
    _car_status = status;
}

void CarControl::update_car_move(CAR_MOVE move) {
    _car_move = move;
}

void CarControl::update_car_mode(int mode) {
    _car_mode = mode;
}