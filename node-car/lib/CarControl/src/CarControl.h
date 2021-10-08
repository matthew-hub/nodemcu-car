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

typedef enum {
    WAITING,
    READY
} CAR_STATUS;

typedef enum {
    FORWARD,
    REVERSE,
    STOP
} CAR_MOVE;

enum CAR_MODE {
    NORMAL,
    OVERRIDE
};

class CarControl {
   public:
    CarControl(uint8_t pin_d0, uint8_t pin_d1, uint8_t car_servo_pin, uint8_t hc_servo_pin);
    void forward();
    void reverse();
    void brake();
    void handbrake();
    void control_car_servo(int set_point);
    void control_sensor_servo(int set_point);
    void set_velocity(int car_speed);
    void update_car_status(CAR_STATUS status);
    void update_car_move(CAR_MOVE move);
    void update_car_mode(int mode);
    int _car_status = WAITING;
    int _car_mode = NORMAL;
    int _car_move = STOP;

   private:
    Servo carServo;
    Servo hcServo;
    uint8_t pin_d0, pin_d1, car_servo_pin, hc_servo_pin;
    int _car_speed = 110;
    int _start_point = 90;
};

#endif