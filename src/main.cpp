#include "main.h"

BluetoothSerial SerialBT;
struct INA219INFO ina219_info;
struct SETUPMOTOR setup_motor;
struct RUNMOTOR run_motor;
Adafruit_INA219 ina219[MAX_NUMBER_MOTOR];
extern int ngoc;
int count_to_start_check_current[MAX_NUMBER_MOTOR] = {0,0,0,0,0,0,0,0,0};

void readValueIna219()
{
    static uint32_t time_read_value = 0;
    if(millis() >= time_read_value + 100)
    {
        time_read_value = millis();
        setup_motor.value_current[MOTOR_1] = abs(ina219[MOTOR_1].getCurrent_mA());
        setup_motor.value_current[MOTOR_2] = abs(ina219[MOTOR_2].getCurrent_mA());
        setup_motor.value_current[MOTOR_3] = abs(ina219[MOTOR_3].getCurrent_mA());
        setup_motor.value_current[MOTOR_4] = abs(ina219[MOTOR_4].getCurrent_mA());
        setup_motor.value_current[MOTOR_5] = abs(ina219[MOTOR_5].getCurrent_mA());
        setup_motor.value_current[MOTOR_6] = abs(ina219[MOTOR_6].getCurrent_mA());
        setup_motor.value_current[MOTOR_7] = abs(ina219[MOTOR_7].getCurrent_mA());
        setup_motor.value_current[MOTOR_8] = abs(ina219[MOTOR_8].getCurrent_mA());
        setup_motor.value_current[MOTOR_9] = abs(ina219[MOTOR_9].getCurrent_mA());
        // setup_motor.value_bus_voltage[MOTOR_1] = abs(ina219[MOTOR_1].getBusVoltage_V());
        // setup_motor.value_bus_voltage[MOTOR_2] = abs(ina219[MOTOR_2].getBusVoltage_V());
        // setup_motor.value_bus_voltage[MOTOR_3] = abs(ina219[MOTOR_3].getBusVoltage_V());
        // setup_motor.value_bus_voltage[MOTOR_4] = abs(ina219[MOTOR_4].getBusVoltage_V());
        // setup_motor.value_bus_voltage[MOTOR_5] = abs(ina219[MOTOR_5].getBusVoltage_V());
        // setup_motor.value_bus_voltage[MOTOR_6] = abs(ina219[MOTOR_6].getBusVoltage_V());
    }
}


void sendDatatoApp()
{
    String data = "{\"1\":\"";
    data += String(setup_motor.value_current[MOTOR_1], 1);
    data += "\",\"2\":\"";
    data += String(setup_motor.value_current[MOTOR_2], 1);
    data += "\",\"3\":\"";
    data += String(setup_motor.value_current[MOTOR_3], 1);
    data += "\",\"4\":\"";
    data += String(setup_motor.value_current[MOTOR_4], 1);
    data += "\",\"5\":\"";
    data += String(setup_motor.value_current[MOTOR_5], 1);
    data += "\",\"6\":\"";
    data += String(setup_motor.value_current[MOTOR_6], 1);
    data += "\",\"7\":\"";
    data += String(setup_motor.value_current[MOTOR_7], 1);
    data += "\",\"8\":\"";
    data += String(setup_motor.value_current[MOTOR_8], 1);
    data += "\",\"9\":\"";
    data += String(setup_motor.value_current[MOTOR_9], 1);
    data += "\"}";
    for(int i = 0; i<data.length(); i++){
        SerialBT.write(data[i]);
    }
}

void setupPinMode()
{
    pinMode(BTN_IN_M1, INPUT);
    pinMode(BTN_IN_M2, INPUT);
    pinMode(BTN_IN_M3, INPUT);
    pinMode(BTN_IN_M4, INPUT);
    pinMode(BTN_IN_M5, INPUT);
    pinMode(BTN_IN_M6, INPUT);
    pinMode(BTN_IN_M7, INPUT);
    pinMode(BTN_IN_M8, INPUT);
    pinMode(BTN_IN_M9, INPUT);
    pinMode(BTN_IN_M10, INPUT);
    pinMode(BTN_MODE_SETUP, INPUT);
    pinMode(BTN_MODE_RUN, INPUT);
    pinMode(BTN_IN_LED_1, INPUT);
    pinMode(BTN_IN_LED_2, INPUT);

    initMotor();
    
}

