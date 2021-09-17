#include "motor.h"


struct motor Set_Motor;
bool motor_is_stop[MAX_NUMBER_MOTOR] = {false,false,false,false,false,false,false,false,false};
uint8_t statusCurrentMotor[MAX_NUMBER_MOTOR] = {MOTOR_STOP,MOTOR_STOP,MOTOR_STOP,MOTOR_STOP,MOTOR_STOP,MOTOR_STOP,MOTOR_STOP,MOTOR_STOP,MOTOR_STOP};
uint8_t btn_in_control_motor[MAX_NUMBER_MOTOR] = {0,0,0,0,0,0,0,0,0};
bool start_check_motor_stop[MAX_NUMBER_MOTOR] = {false,false,false,false,false,false,false,false,false};
bool reverse_motor[MAX_NUMBER_MOTOR] = {false,false,false,false,false,false,false,false,false};
bool is_done_step()
{
    if(
        motor_is_stop[MOTOR_1]
        && motor_is_stop[MOTOR_2]
        && motor_is_stop[MOTOR_3]
        && motor_is_stop[MOTOR_4]
        && motor_is_stop[MOTOR_5]
        && motor_is_stop[MOTOR_6]
        && motor_is_stop[MOTOR_7]
        && motor_is_stop[MOTOR_8]
        && motor_is_stop[MOTOR_9]
    )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void initMotor()
{
    pinMode(DATA_PIN_LED, OUTPUT);
    pinMode(LATCH_PIN_LED, OUTPUT);
    pinMode(CLOCK_PIN_LED, OUTPUT);
    pinMode(DATA_PIN_MOTOR, OUTPUT);
    pinMode(LATCH_PIN_MOTOR, OUTPUT);
    pinMode(CLOCK_PIN_MOTOR, OUTPUT);

    Set_Motor.open_motor[MOTOR_1]               = 0b10000000000000000000000000000000;
    Set_Motor.close_motor[MOTOR_1]              = 0b01000000000000000000000000000000;
    Set_Motor.stop_motor[MOTOR_1]               = 0b00111111111111111111111111111111;

    Set_Motor.open_motor[MOTOR_2]               = 0b00100000000000000000000000000000;
    Set_Motor.close_motor[MOTOR_2]              = 0b00010000000000000000000000000000;
    Set_Motor.stop_motor[MOTOR_2]               = 0b11001111111111111111111111111111;

    Set_Motor.open_motor[MOTOR_3]               = 0b00001000000000000000000000000000;
    Set_Motor.close_motor[MOTOR_3]              = 0b00000100000000000000000000000000;
    Set_Motor.stop_motor[MOTOR_3]               = 0b11110011111111111111111111111111;

    Set_Motor.open_motor[MOTOR_4]               = 0b00000010000000000000000000000000;
    Set_Motor.close_motor[MOTOR_4]              = 0b00000001000000000000000000000000;
    Set_Motor.stop_motor[MOTOR_4]               = 0b11111100111111111111111111111111;

    Set_Motor.open_motor[MOTOR_5]               = 0b00000000100000000000000000000000;
    Set_Motor.close_motor[MOTOR_5]              = 0b00000000010000000000000000000000;
    Set_Motor.stop_motor[MOTOR_5]               = 0b11111111001111111111111111111111;

    Set_Motor.open_motor[MOTOR_6]               = 0b00000000001000000000000000000000;
    Set_Motor.close_motor[MOTOR_6]              = 0b00000000000100000000000000000000;
    Set_Motor.stop_motor[MOTOR_6]               = 0b11111111110011111111111111111111;

    Set_Motor.open_motor[MOTOR_7]               = 0b00000000000010000000000000000000;
    Set_Motor.close_motor[MOTOR_7]              = 0b00000000000001000000000000000000;
    Set_Motor.stop_motor[MOTOR_7]               = 0b11111111111100111111111111111111;

    Set_Motor.open_motor[MOTOR_8]               = 0b00000000000000100000000000000000;
    Set_Motor.close_motor[MOTOR_8]              = 0b00000000000000010000000000000000;
    Set_Motor.stop_motor[MOTOR_8]               = 0b11111111111111001111111111111111;

    Set_Motor.open_motor[MOTOR_9]               = 0b00000000000000001000000000000000;
    Set_Motor.close_motor[MOTOR_9]              = 0b00000000000000000100000000000000;
    Set_Motor.stop_motor[MOTOR_9]               = 0b11111111111111110011111111111111;

    Set_Motor.on_out_led_mosfet[LED_MOSFET_1]   = 0b00000000000000000000100000000000;
    Set_Motor.off_out_led_mosfet[LED_MOSFET_1]  = 0b11111111111111111111011111111111;

    Set_Motor.on_out_led_mosfet[LED_MOSFET_2]   = 0b00000000000000000000010000000000;
    Set_Motor.off_out_led_mosfet[LED_MOSFET_2]  = 0b11111111111111111111101111111111;
    
    Set_Motor.convert_data_motor    = 0;

    //-------------------------------------------------
    Set_Motor.close_led[MOTOR_1]    = 0b01111111111111111111111111111111;
    Set_Motor.open_led[MOTOR_1]     = 0b10111111111111111111111111111111;
    Set_Motor.stop_led[MOTOR_1]     = 0b11000000000000000000000000000000;

    Set_Motor.close_led[MOTOR_2]    = 0b11011111111111111111111111111111;
    Set_Motor.open_led[MOTOR_2]     = 0b11101111111111111111111111111111;
    Set_Motor.stop_led[MOTOR_2]     = 0b00110000000000000000000000000000;

    Set_Motor.close_led[MOTOR_3]    = 0b11110111111111111111111111111111;
    Set_Motor.open_led[MOTOR_3]     = 0b11111011111111111111111111111111;
    Set_Motor.stop_led[MOTOR_3]     = 0b00001100000000000000000000000000;

    Set_Motor.close_led[MOTOR_4]    = 0b11111101111111111111111111111111;
    Set_Motor.open_led[MOTOR_4]     = 0b11111110111111111111111111111111;
    Set_Motor.stop_led[MOTOR_4]     = 0b00000011000000000000000000000000;

    Set_Motor.open_led[MOTOR_8]     = 0b11111111011111111111111111111111;
    Set_Motor.close_led[MOTOR_8]    = 0b11111111101111111111111111111111;
    Set_Motor.stop_led[MOTOR_8]     = 0b00000000110000000000000000000000;

    Set_Motor.open_led[MOTOR_7]     = 0b11111111110111111111111111111111;
    Set_Motor.close_led[MOTOR_7]    = 0b11111111111011111111111111111111;
    Set_Motor.stop_led[MOTOR_7]     = 0b00000000001100000000000000000000;

    Set_Motor.open_led[MOTOR_6]     = 0b11111111111101111111111111111111;
    Set_Motor.close_led[MOTOR_6]    = 0b11111111111110111111111111111111;
    Set_Motor.stop_led[MOTOR_6]     = 0b00000000000011000000000000000000;

    Set_Motor.open_led[MOTOR_5]     = 0b11111111111111011111111111111111;
    Set_Motor.close_led[MOTOR_5]    = 0b11111111111111101111111111111111;
    Set_Motor.stop_led[MOTOR_5]     = 0b00000000000000110000000000000000;

    Set_Motor.open_led[MOTOR_9]     = 0b11111111111111110111111111111111;
    Set_Motor.close_led[MOTOR_9]    = 0b11111111111111111011111111111111;
    Set_Motor.stop_led[MOTOR_9]     = 0b00000000000000001100000000000000;

    Set_Motor.off_led_r             = 0b00000000000000000000100000000000;
    Set_Motor.on_led_r              = 0b11111111111111111111011111111111;
    Set_Motor.off_led_g             = 0b00000000000000000000010000000000;
    Set_Motor.on_led_g              = 0b11111111111111111111101111111111;
    Set_Motor.off_led_b             = 0b00000000000000000000001000000000;
    Set_Motor.on_led_b              = 0b11111111111111111111110111111111;

    Set_Motor.convert_data_led      = 0b11111111111111111111111111111111;
}


void stop_motor(int number){
    ECHO("Stop Motor ");
    ECHOLN(number + 1);

    motor_is_stop[number] = true;
    statusCurrentMotor[number] = MOTOR_STOP;
    if(btn_in_control_motor[number] == 1)     //motor stop, press button on board next to close or open
    {
        btn_in_control_motor[number] = 2;
    }
    else if(btn_in_control_motor[number] == 3)     //motor stop, press button on board next to close or open
    {
        btn_in_control_motor[number] = 0;
    }
    
    Set_Motor.convert_data_motor = Set_Motor.convert_data_motor & Set_Motor.stop_motor[number];
    Set_Motor.data_send_motor = (char*) &Set_Motor.convert_data_motor;
    for(int i = 0; i < 4; i++)
    {
        digitalWrite(LATCH_PIN_MOTOR, LOW);
        shiftOut(DATA_PIN_MOTOR, CLOCK_PIN_MOTOR, LSBFIRST, *(Set_Motor.data_send_motor + i));
        digitalWrite(LATCH_PIN_MOTOR, HIGH);
    }

    stop_led(number);
}

//----------------------------------------------------------------------------------------
void open_motor(int number)
{
    // stop_motor(number);
    ECHO("Open Motor ");
    ECHOLN(number + 1);

    start_check_motor_stop[number] = true;
    motor_is_stop[number] = false;
    statusCurrentMotor[number] = MOTOR_OPEN;
    btn_in_control_motor[number] = 1;     //motor open, press button on board next to stop


    //stop first
    Set_Motor.convert_data_motor = Set_Motor.convert_data_motor & Set_Motor.stop_motor[number];
    Set_Motor.data_send_motor = (char*) &Set_Motor.convert_data_motor;
    for(int i = 0; i < 4; i++)
    {
        digitalWrite(LATCH_PIN_MOTOR, LOW);
        shiftOut(DATA_PIN_MOTOR, CLOCK_PIN_MOTOR, LSBFIRST, *(Set_Motor.data_send_motor + i));
        digitalWrite(LATCH_PIN_MOTOR, HIGH);
    }

    //Open
    if(reverse_motor[number])
    {
        Set_Motor.convert_data_motor = Set_Motor.convert_data_motor | Set_Motor.open_motor[number];
        Set_Motor.data_send_motor = (char*) &Set_Motor.convert_data_motor;
        for(int i = 0; i < 4; i++)
        {
            digitalWrite(LATCH_PIN_MOTOR, LOW);
            shiftOut(DATA_PIN_MOTOR, CLOCK_PIN_MOTOR, LSBFIRST, *(Set_Motor.data_send_motor + i));
            digitalWrite(LATCH_PIN_MOTOR, HIGH);
        }
    }
    else
    {
        Set_Motor.convert_data_motor = Set_Motor.convert_data_motor | Set_Motor.close_motor[number];
        Set_Motor.data_send_motor = (char*) &Set_Motor.convert_data_motor;
        for(int i = 0; i < 4; i++)
        {
            digitalWrite(LATCH_PIN_MOTOR, LOW);
            shiftOut(DATA_PIN_MOTOR, CLOCK_PIN_MOTOR, LSBFIRST, *(Set_Motor.data_send_motor + i));
            digitalWrite(LATCH_PIN_MOTOR, HIGH);
        }
    }

    
    stop_led(number);
    open_led(number);
}

//----------------------------------------------------------------------------------------
void close_motor(int number)
{
    // stop_motor(number);
    ECHO("Close Motor ");
    ECHOLN(number + 1);

    start_check_motor_stop[number] = true;
    motor_is_stop[number] = false;
    statusCurrentMotor[number] = MOTOR_CLOSE;
    
    btn_in_control_motor[number] = 3;     //motor close, press button on board next to stop

    //stop first
    Set_Motor.convert_data_motor = Set_Motor.convert_data_motor & Set_Motor.stop_motor[number];
    Set_Motor.data_send_motor = (char*) &Set_Motor.convert_data_motor;
    for(int i = 0; i < 4; i++)
    {
        digitalWrite(LATCH_PIN_MOTOR, LOW);
        shiftOut(DATA_PIN_MOTOR, CLOCK_PIN_MOTOR, LSBFIRST, *(Set_Motor.data_send_motor + i));
        digitalWrite(LATCH_PIN_MOTOR, HIGH);
    }

    //close
    if(reverse_motor[number])
    {
        Set_Motor.convert_data_motor = Set_Motor.convert_data_motor | Set_Motor.close_motor[number];
        Set_Motor.data_send_motor = (char*) &Set_Motor.convert_data_motor;
        for(int i = 0; i < 4; i++)
        {
            digitalWrite(LATCH_PIN_MOTOR, LOW);
            shiftOut(DATA_PIN_MOTOR, CLOCK_PIN_MOTOR, LSBFIRST, *(Set_Motor.data_send_motor + i));
            digitalWrite(LATCH_PIN_MOTOR, HIGH);
        }
    }
    else
    {
        Set_Motor.convert_data_motor = Set_Motor.convert_data_motor | Set_Motor.open_motor[number];
        Set_Motor.data_send_motor = (char*) &Set_Motor.convert_data_motor;
        for(int i = 0; i < 4; i++)
        {
            digitalWrite(LATCH_PIN_MOTOR, LOW);
            shiftOut(DATA_PIN_MOTOR, CLOCK_PIN_MOTOR, LSBFIRST, *(Set_Motor.data_send_motor + i));
            digitalWrite(LATCH_PIN_MOTOR, HIGH);
        }
    }

    stop_led(number);
    close_led(number);
}

//----------------------------------------------------------------------------------------
void open_led(int number)
{
    // ECHO("led: ");
    // ECHOLN(number);
    Set_Motor.convert_data_led = Set_Motor.convert_data_led | Set_Motor.open_led[number];
    Set_Motor.data_send_led = (char*) &Set_Motor.convert_data_led;
    digitalWrite(LATCH_PIN_LED, LOW);
    shiftOut(DATA_PIN_LED, CLOCK_PIN_LED, LSBFIRST, *Set_Motor.data_send_led);
    digitalWrite(LATCH_PIN_LED, HIGH);

    digitalWrite(LATCH_PIN_LED, LOW);
    shiftOut(DATA_PIN_LED, CLOCK_PIN_LED, LSBFIRST, *(Set_Motor.data_send_led + 1));
    digitalWrite(LATCH_PIN_LED, HIGH);
}

//----------------------------------------------------------------------------------------
void close_led(int number)
{
    // ECHO("led: ");
    // ECHOLN(number);
    Set_Motor.convert_data_led = Set_Motor.convert_data_led & Set_Motor.close_led[number];
    Set_Motor.data_send_led = (char*) &Set_Motor.convert_data_led;
    digitalWrite(LATCH_PIN_LED, LOW);
    shiftOut(DATA_PIN_LED, CLOCK_PIN_LED, LSBFIRST, *Set_Motor.data_send_led);
    digitalWrite(LATCH_PIN_LED, HIGH);

    digitalWrite(LATCH_PIN_LED, LOW);
    shiftOut(DATA_PIN_LED, CLOCK_PIN_LED, LSBFIRST, *(Set_Motor.data_send_led + 1));
    digitalWrite(LATCH_PIN_LED, HIGH);
}
//----------------------------------------------------------------------------------------
void stop_led(int number)
{
    // ECHO("led: ");
    // ECHOLN(number);
    Set_Motor.convert_data_led = Set_Motor.convert_data_led & Set_Motor.stop_led[number];
    Set_Motor.data_send_led = (char*) &Set_Motor.convert_data_led;
    digitalWrite(LATCH_PIN_LED, LOW);
    shiftOut(DATA_PIN_LED, CLOCK_PIN_LED, LSBFIRST, *Set_Motor.data_send_led);
    digitalWrite(LATCH_PIN_LED, HIGH);

    digitalWrite(LATCH_PIN_LED, LOW);
    shiftOut(DATA_PIN_LED, CLOCK_PIN_LED, LSBFIRST, *(Set_Motor.data_send_led + 1));
    digitalWrite(LATCH_PIN_LED, HIGH);
}
