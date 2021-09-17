#ifndef DC_INA219_H
#define DC_INA219_H

#include <Arduino.h>
#include "config.h"
#include "AppDebug.h"
#include <Wire.h>
#include "Adafruit_INA219.h"
#include "motor.h"
#include "soc/soc.h"  //Brownout detector was triggered
#include "soc/rtc_cntl_reg.h"
#include "EEPROM.h"
#include "ArduinoJson.h"
#include "BluetoothSerial.h"
#include "Ticker.h"

struct INA219INFO
{
    uint8_t address_ina[MAX_NUMBER_MOTOR];
    uint8_t count;
};

struct SETUPMOTOR
{
    float value_bus_voltage[MAX_NUMBER_MOTOR];
    float value_current[MAX_NUMBER_MOTOR];    //gia tri hien tai 
    uint8_t define_max_current[MAX_NUMBER_MOTOR];   //gia tri max. neu vuot gia tri nay thi se ngat
    uint8_t define_min_current[MAX_NUMBER_MOTOR];   //gia tri min. neu vuot gia tri nay thi se ngat
    
    // uint8_t define_time_return[MAX_NUMBER_MOTOR];
};

// struct VALUEINA
// {
//     /* data */
// };


struct RUNMOTOR
{
    bool isModeConfig;
    bool beginChangeStep;
    int mode_run_open_step;
    int mode_run_close_step;
};


void readValueIna219();
void sendDatatoApp();
void setupPinMode();
void setupI2c();
void loadDataBegin();
void scannerI2cAddress();
void callbackBluetooth(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
void check_current_motor_1();
void check_current_motor_2();
void check_current_motor_3();
void check_current_motor_4();
void check_current_motor_5();
void check_current_motor_6();
void check_current_motor_7();
void check_current_motor_8();
void check_current_motor_9();
void checkButtonControl();
void checkStartCalCurrent();
void tickerUpdate();
void checkButtonConfigModeRun();

Ticker checkCurrentMotor1(check_current_motor_1, 100);   //every 100ms
Ticker checkCurrentMotor2(check_current_motor_2, 100);   //every 100ms
Ticker checkCurrentMotor3(check_current_motor_3, 100);   //every 100ms
Ticker checkCurrentMotor4(check_current_motor_4, 100);   //every 100ms
Ticker checkCurrentMotor5(check_current_motor_5, 100);   //every 100ms
Ticker checkCurrentMotor6(check_current_motor_6, 100);   //every 100ms
Ticker checkCurrentMotor7(check_current_motor_7, 100);   //every 100ms
Ticker checkCurrentMotor8(check_current_motor_8, 100);   //every 100ms
Ticker checkCurrentMotor9(check_current_motor_9, 100);   //every 100ms
Ticker sendDatatoAppTicker(sendDatatoApp, 1000);   //every 500ms


#endif