void setupI2c()
{
    ina219[MOTOR_1] = Adafruit_INA219(ADDRESS_INA_M1);
    ina219[MOTOR_2] = Adafruit_INA219(ADDRESS_INA_M2);
    ina219[MOTOR_3] = Adafruit_INA219(ADDRESS_INA_M3);
    ina219[MOTOR_4] = Adafruit_INA219(ADDRESS_INA_M4);
    ina219[MOTOR_5] = Adafruit_INA219(ADDRESS_INA_M5);
    ina219[MOTOR_6] = Adafruit_INA219(ADDRESS_INA_M6);
    ina219[MOTOR_7] = Adafruit_INA219(ADDRESS_INA_M7);
    ina219[MOTOR_8] = Adafruit_INA219(ADDRESS_INA_M8);
    ina219[MOTOR_9] = Adafruit_INA219(ADDRESS_INA_M9);
    ina219[MOTOR_1].begin();
    ina219[MOTOR_2].begin();
    ina219[MOTOR_3].begin();
    ina219[MOTOR_4].begin();
    ina219[MOTOR_5].begin();
    ina219[MOTOR_6].begin();
    ina219[MOTOR_7].begin();
    ina219[MOTOR_8].begin();
    ina219[MOTOR_9].begin();
    delay(10);
}


void loadDataBegin()
{
    run_motor.isModeConfig = true;
    run_motor.beginChangeStep = true;
    run_motor.mode_run_open_step = OPEN_STEP_1;
    run_motor.mode_run_close_step = CLOSE_STEP_1;
    for (int i = 0; i < MAX_NUMBER_MOTOR; i++)
    {
        setup_motor.define_max_current[i] = EEPROM.read(EEPROM_MAX_CURRENT_1 + i);
        ECHO("define_max_current[");
        ECHO(i+1);
        ECHO("] : ");
        ECHOLN(setup_motor.define_max_current[i]*VALUE_CONVERT);
    }
    for (int i = 0; i < MAX_NUMBER_MOTOR; i++)
    {
        setup_motor.define_max_current[i] = EEPROM.read(EEPROM_MAX_CURRENT_1 + i);
        ECHO("define_min_current[");
        ECHO(i+1);
        ECHO("] : ");
        ECHOLN(setup_motor.define_max_current[MOTOR_1]*VALUE_CONVERT);
    }
    for (int i = 0; i < MAX_NUMBER_MOTOR; i++)
    {
        ECHO("reverse_motor[");
        ECHO(i+1);
        ECHO("] : ");
        if(EEPROM.read(EEPROM_MIN_CURRENT_1 + i) == 1)
        {
            reverse_motor[i] = true;
            ECHOLN("TRUE");
        }
        else{
            reverse_motor[i] = false;
            ECHOLN("FALSE");
        }
    }
}


void scannerI2cAddress()
{
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
    ECHOLN ();
    ECHOLN ("I2C scanner. Scanning ...");

    ina219_info.count = 0;
    for (uint8_t i = 8; i < 120; i++)
    {
        Wire.beginTransmission (i);          // Begin I2C transmission Address (i)
        if (Wire.endTransmission () == 0)  // Receive 0 = success (ACK response) 
        {
        Serial.print ("Found address: ");
        Serial.print (i, DEC);
        Serial.print (" (0x");
        Serial.print (i, HEX);     // PCF8574 7 bit address
        ECHOLN (")");
        ina219_info.address_ina[ina219_info.count] = i;
        ina219_info.count++;
        }
    }
    Serial.print ("Found ");      
    Serial.print (ina219_info.count, DEC);        // numbers of devices
    ECHOLN (" device(s).");
}




