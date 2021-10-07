/* ***************  */
/* HCSR04 LIBRARY  */
/* *************** */

// TODO: approach without delayMicroseconds ? * (-_-) ESP.getCycleCount() | micros()
// https://sub.nanona.fi/esp8266/timing-and-ticks.html
// some help (*_*)

#include "HCSR04.h"

HCSR04::HCSR04(uint8_t trigger_pin, uint8_t echo_pin, unsigned short max_cm_distance) {
    this->_trigger_pin = trigger_pin;
    this->_echo_pin = echo_pin;

    set_max_distance(max_cm_distance);

    pinMode(trigger_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
}

unsigned int HCSR04::ping(unsigned int max_cm_distance) {
    if (max_cm_distance > 0) set_max_distance(max_cm_distance);

    if (!ping_trigger()) return NO_ECHO;  // trigger a ping, if it returns false, return NO_ECHO to the calling function.

    while (digitalRead(_echo_pin))                 // wait for the ping echo.
        if (micros() > _max_time) return NO_ECHO;  // stop the loop and return NO_ECHO (false) if we're beyond the set maximum distance.

    return (micros() - (_max_time - _max_echo_time) - PING_OVERHEAD);  // calculate ping time, include overhead.
}

boolean HCSR04::ping_trigger() {
    digitalWrite(_trigger_pin, LOW);   // set the trigger pin low, should already be low, but this will make sure it is
    delayMicroseconds(4);              // wait for pin to go low, testing shows it needs 4uS to work every time
    digitalWrite(_trigger_pin, HIGH);  // set trigger pin high, this tells the sensor to send out a ping
    delayMicroseconds(10);             // wait long enough for the sensor to realize the trigger pin is high. Sensor specs say to wait 10uS
    digitalWrite(_trigger_pin, LOW);   // set trigger pin back to low

    if (digitalRead(_echo_pin)) return false;  // previous ping hasn't finished, abort

    _max_time = micros() + _max_echo_time + MAX_SENSOR_DELAY;  // maximum time we'll wait for ping to start

    while (!digitalRead(_echo_pin))  // wait for ping to start
        if (micros() > _max_time) return false;

    _max_time = micros() + _max_echo_time;  // ping started, set the time-out.
    return true;                            // ping started successfully.
}

void HCSR04::set_max_distance(unsigned int max_cm_distance) {
    _max_echo_time = min(max_cm_distance + 1, (unsigned int)MAX_SENSOR_DISTANCE + 1) * US_ROUNDTRIP_CM;  // Calculate the maximum distance in uS (no rounding).
}

// for later approach

// void HCSR04::measureDistance() {
//     cur_millis = micros();
//     if (trigger_state == LOW) {
//         if (cur_millis - prev_millis >= 4) {
//             prev_millis = cur_millis;
//             trigger_state = HIGH;
//             digitalWrite(_trigger_pin, trigger_state);
//         }
//     } else if (trigger_state == HIGH) {
//         if (cur_millis - prev_millis >= 10) {
//             prev_millis = cur_millis;
//             trigger_state = LOW;
//             digitalWrite(_trigger_pin, trigger_state);

//             unsigned long max_distance_duration_micro_sec;

//             float speed_sound_cm_per_micro_sec = 0.03313 + 0.0000606 * 19;  // Cair ≈ (331.3 + 0.606 ⋅ ϑ) m/s
//             /* Compute max delay based on max distance with 25% margin in microseconds */
//             max_distance_duration_micro_sec = 2.5 * _max_distance_cm / speed_sound_cm_per_micro_sec;
//             if (_max_timeout_micro_sec > 0) {
//                 max_distance_duration_micro_sec = min(max_distance_duration_micro_sec, _max_timeout_micro_sec);
//             }

//             // Measure the length of echo signal, which is equal to the time needed for sound to go there and back.
//             unsigned long duration_micro_sec = pulseIn(_echo_pin, HIGH, max_distance_duration_micro_sec);  // can't measure beyond max distance

//             float distance_cm = duration_micro_sec / 2.0 * speed_sound_cm_per_micro_sec;
//             if (distance_cm == 0 || distance_cm > _max_distance_cm) {
//                 distance = -1.0;
//             } else {
//                 distance = distance_cm;
//             }
//         }
//     }
// }