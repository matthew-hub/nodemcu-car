/* *************** */
/* HCSR04 LIBRARY  */
/* *************** */

/**
  Specifications: HCSR04 
  Working Voltage: DC 5.0V
  Working Current: 15mA
  Working Frequency: 40Hz
  Max Range: 4m
  Min Range: 2cm
  Measuring angle: 15 degree
  Trigger Input Signal: 10uS TTL pulse
  Echo Output Signal: Input TTL lever signal and the range in proportion
  Dimension (mm): 45*20*15
**/

#ifndef HCSR04_H
#define HCSR04_H

#define MAX_SENSOR_DISTANCE 200  // maximum sensor distance can be as high as 500cm, no reason to wait for ping longer than sound takes to travel this distance and back
#define US_ROUNDTRIP_CM 57       // microseconds (uS) it takes sound to travel round-trip 1cm (2cm total), uses integer to save compiled code space

#define NO_ECHO 0              // value returned if there's no ping echo within the specified MAX_SENSOR_DISTANCE or max_cm_distance
#define MAX_SENSOR_DELAY 5800  // maximum uS it takes for sensor to start the ping
#define PING_OVERHEAD 5        // ping overhead in microseconds (uS)

#include <Arduino.h>

class HCSR04 {
   public:
    HCSR04(uint8_t trigger_pin, uint8_t echo_pin, unsigned short max_cm_distance = MAX_SENSOR_DISTANCE);
    unsigned int ping(unsigned int max_cm_distance = 0);

   private:
    boolean ping_trigger();
    void set_max_distance(unsigned int max_cm_distance);
    uint8_t _trigger_pin, _echo_pin;
    unsigned long _max_time;
    unsigned int _max_echo_time;
};

#endif