void callbackBluetooth(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
    switch (event)
	{
	case ESP_SPP_SRV_OPEN_EVT:
		ECHOLN("Client Connected");
        sendDatatoAppTicker.start();
		break;
    case ESP_SPP_CLOSE_EVT:
        ECHOLN("Client Disconnected");
        sendDatatoAppTicker.stop();
		break;
	case ESP_SPP_DATA_IND_EVT:	
		if (param->data_ind.len < MAX_RESPONSE_LENGTH) 
        {
            String data;
            for(int i = 0; i < param->data_ind.len; i++)
            {
                data += (char)param->data_ind.data[i];
            }
            ECHO("data bluetooth: ");
            ECHOLN(data);
            StaticJsonBuffer<MAX_RESPONSE_LENGTH> jsonBuffer;
            JsonObject& rootData = jsonBuffer.parseObject(data);
            if(run_motor.isModeConfig)
            {
				if (rootData.success())
                {
					String type = rootData["type"];
					String name = rootData["name"];
                    if(type == "run_no_step")
                    {
                        String data = rootData["command"];
                        for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                        {
                            if(i == name.toInt())
                            {
                                if(data == "open")
                                {
                                    open_motor(i);
                                }
                                else if(data == "stop")
                                {
                                    stop_motor(i);
                                    switch (i)
                                    {
                                    case MOTOR_1:
                                        checkCurrentMotor1.stop();
                                        break;
                                    case MOTOR_2:
                                        checkCurrentMotor2.stop();
                                        break;
                                    case MOTOR_3:
                                        checkCurrentMotor3.stop();
                                        break;
                                    case MOTOR_4:
                                        checkCurrentMotor4.stop();
                                        break;
                                    case MOTOR_5:
                                        checkCurrentMotor5.stop();
                                        break;
                                    case MOTOR_6:
                                        checkCurrentMotor6.stop();
                                        break;
                                    case MOTOR_7:
                                        checkCurrentMotor7.stop();
                                        break;
                                    case MOTOR_8:
                                        checkCurrentMotor8.stop();
                                        break;
                                    case MOTOR_9:
                                        checkCurrentMotor9.stop();
                                        break;
                                    default:
                                        break;
                                    }
                                    
                                    count_to_start_check_current[i] = 0;
                                }
                                else if(data == "close")
                                {
                                    close_motor(i);
                                }
                            }
                        }
                    }
                    else if(type == "save_data")
                    {
                        String max_current = rootData["max_current"];
                        String min_current = rootData["min_current"];
                        String reverse = rootData["reverse"];

                        for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                        {
                            if(i == name.toInt())
                            {
                                if(max_current != "")
                                {
                                    setup_motor.define_max_current[i] = max_current.toInt()/VALUE_CONVERT;
                                    // ECHOLN(setup_motor.define_max_current[i]);
                                    EEPROM.write(EEPROM_MAX_CURRENT_1 + i,setup_motor.define_max_current[i]);
                                }
                                if(min_current != "")
                                {
                                    setup_motor.define_min_current[i] = min_current.toInt();
                                    // ECHOLN(setup_motor.define_max_current[i]);
                                    EEPROM.write(EEPROM_MIN_CURRENT_1 + i,setup_motor.define_min_current[i]);
                                }
                                if(reverse == "true")
                                {
                                    reverse_motor[i] = reverse.toInt();
                                    // ECHOLN(reverse_motor[i]);
                                    EEPROM.write(EEPROM_REVERSE_MOTOR_1 + i,reverse_motor[i]);
                                }
                                EEPROM.commit();
                                break;
                            }
                        }
                        
                    }
                }
            }

        }
        break;
    default:
        break;
    }
}

void check_current_motor_1()
{
    // uint8_t status_forward;
    // status_forward = statusCurrentMotor[MOTOR_1];
    count_to_start_check_current[MOTOR_1]++;
    if(count_to_start_check_current[MOTOR_1] >= 5)
    {
         if(int(setup_motor.value_current[MOTOR_1]) < setup_motor.define_min_current[MOTOR_1] 
         || int(setup_motor.value_current[MOTOR_1]) > (setup_motor.define_max_current[MOTOR_1]*VALUE_CONVERT))
//        if(int(setup_motor.value_current[MOTOR_1]) > (setup_motor.define_max_current[MOTOR_1]*VALUE_CONVERT))
        {   
            count_to_start_check_current[MOTOR_1] = 0;
            ECHOLN("Qua Tai Motor 1");
            stop_motor(MOTOR_1);
            if(is_done_step())
            {
                run_motor.mode_run_open_step++;
                run_motor.mode_run_close_step++;
                run_motor.beginChangeStep = true;
            }
            checkCurrentMotor1.stop();
        }
    }
}

