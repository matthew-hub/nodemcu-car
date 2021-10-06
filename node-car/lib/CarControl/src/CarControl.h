/* ************************************************* */
/* FOR CONTROLING SERVOS AND TA6586 5A DRIVER MODULE */
/*      ONLY ONE DC MOTOR AND 2 SERVO MOTOR          */
/* ************************************************* */

/* REMOTE CAR: X-FLASH 4.0 */

/** DRIVER MODULE
 Specifications:  TA6586 5A
 Working Voltage: DC 3.0V-14V
 Input Signal Voltage: DC 2.2V-6.0V
 Drive Current: 5A
 Stand-by Current: 10uA
 Working Temperature: -20 to 85 DegC
 Dimensions (mm): 38x27x18
 Mounting hole diameter (mm): 2
**/

/** SERVOS
 Specifications: Tower Pro SG90 9g (steering system) 
 Working Voltage: DC  4.8V – 7.2V
 Gear speed: 0.12s / 60 ° (4.8V)
 Torque: 1.2 - 1.8 kg / cm (4.8 V)
 Dimensions (mm): 22mm x 12mm x 22,7mm

 Speccifications: Redox S90 MG (for HCSR04 sensor)
 Gear speed: 0.1 / 60 ° (4.8V)
 Torque:  2,2 kg (4,8V)
 Dimensions (mm): 23.1x12mm, 2x24.8mm
**/

#ifndef CARCONTROL_H
#define CARCONTROL_H

#include <Arduino.h>
#include <Servo.h>

class CarControl {
   public:
    CarControl(uint8_t pin_d0, uint8_t pin_d1, uint8_t car_servo_pin, uint8_t hc_servo_pin);
    void setVelocity(int car_speed);
    void forward();
    void reverse();
    void brake();
    void handbrake();
    void controlCarServo(int set_point);
    void controlSensorServo(int set_point);

   private:
    Servo carServo;
    Servo hcServo;
    uint8_t pin_d0, pin_d1, car_servo_pin, hc_servo_pin;
    int car_speed = 110;
    int start_point = 90;
};

#endif