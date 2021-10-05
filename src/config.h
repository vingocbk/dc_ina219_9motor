#ifndef CONFIG_H
#define CONFIG_H

#define SERIAL_BAUDRATE     115200

#define SCL_PIN             22
#define SDA_PIN             21

#define BTN_IN_M1           4
#define BTN_IN_M2           5
#define BTN_IN_M3           34
#define BTN_IN_M4           15
#define BTN_IN_M5           35
#define BTN_IN_M6           32
#define BTN_IN_M7           33
#define BTN_IN_M8           26
#define BTN_IN_M9           0
#define BTN_IN_M10          2
#define BTN_IN_LED_1        12
// #define BTN_IN_LED_2        25  //27
#define BTN_IN_LED_2        27  //design

#define BTN_MODE_SETUP      23
// #define BTN_MODE_RUN        27  //25
#define BTN_MODE_RUN        25  //design

#define DATA_PIN_LED        16
#define LATCH_PIN_LED       13  
#define CLOCK_PIN_LED       14

#define DATA_PIN_MOTOR      19
#define LATCH_PIN_MOTOR     18      //17
#define CLOCK_PIN_MOTOR     17      //18


#define ADDRESS_INA_M1          0x40
#define ADDRESS_INA_M2          0x41
#define ADDRESS_INA_M3          0x42
#define ADDRESS_INA_M4          0x43
#define ADDRESS_INA_M5          0x44
#define ADDRESS_INA_M6          0x45
#define ADDRESS_INA_M7          0x46
#define ADDRESS_INA_M8          0x47
#define ADDRESS_INA_M9          0x48
#define ADDRESS_INA_M10         0x49
#define ADDRESS_INA_M11         0x50

#define MAX_NUMBER_MOTOR        9
#define MAX_NUMBER_LED_MOSFET   2
// #define MAX_ALL_MOTOR           12
#define MAX_NUMBER_LED          18
#define MAX_RESPONSE_LENGTH     2048

#define MAX_SIZE_EEPROM_BUFFER      512
#define EEPROM_MAX_CURRENT_1        1
#define EEPROM_MIN_CURRENT_1        11
#define EEPROM_REVERSE_MOTOR_1      21
#define EEPROM_OPEN_STEP_1_MTOR_1   31
#define EEPROM_OPEN_STEP_2_MTOR_1   41
#define EEPROM_OPEN_STEP_3_MTOR_1   51
#define EEPROM_CLOSE_STEP_1_MTOR_1  61
#define EEPROM_CLOSE_STEP_2_MTOR_1  71
#define EEPROM_CLOSE_STEP_3_MTOR_1  81


#define VALUE_CONVERT           10
#define MIN_CURRENT_MOTOR_CHECK_START       200          //mA

#define MIN_VALUE_READ_RX       500
#define MAX_VALUE_READ_RX       2500
#define ON_LED      true
#define OFF_LED     false

#define CONFIG_HOLD_TIME        3000

enum
{
    MOTOR_1 = 0,
    MOTOR_2,
    MOTOR_3,
    MOTOR_4,
    MOTOR_5,
    MOTOR_6,
    MOTOR_7,
    MOTOR_8,
    MOTOR_9
};


// #define 
enum
{
    LED_R = 0,
    LED_G,
    LED_B,
    LED_1,
    LED_2,
    LED_3,
    LED_4,
    LED_5,
    LED_6,
    LED_7,
    LED_8,
    LED_9,
    LED_10,
    LED_11,
    LED_12
};

enum
{
    LED_MOSFET_1 = 0,
    LED_MOSFET_2
};

// int LED_R      = 0;
// int LED_G      = 1;   
// int LED_B      = 2;
// int LED_1      = 3;
// int LED_2      = 4;
// int LED_3      = 5;
// int LED_4      = 6;
// int LED_5      = 7;
// int LED_6      = 8;
// int LED_7      = 9;
// int LED_8      = 10;
// int LED_9      = 11;
// int LED_10     = 12; 
// int LED_11     = 13;
// int LED_12     = 14;

enum
{
    MOTOR_STOP = 0,
    MOTOR_OPEN,
    MOTOR_CLOSE
};

enum
{
    OPEN_STEP_1 = 1,
    OPEN_STEP_2,
    OPEN_STEP_3,
    DONE_MODE_OPEN
};

enum
{
    CLOSE_STEP_1 = 1,
    CLOSE_STEP_2,
    CLOSE_STEP_3,
    DONE_MODE_CLOSE
};


#endif