void check_current_motor_2()
{
    // uint8_t status_forward;
    // status_forward = statusCurrentMotor[MOTOR_2];
    count_to_start_check_current[MOTOR_2]++;
    if(count_to_start_check_current[MOTOR_2] >= 5)
    {
         if(int(setup_motor.value_current[MOTOR_2]) < setup_motor.define_min_current[MOTOR_2] 
         || int(setup_motor.value_current[MOTOR_2]) > (setup_motor.define_max_current[MOTOR_2]*VALUE_CONVERT))
//        if(int(setup_motor.value_current[MOTOR_2]) > (setup_motor.define_max_current[MOTOR_2]*VALUE_CONVERT))
        {   
            count_to_start_check_current[MOTOR_2] = 0;
            ECHOLN("Qua Tai Motor 2");
            stop_motor(MOTOR_2);
            if(is_done_step())
            {
                run_motor.mode_run_open_step++;
                run_motor.mode_run_close_step++;
                run_motor.beginChangeStep = true;
            }
            checkCurrentMotor2.stop();
        }
    }
}

void check_current_motor_3()
{
    // uint8_t status_forward;
    // status_forward = statusCurrentMotor[MOTOR_3];
    count_to_start_check_current[MOTOR_3]++;
    if(count_to_start_check_current[MOTOR_3] >= 5)
    {
        if(int(setup_motor.value_current[MOTOR_3]) < setup_motor.define_min_current[MOTOR_3] 
        || int(setup_motor.value_current[MOTOR_3]) > (setup_motor.define_max_current[MOTOR_3]*VALUE_CONVERT))
        // if(int(setup_motor.value_current[MOTOR_3]) > (setup_motor.define_max_current[MOTOR_3]*VALUE_CONVERT))
        {   
            count_to_start_check_current[MOTOR_3] = 0;
            ECHOLN("Qua Tai Motor 3");
            stop_motor(MOTOR_3);
            if(is_done_step())
            {
                run_motor.mode_run_open_step++;
                run_motor.mode_run_close_step++;
                run_motor.beginChangeStep = true;
            }
            checkCurrentMotor3.stop();
        }
    }
}

void check_current_motor_4()
{
    // uint8_t status_forward;
    // status_forward = statusCurrentMotor[MOTOR_4];
    count_to_start_check_current[MOTOR_4]++;
    if(count_to_start_check_current[MOTOR_4] >= 5)
    {
        if(int(setup_motor.value_current[MOTOR_4]) < setup_motor.define_min_current[MOTOR_4] 
        || int(setup_motor.value_current[MOTOR_4]) > (setup_motor.define_max_current[MOTOR_4]*VALUE_CONVERT))
        // if(int(setup_motor.value_current[MOTOR_4]) > (setup_motor.define_max_current[MOTOR_4]*VALUE_CONVERT))
        {   
            count_to_start_check_current[MOTOR_4] = 0;
            ECHOLN("Qua Tai Motor 4");
            stop_motor(MOTOR_4);
            if(is_done_step())
            {
                run_motor.mode_run_open_step++;
                run_motor.mode_run_close_step++;
                run_motor.beginChangeStep = true;
            }
            checkCurrentMotor4.stop();
        }
    }
}

void check_current_motor_5()
{
    // uint8_t status_forward;
    // status_forward = statusCurrentMotor[MOTOR_5];
    count_to_start_check_current[MOTOR_5]++;
    if(count_to_start_check_current[MOTOR_5] >= 5)
    {
        if(int(setup_motor.value_current[MOTOR_5]) < setup_motor.define_min_current[MOTOR_5] 
        || int(setup_motor.value_current[MOTOR_5]) > (setup_motor.define_max_current[MOTOR_5]*VALUE_CONVERT))
        // if(int(setup_motor.value_current[MOTOR_5]) > (setup_motor.define_max_current[MOTOR_5]*VALUE_CONVERT))
        {   
            count_to_start_check_current[MOTOR_5] = 0;
            ECHOLN("Qua Tai Motor 5");
            stop_motor(MOTOR_5);
            if(is_done_step())
            {
                run_motor.mode_run_open_step++;
                run_motor.mode_run_close_step++;
                run_motor.beginChangeStep = true;
            }
            checkCurrentMotor5.stop();
        }
    }
}

