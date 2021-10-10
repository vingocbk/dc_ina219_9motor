#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include "config.h"
#include "AppDebug.h"

extern bool motor_is_stop[MAX_NUMBER_MOTOR];
extern uint8_t statusCurrentMotor[MAX_NUMBER_MOTOR];
extern uint8_t btn_in_control_motor[MAX_NUMBER_MOTOR];
extern bool start_check_motor_stop[MAX_NUMBER_MOTOR];
extern uint8_t reverse_motor[MAX_NUMBER_MOTOR];
//----mode run for each motor in each step
// enum
// {
//     RUN_OPEN,
//     RUN_STOP,
//     RUN_CLOSE
// };


struct _controlStep
{
    /* data */
};
struct _stepMode
{
    /* data */
};



// //--------mode open for each motor----------
// uint8_t MODE_OPEN_STEP_1[MAX_NUMBER_MOTOR] = {RUN_OPEN, RUN_OPEN, RUN_OPEN, RUN_STOP, RUN_STOP, RUN_STOP};
// uint8_t MODE_OPEN_STEP_2[MAX_NUMBER_MOTOR] = {RUN_STOP, RUN_STOP, RUN_STOP, RUN_OPEN, RUN_OPEN, RUN_OPEN};
// uint8_t MODE_OPEN_STEP_3[MAX_NUMBER_MOTOR] = {RUN_CLOSE, RUN_CLOSE, RUN_STOP, RUN_STOP, RUN_STOP, RUN_STOP};
// //--------mode close for each motor----------
// uint8_t MODE_CLOSE_STEP_1[MAX_NUMBER_MOTOR] = {RUN_OPEN, RUN_OPEN, RUN_STOP, RUN_STOP, RUN_STOP, RUN_STOP};
// uint8_t MODE_CLOSE_STEP_2[MAX_NUMBER_MOTOR] = {RUN_STOP, RUN_STOP, RUN_STOP, RUN_CLOSE, RUN_CLOSE, RUN_CLOSE};
// uint8_t MODE_CLOSE_STEP_3[MAX_NUMBER_MOTOR] = {RUN_CLOSE, RUN_CLOSE, RUN_CLOSE, RUN_STOP, RUN_STOP, RUN_STOP};


struct motor
{
    // uint116_t 
    uint32_t open_motor[MAX_NUMBER_MOTOR];
    uint32_t stop_motor[MAX_NUMBER_MOTOR];
    uint32_t close_motor[MAX_NUMBER_MOTOR];
    uint32_t convert_data_motor;
    char *data_send_motor;

    uint32_t on_out_led_mosfet[MAX_NUMBER_LED_MOSFET];
    uint32_t off_out_led_mosfet[MAX_NUMBER_LED_MOSFET];

    //-------------------------------
    uint32_t open_led[MAX_NUMBER_LED];
    uint32_t stop_led[MAX_NUMBER_LED];
    uint32_t close_led[MAX_NUMBER_LED];
    uint32_t convert_data_led;
    char *data_send_led;

    uint32_t on_led_r;
    uint32_t off_led_r;
    uint32_t on_led_g;
    uint32_t off_led_g;
    uint32_t on_led_b;
    uint32_t off_led_b;
};


void initMotor();
bool is_done_step();
void stop_motor(int number);
void open_motor(int number);
void close_motor(int number);
void on_led_mosfet(int number);
void off_led_mosfet(int number);

void open_led(int number);
void stop_led(int number);
void close_led(int number);

void set_led_R(bool status);
void set_led_G(bool status);
void set_led_B(bool status);
#endif