void check_current_motor_6()
{
    // uint8_t status_forward;
    // status_forward = statusCurrentMotor[MOTOR_6];
    count_to_start_check_current[MOTOR_6]++;
    if(count_to_start_check_current[MOTOR_6] >= 5)
    {
        if(int(setup_motor.value_current[MOTOR_6]) < setup_motor.define_min_current[MOTOR_6] 
        || int(setup_motor.value_current[MOTOR_6]) > (setup_motor.define_max_current[MOTOR_6]*VALUE_CONVERT))
        // if(int(setup_motor.value_current[MOTOR_6]) > (setup_motor.define_max_current[MOTOR_6]*VALUE_CONVERT))
        {   
            count_to_start_check_current[MOTOR_6] = 0;
            ECHOLN("Qua Tai Motor 6");
            stop_motor(MOTOR_6);
            if(is_done_step())
            {
                run_motor.mode_run_open_step++;
                run_motor.mode_run_close_step++;
                run_motor.beginChangeStep = true;
            }
            checkCurrentMotor6.stop();
        }
    }
}

void check_current_motor_7()
{
    // uint8_t status_forward;
    // status_forward = statusCurrentMotor[MOTOR_7];
    count_to_start_check_current[MOTOR_7]++;
    if(count_to_start_check_current[MOTOR_7] >= 5)
    {
        if(int(setup_motor.value_current[MOTOR_7]) < setup_motor.define_min_current[MOTOR_7] 
        || int(setup_motor.value_current[MOTOR_7]) > (setup_motor.define_max_current[MOTOR_7]*VALUE_CONVERT))
        // if(int(setup_motor.value_current[MOTOR_7]) > (setup_motor.define_max_current[MOTOR_7]*VALUE_CONVERT))
        {   
            count_to_start_check_current[MOTOR_7] = 0;
            ECHOLN("Qua Tai Motor 7");
            stop_motor(MOTOR_7);
            if(is_done_step())
            {
                run_motor.mode_run_open_step++;
                run_motor.mode_run_close_step++;
                run_motor.beginChangeStep = true;
            }
            checkCurrentMotor7.stop();
        }
    }
}

void check_current_motor_8()
{
    // uint8_t status_forward;
    // status_forward = statusCurrentMotor[MOTOR_8];
    count_to_start_check_current[MOTOR_8]++;
    if(count_to_start_check_current[MOTOR_8] >= 5)
    {
        if(int(setup_motor.value_current[MOTOR_8]) < setup_motor.define_min_current[MOTOR_8] 
        || int(setup_motor.value_current[MOTOR_8]) > (setup_motor.define_max_current[MOTOR_8]*VALUE_CONVERT))
        // if(int(setup_motor.value_current[MOTOR_8]) > (setup_motor.define_max_current[MOTOR_8]*VALUE_CONVERT))
        {   
            count_to_start_check_current[MOTOR_8] = 0;
            ECHOLN("Qua Tai Motor 8");
            stop_motor(MOTOR_8);
            if(is_done_step())
            {
                run_motor.mode_run_open_step++;
                run_motor.mode_run_close_step++;
                run_motor.beginChangeStep = true;
            }
            checkCurrentMotor8.stop();
        }
    }
}

void check_current_motor_9()
{
    // uint8_t status_forward;
    // status_forward = statusCurrentMotor[MOTOR_9];
    count_to_start_check_current[MOTOR_9]++;
    if(count_to_start_check_current[MOTOR_9] >= 5)
    {
        if(int(setup_motor.value_current[MOTOR_9]) < setup_motor.define_min_current[MOTOR_9] 
        || int(setup_motor.value_current[MOTOR_9]) > (setup_motor.define_max_current[MOTOR_9]*VALUE_CONVERT))
        // if(int(setup_motor.value_current[MOTOR_9]) > (setup_motor.define_max_current[MOTOR_9]*VALUE_CONVERT))
        {   
            count_to_start_check_current[MOTOR_9] = 0;
            ECHOLN("Qua Tai Motor 9");
            stop_motor(MOTOR_9);
            if(is_done_step())
            {
                run_motor.mode_run_open_step++;
                run_motor.mode_run_close_step++;
                run_motor.beginChangeStep = true;
            }
            checkCurrentMotor9.stop();
        }
    }
}


void checkButtonControl()
{
    if(!digitalRead(BTN_IN_M1))
    {
        while (!digitalRead(BTN_IN_M1))
        {
            delay(10);
        }
        btn_in_control_motor[MOTOR_1] ++;
        if(btn_in_control_motor[MOTOR_1] == 1)
        {
            open_motor(MOTOR_1);
        }
        else if(btn_in_control_motor[MOTOR_1] == 2)
        {
            stop_motor(MOTOR_1);
        }
        else if(btn_in_control_motor[MOTOR_1] == 3)
        {
            close_motor(MOTOR_1);
        }
        else if(btn_in_control_motor[MOTOR_1] == 4)
        {
            btn_in_control_motor[MOTOR_1] = 0;
            stop_motor(MOTOR_1);
        }
    }

    if(!digitalRead(BTN_IN_M2))
    {
        while (!digitalRead(BTN_IN_M2))
        {
            delay(10);
        }
        btn_in_control_motor[MOTOR_2] ++;
        if(btn_in_control_motor[MOTOR_2] == 1)
        {
            open_motor(MOTOR_2);
        }
        else if(btn_in_control_motor[MOTOR_2] == 2)
        {
            stop_motor(MOTOR_2);
        }
        else if(btn_in_control_motor[MOTOR_2] == 3)
        {
            close_motor(MOTOR_2);
        }
        else if(btn_in_control_motor[MOTOR_2] == 4)
        {
            btn_in_control_motor[MOTOR_2] = 0;
            stop_motor(MOTOR_2);
        }
    }

    if(!digitalRead(BTN_IN_M3))
    {
        while (!digitalRead(BTN_IN_M3))
        {
            delay(10);
        }
        btn_in_control_motor[MOTOR_3] ++;
        if(btn_in_control_motor[MOTOR_3] == 1)
        {
            open_motor(MOTOR_3);
        }
        else if(btn_in_control_motor[MOTOR_3] == 2)
        {
            stop_motor(MOTOR_3);
        }
        else if(btn_in_control_motor[MOTOR_3] == 3)
        {
            close_motor(MOTOR_3);
        }
        else if(btn_in_control_motor[MOTOR_3] == 4)
        {
            btn_in_control_motor[MOTOR_3] = 0;
            stop_motor(MOTOR_3);
        }
    }

    if(!digitalRead(BTN_IN_M4))
    {
        while (!digitalRead(BTN_IN_M4))
        {
            delay(10);
        }
        btn_in_control_motor[MOTOR_4] ++;
        if(btn_in_control_motor[MOTOR_4] == 1)
        {
            open_motor(MOTOR_4);
        }
        else if(btn_in_control_motor[MOTOR_4] == 2)
        {
            stop_motor(MOTOR_4);
        }
        else if(btn_in_control_motor[MOTOR_4] == 3)
        {
            close_motor(MOTOR_4);
        }
        else if(btn_in_control_motor[MOTOR_4] == 4)
        {
            btn_in_control_motor[MOTOR_4] = 0;
            stop_motor(MOTOR_4);
        }
    }

    if(!digitalRead(BTN_IN_M5))
    {
        while (!digitalRead(BTN_IN_M5))
        {
            delay(10);
        }
        btn_in_control_motor[MOTOR_5] ++;
        if(btn_in_control_motor[MOTOR_5] == 1)
        {
            open_motor(MOTOR_5);
        }
        else if(btn_in_control_motor[MOTOR_5] == 2)
        {
            stop_motor(MOTOR_5);
        }
        else if(btn_in_control_motor[MOTOR_5] == 3)
        {
            close_motor(MOTOR_5);
        }
        else if(btn_in_control_motor[MOTOR_5] == 4)
        {
            btn_in_control_motor[MOTOR_5] = 0;
            stop_motor(MOTOR_5);
        }
    }

    if(!digitalRead(BTN_IN_M6))
    {
        while (!digitalRead(BTN_IN_M6))
        {
            delay(10);
        }
        btn_in_control_motor[MOTOR_6] ++;
        if(btn_in_control_motor[MOTOR_6] == 1)
        {
            open_motor(MOTOR_6);
        }
        else if(btn_in_control_motor[MOTOR_6] == 2)
        {
            stop_motor(MOTOR_6);
        }
        else if(btn_in_control_motor[MOTOR_6] == 3)
        {
            close_motor(MOTOR_6);
        }
        else if(btn_in_control_motor[MOTOR_6] == 4)
        {
            btn_in_control_motor[MOTOR_6] = 0;
            stop_motor(MOTOR_6);
        }
    }
    

}

void checkStartCalCurrent()
{
    if(start_check_motor_stop[MOTOR_1])
    {
        start_check_motor_stop[MOTOR_1] = false;
        checkCurrentMotor1.start();
    }
    if(start_check_motor_stop[MOTOR_2])
    {
        start_check_motor_stop[MOTOR_2] = false;
        checkCurrentMotor2.start();
    }
    if(start_check_motor_stop[MOTOR_3])
    {
        start_check_motor_stop[MOTOR_3] = false;
        checkCurrentMotor3.start();
    }
    if(start_check_motor_stop[MOTOR_4])
    {
        start_check_motor_stop[MOTOR_4] = false;
        checkCurrentMotor4.start();
    }
    if(start_check_motor_stop[MOTOR_5])
    {
        start_check_motor_stop[MOTOR_5] = false;
        checkCurrentMotor5.start();
    }
    if(start_check_motor_stop[MOTOR_6])
    {
        start_check_motor_stop[MOTOR_6] = false;
        checkCurrentMotor6.start();
    }
    if(start_check_motor_stop[MOTOR_7])
    {
        start_check_motor_stop[MOTOR_7] = false;
        checkCurrentMotor7.start();
    }
    if(start_check_motor_stop[MOTOR_8])
    {
        start_check_motor_stop[MOTOR_8] = false;
        checkCurrentMotor8.start();
    }
    if(start_check_motor_stop[MOTOR_9])
    {
        start_check_motor_stop[MOTOR_9] = false;
        checkCurrentMotor9.start();
    }
}

void tickerUpdate()
{
    sendDatatoAppTicker.update();
    checkCurrentMotor1.update();
    checkCurrentMotor2.update();
    checkCurrentMotor3.update();
    checkCurrentMotor4.update();
    checkCurrentMotor5.update();
    checkCurrentMotor6.update();
    checkCurrentMotor7.update();
    checkCurrentMotor8.update();
    checkCurrentMotor9.update();
    sendDatatoAppTicker.update();
}

void checkButtonConfigModeRun()
{
    static bool check_mode_setup = true;
    if(!digitalRead(BTN_MODE_SETUP) && check_mode_setup)
    {
        delay(100);
        ECHOLN("MODE SETUP");
        check_mode_setup = false;
        run_motor.isModeConfig = true;
    }
    else if(digitalRead(BTN_MODE_SETUP) && !check_mode_setup)
    {
        delay(100);
        ECHOLN("MODE RUN");
        check_mode_setup = true;
        run_motor.isModeConfig = false;
    }
}

void setup()
{
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  //Brownout detector was triggered
    Serial.begin (SERIAL_BAUDRATE);  
    Wire.begin (SDA_PIN, SCL_PIN);   // sda= GPIO_21 /scl= GPIO_22
    EEPROM.begin(MAX_SIZE_EEPROM_BUFFER);
    setupPinMode();
    scannerI2cAddress();
    setupI2c();
    loadDataBegin();

    SerialBT.flush();
    SerialBT.end(); 
    if(!SerialBT.begin("Test Motor")){
        ECHOLN("An error occurred initializing Bluetooth");
    }else{
        ECHOLN("Bluetooth initialized");
    }
	SerialBT.register_callback(callbackBluetooth);

    for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
    {
        stop_motor(i);
    }
}



void loop()
{
    readValueIna219();
    tickerUpdate();
    checkStartCalCurrent();
    checkButtonConfigModeRun();

    if(!run_motor.isModeConfig)
    {
        if(digitalRead(BTN_MODE_RUN))
        {
            run_motor.mode_run_close_step = CLOSE_STEP_1;
            switch (run_motor.mode_run_open_step)
            {
            case OPEN_STEP_1:
                if(run_motor.beginChangeStep)
                {
                    ECHOLN("START MODE RUN OPEN STEP 1");
                    run_motor.beginChangeStep = false;
                    open_motor(MOTOR_1);
                    open_motor(MOTOR_2);
                    open_motor(MOTOR_3);
                    stop_motor(MOTOR_4);
                    stop_motor(MOTOR_5);
                    stop_motor(MOTOR_6);
                }
                break;
            case OPEN_STEP_2:
                if(run_motor.beginChangeStep)
                {
                    ECHOLN("START MODE RUN OPEN STEP 2");
                    run_motor.beginChangeStep = false;
                    stop_motor(MOTOR_1);
                    stop_motor(MOTOR_2);
                    stop_motor(MOTOR_3);
                    open_motor(MOTOR_4);
                    open_motor(MOTOR_5);
                    open_motor(MOTOR_6);
                }
                break;
            case OPEN_STEP_3:
                if(run_motor.beginChangeStep)
                {
                    ECHOLN("START MODE RUN OPEN STEP 3");
                    run_motor.beginChangeStep = false;
                    close_motor(MOTOR_1);
                    close_motor(MOTOR_2);
                    stop_motor(MOTOR_3);
                    stop_motor(MOTOR_4);
                    stop_motor(MOTOR_5);
                    stop_motor(MOTOR_6);
                }
                break;
            case DONE_MODE_OPEN:
                ECHOLN("DONE RUN OPEN MODE");
                run_motor.mode_run_open_step++;
                break;
            default:
                break;
            }
        }
        //-----------------------------------------------
        else
        {
            run_motor.mode_run_open_step = OPEN_STEP_1;
            switch (run_motor.mode_run_close_step)
            {
            case CLOSE_STEP_1:
                if(run_motor.beginChangeStep)
                {
                    ECHOLN("START MODE RUN CLOSE STEP 1");
                    run_motor.beginChangeStep = false;
                    open_motor(MOTOR_1);
                    open_motor(MOTOR_2);
                    stop_motor(MOTOR_3);
                    stop_motor(MOTOR_4);
                    stop_motor(MOTOR_5);
                    stop_motor(MOTOR_6);
                }
                break;
            case CLOSE_STEP_2:
                if(run_motor.beginChangeStep)
                {
                    ECHOLN("START MODE RUN CLOSE STEP 2");
                    run_motor.beginChangeStep = false;
                    stop_motor(MOTOR_1);
                    stop_motor(MOTOR_2);
                    stop_motor(MOTOR_3);
                    close_motor(MOTOR_4);
                    close_motor(MOTOR_5);
                    close_motor(MOTOR_6);
                }
                break;
            case CLOSE_STEP_3:
                if(run_motor.beginChangeStep)
                {
                    ECHOLN("START MODE RUN CLOSE STEP 3");
                    run_motor.beginChangeStep = false;
                    close_motor(MOTOR_1);
                    close_motor(MOTOR_2);
                    close_motor(MOTOR_3);
                    stop_motor(MOTOR_4);
                    stop_motor(MOTOR_5);
                    stop_motor(MOTOR_6);
                }
                break;
            case DONE_MODE_CLOSE:
                ECHOLN("DONE RUN CLOSE MODE");
                run_motor.mode_run_close_step++;
                break;
            default:
                break;
            }
        }
        


    }
    else{
        checkButtonControl();
    }
    


}
