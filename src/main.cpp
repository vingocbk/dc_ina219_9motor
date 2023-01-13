#include "main.h"

BluetoothSerial SerialBT;
struct INA219INFO ina219_info;
struct SETUPMOTOR setup_motor;
struct RUNMOTOR run_motor;
Adafruit_INA219 ina219[MAX_NUMBER_MOTOR];
// Adafruit_INA219 ina219_bat;
extern int count_to_start_check_current[MAX_NUMBER_MOTOR];

PCA9685 pwmController(ADDRESS_PCA9685);           // Library using Wire1 @400kHz
// Linearly interpolates between standard 2.5%/12.5% phase length (102/512) for -90°/+90°
PCA9685_ServoEval pwmServo;
SemaphoreHandle_t xMutexI2C;

void readValueIna219()
{
    static uint8_t count = 0;
	count ++;
	static float current[MAX_NUMBER_MOTOR] = {0,0,0,0,0,0,0,0,0};    //gia tri hien tai 
    static float voltage = 0;
    current[MOTOR_1] += abs(ina219[MOTOR_1].getCurrent_mA());
	current[MOTOR_2] += abs(ina219[MOTOR_2].getCurrent_mA());
	current[MOTOR_3] += abs(ina219[MOTOR_3].getCurrent_mA());
	current[MOTOR_4] += abs(ina219[MOTOR_4].getCurrent_mA());
	current[MOTOR_5] += abs(ina219[MOTOR_5].getCurrent_mA());
	current[MOTOR_6] += abs(ina219[MOTOR_6].getCurrent_mA());
	current[MOTOR_7] += abs(ina219[MOTOR_7].getCurrent_mA());
	current[MOTOR_8] += abs(ina219[MOTOR_8].getCurrent_mA());
	current[MOTOR_9] += abs(ina219[MOTOR_9].getCurrent_mA());
    voltage += 11.0*analogRead(INPUT_VOLTAGE)*3.3/4096.0;
	if(count >= 10){
		for(int i = 0; i < MAX_NUMBER_MOTOR; i++){
			setup_motor.value_current[i] = current[i]/count;
            current[i] = 0;
            setup_motor.total_power += setup_motor.value_current[i]*0.1/3600.0;
		}
        float calib12V = 1.1538;
        setup_motor.value_voltage_battery = calib12V*voltage/(count);
        voltage = 0;
		count = 0;
	}
    // ECHOLN(analogRead(INPUT_VOLTAGE));
}


void sendDatatoApp()
{
    // ECHOLN("sendDatatoApp");
    String data = "{\"1\":[";
    data += String(setup_motor.value_current[MOTOR_1], 1);
    data += ",";
    data += String(setup_motor.value_current[MOTOR_2], 1);
    data += ",";
    data += String(setup_motor.value_current[MOTOR_3], 1);
    data += ",";
    data += String(setup_motor.value_current[MOTOR_4], 1);
    data += ",";
    data += String(setup_motor.value_current[MOTOR_5], 1);
    data += ",";
    data += String(setup_motor.value_current[MOTOR_6], 1);
    data += ",";
    data += String(setup_motor.value_current[MOTOR_7], 1);
    data += ",";
    data += String(setup_motor.value_current[MOTOR_8], 1);
    data += ",";
    data += String(setup_motor.value_current[MOTOR_9], 1);
    // data += "]}";
    data += ",";
    data += String(setup_motor.value_voltage_battery, 1);
    data += ",";
    data += String(setup_motor.total_power);
	data += ",";
	if(APP_FLAG(MODE_CONFIG)){
		data += "0";		// 0 - mode Manual
	}
	else{
		data += "1";		// 1 - mode Auto
	}
    data += "]}";
    for(int i = 0; i<data.length(); i++){
        SerialBT.write(data[i]);
    }
}

void sendDataMinMaxCurrenttoApp()
{
	ECHOLN("sendDataMinMaxCurrenttoApp");
    // Min Current
    String data = "{\"2\":[";
    data += String(setup_motor.define_min_current[MOTOR_1]);
    data += ",";
    data += String(setup_motor.define_min_current[MOTOR_2]);
    data += ",";
    data += String(setup_motor.define_min_current[MOTOR_3]);
    data += ",";
    data += String(setup_motor.define_min_current[MOTOR_4]);
    data += ",";
    data += String(setup_motor.define_min_current[MOTOR_5]);
    data += ",";
    data += String(setup_motor.define_min_current[MOTOR_6]);
    data += ",";
    data += String(setup_motor.define_min_current[MOTOR_7]);
    data += ",";
    data += String(setup_motor.define_min_current[MOTOR_8]);
    data += ",";
    data += String(setup_motor.define_min_current[MOTOR_9]);

    //Max Current
    data += ",";
    data += String(setup_motor.define_max_current[MOTOR_1]);
    data += ",";
    data += String(setup_motor.define_max_current[MOTOR_2]);
    data += ",";
    data += String(setup_motor.define_max_current[MOTOR_3]);
    data += ",";
    data += String(setup_motor.define_max_current[MOTOR_4]);
    data += ",";
    data += String(setup_motor.define_max_current[MOTOR_5]);
    data += ",";
    data += String(setup_motor.define_max_current[MOTOR_6]);
    data += ",";
    data += String(setup_motor.define_max_current[MOTOR_7]);
    data += ",";
    data += String(setup_motor.define_max_current[MOTOR_8]);
    data += ",";
    data += String(setup_motor.define_max_current[MOTOR_9]);

    //Select Motor
    data += ",";
    data += String(select_motor[MOTOR_1]);
    data += ",";
    data += String(select_motor[MOTOR_2]);
    data += ",";
    data += String(select_motor[MOTOR_3]);
    data += ",";
    data += String(select_motor[MOTOR_4]);
    data += ",";
    data += String(select_motor[MOTOR_5]);
    data += ",";
    data += String(select_motor[MOTOR_6]);
    data += ",";
    data += String(select_motor[MOTOR_7]);
    data += ",";
    data += String(select_motor[MOTOR_8]);
    data += ",";
    data += String(select_motor[MOTOR_9]);

    //Select Servo
    data += ",";
    data += String(select_servo[MOTOR_1]);
    data += ",";
    data += String(select_servo[MOTOR_2]);
    data += ",";
    data += String(select_servo[MOTOR_3]);
    data += ",";
    data += String(select_servo[MOTOR_4]);
    data += ",";
    data += String(select_servo[MOTOR_5]);
    data += ",";
    data += String(select_servo[MOTOR_6]);
    data += ",";
    data += String(select_servo[MOTOR_7]);
    data += ",";
    data += String(select_servo[MOTOR_8]);
    data += ",";
    data += String(select_servo[MOTOR_9]);

    //Reverve
    data += ",";
    data += String(reverse_motor[MOTOR_1]);
    data += ",";
    data += String(reverse_motor[MOTOR_2]);
    data += ",";
    data += String(reverse_motor[MOTOR_3]);
    data += ",";
    data += String(reverse_motor[MOTOR_4]);
    data += ",";
    data += String(reverse_motor[MOTOR_5]);
    data += ",";
    data += String(reverse_motor[MOTOR_6]);
    data += ",";
    data += String(reverse_motor[MOTOR_7]);
    data += ",";
    data += String(reverse_motor[MOTOR_8]);
    data += ",";
    data += String(reverse_motor[MOTOR_9]);

    //Min Angle
    data += ",";
    data += String(setup_motor.define_start_angle[MOTOR_1]);
    data += ",";
    data += String(setup_motor.define_start_angle[MOTOR_2]);
    data += ",";
    data += String(setup_motor.define_start_angle[MOTOR_3]);
    data += ",";
    data += String(setup_motor.define_start_angle[MOTOR_4]);
    data += ",";
    data += String(setup_motor.define_start_angle[MOTOR_5]);
    data += ",";
    data += String(setup_motor.define_start_angle[MOTOR_6]);
    data += ",";
    data += String(setup_motor.define_start_angle[MOTOR_7]);
    data += ",";
    data += String(setup_motor.define_start_angle[MOTOR_8]);
    data += ",";
    data += String(setup_motor.define_start_angle[MOTOR_9]);

    //Max Current
    data += ",";
    data += String(setup_motor.define_end_angle[MOTOR_1]);
    data += ",";
    data += String(setup_motor.define_end_angle[MOTOR_2]);
    data += ",";
    data += String(setup_motor.define_end_angle[MOTOR_3]);
    data += ",";
    data += String(setup_motor.define_end_angle[MOTOR_4]);
    data += ",";
    data += String(setup_motor.define_end_angle[MOTOR_5]);
    data += ",";
    data += String(setup_motor.define_end_angle[MOTOR_6]);
    data += ",";
    data += String(setup_motor.define_end_angle[MOTOR_7]);
    data += ",";
    data += String(setup_motor.define_end_angle[MOTOR_8]);
    data += ",";
    data += String(setup_motor.define_end_angle[MOTOR_9]);

    //Time Servo
    data += ",";
    data += String(time_run_servo[MOTOR_1]);
    data += ",";
    data += String(time_run_servo[MOTOR_2]);
    data += ",";
    data += String(time_run_servo[MOTOR_3]);
    data += ",";
    data += String(time_run_servo[MOTOR_4]);
    data += ",";
    data += String(time_run_servo[MOTOR_5]);
    data += ",";
    data += String(time_run_servo[MOTOR_6]);
    data += ",";
    data += String(time_run_servo[MOTOR_7]);
    data += ",";
    data += String(time_run_servo[MOTOR_8]);
    data += ",";
    data += String(time_run_servo[MOTOR_9]);
    data += "]}";
    for(int i = 0; i<data.length(); i++){
        SerialBT.write(data[i]);
    }
}

void sendDataSteptoApp()
{
	ECHOLN("sendDataSteptoApp");
    // return;
    //Open Step
    String data = "{\"3\":[";
    for(int j = 0; j < MAX_NUMBER_MOTOR; j++)
    {
        data += setup_motor.open_step_1[j];
        data += ",";
    }
    for(int j = 0; j < MAX_NUMBER_MOTOR; j++)
    {
        data += setup_motor.open_step_2[j];
        data += ",";
    }
    for(int j = 0; j < MAX_NUMBER_MOTOR; j++)
    {
        data += setup_motor.open_step_3[j];
        data += ",";
    }
    //Close Step
    for(int j = 0; j < MAX_NUMBER_MOTOR; j++)
    {
        data += setup_motor.close_step_1[j];
        data += ",";
    }
    for(int j = 0; j < MAX_NUMBER_MOTOR; j++)
    {
        data += setup_motor.close_step_2[j];
        data += ",";
    }
    for(int j = 0; j < MAX_NUMBER_MOTOR; j++)
    {
        data += setup_motor.close_step_3[j];
        if(j == (MAX_NUMBER_MOTOR - 1))
        {
            break;
        }
        data += ",";
    }
    data += "]}";
    for(int i = 0; i<data.length(); i++){
        SerialBT.write(data[i]);
    }
}

void sendDataVoltagetoApp()
{
	ECHOLN("sendDataVoltagetoApp");
	String data = "{\"4\":[";
    for(int j = 0; j < MAX_NUMBER_MOTOR; j++)
    {
        data += set_voltage_motor[j];
        if(j == (MAX_NUMBER_MOTOR - 1))
        {
            break;
        }
        data += ",";
    }
	data += "]}";
    for(int i = 0; i<data.length(); i++){
        SerialBT.write(data[i]);
    }
}


// void IRAM_ATTR readRxModeRunRising()
// {
//     attachInterrupt(digitalPinToInterrupt(BTN_MODE_RUN), readRxModeRunFalling, FALLING);    
//     run_motor.prev_time_mode_run = micros();
// }
// void IRAM_ATTR readRxModeRunFalling()
// {
//     attachInterrupt(digitalPinToInterrupt(BTN_MODE_RUN), readRxModeRunRising, RISING);    
//     run_motor.pwm_value_mode_run = micros()-run_motor.prev_time_mode_run;
//     // ECHOLN(run_motor.pwm_value_mode_run);
// }
// void IRAM_ATTR readRxLed1Rising()
// {
//     attachInterrupt(digitalPinToInterrupt(BTN_IN_LED_1), readRxLed1Falling, FALLING);    
//     run_motor.prev_time_led1 = micros();
// }
// void IRAM_ATTR readRxLed1Falling()
// {
//     attachInterrupt(digitalPinToInterrupt(BTN_IN_LED_1), readRxLed1Rising, RISING);    
//     run_motor.pwm_value_led1 = micros()-run_motor.prev_time_led1;
// }
// void IRAM_ATTR readRxLed2Rising()
// {
//     attachInterrupt(digitalPinToInterrupt(BTN_MODE_RUN), readRxLed2Falling, FALLING);    
//     run_motor.prev_time_led2 = micros();
// }
// void IRAM_ATTR readRxLed2Falling()
// {
//     attachInterrupt(digitalPinToInterrupt(BTN_MODE_RUN), readRxLed2Rising, RISING);    
//     run_motor.pwm_value_led2 = micros()-run_motor.prev_time_led2;
// }

void setupPinMode()
{
    pinMode(BTN_IN_M1, INPUT_PULLUP);
    pinMode(BTN_IN_M2, INPUT_PULLUP);
    pinMode(BTN_IN_M3, INPUT_PULLUP);
    pinMode(BTN_IN_M4, INPUT_PULLUP);
    pinMode(BTN_IN_M5, INPUT_PULLUP);
    pinMode(BTN_IN_M6, INPUT_PULLUP);
    pinMode(BTN_IN_M7, INPUT_PULLUP);
    pinMode(BTN_IN_M8, INPUT_PULLUP);
    pinMode(BTN_IN_M9, INPUT_PULLUP);
    pinMode(BTN_MODE_SETUP, INPUT_PULLUP);
    pinMode(BTN_MODE_RUN, INPUT);
    pinMode(BTN_IN_LED_1, INPUT);
    pinMode(BTN_IN_LED_2, INPUT);

    pinMode(DATA_PIN_LED, OUTPUT);
    pinMode(LATCH_PIN_LED, OUTPUT);
    pinMode(CLOCK_PIN_LED, OUTPUT);
    pinMode(DATA_PIN_MOTOR, OUTPUT);
    pinMode(LATCH_PIN_MOTOR, OUTPUT);
    pinMode(CLOCK_PIN_MOTOR, OUTPUT);
    initMotor();

    analogReadResolution(12);
    analogSetWidth(12);

    // attachInterrupt(digitalPinToInterrupt(BTN_MODE_RUN), readRxModeRunRising, RISING);    
    // attachInterrupt(digitalPinToInterrupt(BTN_IN_LED_1), readRxLed1Rising, RISING);
    // attachInterrupt(digitalPinToInterrupt(BTN_IN_LED_2), readRxLed2Rising, RISING);
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
    // ina219_bat = Adafruit_INA219(ADDRESS_INA_M10);
    ina219[MOTOR_1].begin();
    ina219[MOTOR_2].begin();
    ina219[MOTOR_3].begin();
    ina219[MOTOR_4].begin();
    ina219[MOTOR_5].begin();
    ina219[MOTOR_6].begin();
    ina219[MOTOR_7].begin();
    ina219[MOTOR_8].begin();
    ina219[MOTOR_9].begin();
    // ina219_bat.begin();
    delay(10);
    pwmController.resetDevices();       // Resets all PCA9685 devices on i2c line
    pwmController.init();               // Initializes module using default totem-pole driver mode, and default disabled phase balancer
    pwmController.setPWMFreqServo();    // 50Hz provides standard 20ms servo phase length
    delay(10);
    xMutexI2C = xSemaphoreCreateMutex();
}


void loadDataBegin()
{
    run_motor.isModeConfig = false;
    run_motor.beginChangeStep = true;
    run_motor.mode_run_open_step = OPEN_STEP_1;
    run_motor.mode_run_close_step = CLOSE_STEP_1;

    for(int i = MOTOR_1; i < MAX_NUMBER_MOTOR; i++)
    {
        setup_motor.value_current[i] = 0;
        setup_motor.isMotorOn[i] = false;
    }
    for (int i = 0; i < MAX_NUMBER_MOTOR; i++)
    {
        setup_motor.define_min_current[i] = EEPROM.read(EEPROM_MIN_CURRENT_1 + i);
        ECHO("define_min_current[");
        ECHO(i+1);
        ECHO("] : ");
        ECHOLN(setup_motor.define_min_current[i]);
    }
    for (int i = 0; i < MAX_NUMBER_MOTOR; i++)
    {
        setup_motor.define_max_current[i] = (EEPROM.read(EEPROM_MAX_CURRENT_1 + 2*i) << 8) | EEPROM.read(EEPROM_MAX_CURRENT_1 + 2*i +1);
        ECHO("define_max_current[");
        ECHO(i+1);
        ECHO("] : ");
        ECHOLN(setup_motor.define_max_current[i]);
    }
    for (int i = 0; i < MAX_NUMBER_MOTOR; i++)
    {
        ECHO("select_motor[");
        ECHO(i+1);
        ECHO("] : ");
        if(EEPROM.read(EEPROM_SELECT_MOTOR_1 + i) == 1)
        {
            select_motor[i] = 1;
            ECHOLN("TRUE");
        }
        else{
            select_motor[i] = 0;
            ECHOLN("FALSE");
        }
    }
    for (int i = 0; i < MAX_NUMBER_MOTOR; i++)
    {
        ECHO("select_servo[");
        ECHO(i+1);
        ECHO("] : ");
        if(EEPROM.read(EEPROM_SELECT_SERVO_1 + i) == 1)
        {
            select_servo[i] = 1;
            ECHOLN("TRUE");
        }
        else{
            select_servo[i] = 0;
            ECHOLN("FALSE");
        }
    }
    for (int i = 0; i < MAX_NUMBER_MOTOR; i++)
    {
        ECHO("reverse_motor[");
        ECHO(i+1);
        ECHO("] : ");
        if(EEPROM.read(EEPROM_REVERSE_MOTOR_1 + i) == 1)
        {
            reverse_motor[i] = 1;
            ECHOLN("TRUE");
        }
        else{
            reverse_motor[i] = 0;
            ECHOLN("FALSE");
        }
    }
    for (int i = 0; i < MAX_NUMBER_MOTOR; i++)
    {
        setup_motor.define_start_angle[i] = (EEPROM.read(EEPROM_START_ANGLE_SERVO_1 + 2*i) << 8) | EEPROM.read(EEPROM_START_ANGLE_SERVO_1 + 2*i +1);
        ECHO("define_start_angle[");
        ECHO(i+1);
        ECHO("] : ");
        ECHOLN(setup_motor.define_start_angle[i]);
    }
    for (int i = 0; i < MAX_NUMBER_MOTOR; i++)
    {
        setup_motor.define_end_angle[i] = (EEPROM.read(EEPROM_END_ANGLE_SERVO_1 + 2*i) << 8) | EEPROM.read(EEPROM_END_ANGLE_SERVO_1 + 2*i +1);
        ECHO("define_end_angle[");
        ECHO(i+1);
        ECHO("] : ");
        ECHOLN(setup_motor.define_end_angle[i]);
    }
    for (int i = 0; i < MAX_NUMBER_MOTOR; i++)
    {
        time_run_servo[i] = (EEPROM.read(EEPROM_TIME_SERVO_1 + 2*i) << 8) | EEPROM.read(EEPROM_TIME_SERVO_1 + 2*i +1);
        ECHO("time_run_servo[");
        ECHO(i+1);
        ECHO("] : ");
        ECHOLN(time_run_servo[i]);
    }
    for (int i = 0; i < MAX_NUMBER_MOTOR; i++)
    {
        set_voltage_motor[i] = EEPROM.read(EEPROM_SET_VOLTAGE_MOTOR_1 + i);
        ECHO("set_voltage_motor[");
        ECHO(i+1);
        ECHO("] : ");
		switch (set_voltage_motor[i])
		{
		case PWM_MOTOR_12V:
			ECHOLN(PWM_VOLATGE_MOTOR_12V);
			break;
		case PWM_MOTOR_11V:
			ECHOLN(PWM_VOLATGE_MOTOR_11V);
			break;
		case PWM_MOTOR_10V:
			ECHOLN(PWM_VOLATGE_MOTOR_10V);
			break;
		case PWM_MOTOR_9V:
			ECHOLN(PWM_VOLATGE_MOTOR_9V);
			break;
		case PWM_MOTOR_8V:
			ECHOLN(PWM_VOLATGE_MOTOR_8V);
			break;
		case PWM_MOTOR_7V:
			ECHOLN(PWM_VOLATGE_MOTOR_7V);
			break;
		case PWM_MOTOR_6V:
			ECHOLN(PWM_VOLATGE_MOTOR_6V);
			break;
		default:
			ECHOLN(PWM_VOLATGE_MOTOR_12V);
			set_voltage_motor[i] = PWM_MOTOR_12V;
			break;
		}
    }
    //Check Open Step 1
    ECHO("OPEN STEP 1: ");
    for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
    {
        setup_motor.open_step_1[i] = EEPROM.read(EEPROM_OPEN_STEP_1_MTOR_1 + i);
        if(setup_motor.open_step_1[i] != MOTOR_OPEN && setup_motor.open_step_1[i] != MOTOR_CLOSE && setup_motor.open_step_1[i] != MOTOR_STOP)
        {
            setup_motor.open_step_1[i] = MOTOR_STOP;
        }
        ECHO(setup_motor.open_step_1[i]);
        ECHO(" - ");
    }
    //Check Open Step 2
    ECHOLN("");
    ECHO("OPEN STEP 2: ");
    for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
    {
        setup_motor.open_step_2[i] = EEPROM.read(EEPROM_OPEN_STEP_2_MTOR_1 + i);
        if(setup_motor.open_step_2[i] != MOTOR_OPEN && setup_motor.open_step_2[i] != MOTOR_CLOSE && setup_motor.open_step_2[i] != MOTOR_STOP)
        {
            setup_motor.open_step_2[i] = MOTOR_STOP;
        }
        ECHO(setup_motor.open_step_2[i]);
        ECHO(" - ");
    }
    //Check Open Step 3
    ECHOLN("");
    ECHO("OPEN STEP 3: ");
    for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
    {
        setup_motor.open_step_3[i] = EEPROM.read(EEPROM_OPEN_STEP_3_MTOR_1 + i);
        if(setup_motor.open_step_3[i] != MOTOR_OPEN && setup_motor.open_step_3[i] != MOTOR_CLOSE && setup_motor.open_step_3[i] != MOTOR_STOP)
        {
            setup_motor.open_step_3[i] = MOTOR_STOP;
        }
        ECHO(setup_motor.open_step_3[i]);
        ECHO(" - ");
    }
    //Check Close Step 1
    ECHOLN("");
    ECHO("CLOSE STEP 1: ");
    for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
    {
        setup_motor.close_step_1[i] = EEPROM.read(EEPROM_CLOSE_STEP_1_MTOR_1 + i);
        if(setup_motor.close_step_1[i] != MOTOR_OPEN && setup_motor.close_step_1[i] != MOTOR_CLOSE && setup_motor.close_step_1[i] != MOTOR_STOP)
        {
            setup_motor.close_step_1[i] = MOTOR_STOP;
        }
        ECHO(setup_motor.close_step_1[i]);
        ECHO(" - ");
    }
    //Check Close Step 2
    ECHOLN("");
    ECHO("CLOSE STEP 2: ");
    for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
    {
        setup_motor.close_step_2[i] = EEPROM.read(EEPROM_CLOSE_STEP_2_MTOR_1 + i);
        if(setup_motor.close_step_2[i] != MOTOR_OPEN && setup_motor.close_step_2[i] != MOTOR_CLOSE && setup_motor.close_step_2[i] != MOTOR_STOP)
        {
            setup_motor.close_step_2[i] = MOTOR_STOP;
        }
        ECHO(setup_motor.close_step_2[i]);
        ECHO(" - ");
    }
    //Check Close Step 3
    ECHOLN("");
    ECHO("CLOSE STEP 3: ");
    for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
    {
        setup_motor.close_step_3[i] = EEPROM.read(EEPROM_CLOSE_STEP_3_MTOR_1 + i);
        if(setup_motor.close_step_3[i] != MOTOR_OPEN && setup_motor.close_step_3[i] != MOTOR_CLOSE && setup_motor.close_step_3[i] != MOTOR_STOP)
        {
            setup_motor.close_step_3[i] = MOTOR_STOP;
        }
        ECHO(setup_motor.close_step_3[i]);
        ECHO(" - ");
    }
    ECHOLN("");
}

void bluetoothInit()
{
    SerialBT.flush();
    SerialBT.end(); 
    if(!SerialBT.begin("Landing Gear")){
        ECHOLN("An error occurred initializing Bluetooth");
    }else{
        ECHOLN("Bluetooth initialized");
    }
  SerialBT.register_callback(callbackBluetooth);
}

void scannerI2cAddress()
{
    // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
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
        // ina219_info.address_ina[ina219_info.count] = i;
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
        // sendDataMinMaxCurrenttoApp();
        // delay(1);
        // sendDataSteptoApp();
        APP_FLAG_SET(SEND_CURRENT_MIN_MAX);
        // sendDatatoAppTicker.start();
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
            if(rootData.success())
            {
				String type = rootData["type"];
        		if (APP_FLAG(MODE_CONFIG))
                {
                    if(type == "run_no_step")
                    {
                        String name = rootData["name"];
                        String data = rootData["command"];
                        for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                        {
                            if(i == name.toInt())
                            {
                                if(select_servo[i]){
                                    xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                                    if(data == "open")
                                    {
                                        stop_led(i);
                                        open_led(i);
                                        run_motor.off_led_servo[i] = true;
                                        pwmController.setChannelPWM(i, pwmServo.pwmForAngle(setup_motor.define_end_angle[i] - 90));
                                    }
                                    if(data == "close")
                                    {
                                        stop_led(i);
                                        close_led(i);
                                        run_motor.off_led_servo[i] = true;
                                        pwmController.setChannelPWM(i, pwmServo.pwmForAngle(setup_motor.define_start_angle[i] - 90));
                                    }
                                    xSemaphoreGive( xMutexI2C );
                                }
                                else{
                                    if(data == "open")
                                    {
                                        set_open_motor(i);
                                    }
                                    else if(data == "stop")
                                    {
                                        set_stop_motor(i);
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
                                    }
                                    else if(data == "close")
                                    {
                                        set_close_motor(i);
                                    }
                                }
                                
                            }
                        }
                    }
                    else if(type == "data")
                    {
                        //Min Current;
                        for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                        {
                            if(rootData["1"][i] != -1)
                            {
                                setup_motor.define_min_current[i] = rootData["1"][i];
                                EEPROM.write(EEPROM_MIN_CURRENT_1 + i,setup_motor.define_min_current[i]);
                            }
                        }
                        //Max Current;
                        for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                        {
                            if(rootData["1"][MAX_NUMBER_MOTOR+i] != -1)
                            {
                                int max = rootData["1"][MAX_NUMBER_MOTOR+i];
                                setup_motor.define_max_current[i] = max;
                                EEPROM.write(EEPROM_MAX_CURRENT_1 + 2*i,setup_motor.define_max_current[i] >> 8);
                                EEPROM.write(EEPROM_MAX_CURRENT_1 + 2*i + 1,setup_motor.define_max_current[i]);
                            }
                        }
                        //select motor;
                        for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                        {
                            select_motor[i] = rootData["1"][2*MAX_NUMBER_MOTOR+i];
                            EEPROM.write(EEPROM_SELECT_MOTOR_1 + i,select_motor[i]);
                        }
                        //select servo;
                        for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                        {
                            select_servo[i] = rootData["1"][3*MAX_NUMBER_MOTOR+i];
                            EEPROM.write(EEPROM_SELECT_SERVO_1 + i,select_servo[i]);
                        }
                        //reverse;
                        for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                        {
                            reverse_motor[i] = rootData["1"][4*MAX_NUMBER_MOTOR+i];
                            EEPROM.write(EEPROM_REVERSE_MOTOR_1 + i,reverse_motor[i]);
                        }
                        //Start Angle;
                        for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                        {
                            if(rootData["1"][5*MAX_NUMBER_MOTOR+i] != -1)
                            {
                                int min = rootData["1"][5*MAX_NUMBER_MOTOR+i];
                                setup_motor.define_start_angle[i] = min;
                                EEPROM.write(EEPROM_START_ANGLE_SERVO_1 + 2*i,setup_motor.define_start_angle[i] >> 8);
                                EEPROM.write(EEPROM_START_ANGLE_SERVO_1 + 2*i + 1,setup_motor.define_start_angle[i]);
                            }
                        }
                        //End Angle;
                        for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                        {
                            if(rootData["1"][5*MAX_NUMBER_MOTOR+i] != -1)
                            {
                                int max = rootData["1"][6*MAX_NUMBER_MOTOR+i];
                                setup_motor.define_end_angle[i] = max;
                                EEPROM.write(EEPROM_END_ANGLE_SERVO_1 + 2*i,setup_motor.define_end_angle[i] >> 8);
                                EEPROM.write(EEPROM_END_ANGLE_SERVO_1 + 2*i + 1,setup_motor.define_end_angle[i]);
                            }
                        }
                        //Time Servo
                        for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                        {
                            if(rootData["1"][7*MAX_NUMBER_MOTOR+i] != -1)
                            {
                                int time = rootData["1"][7*MAX_NUMBER_MOTOR+i];
                                time_run_servo[i] = time;
                                EEPROM.write(EEPROM_TIME_SERVO_1 + 2*i,time_run_servo[i] >> 8);
                                EEPROM.write(EEPROM_TIME_SERVO_1 + 2*i + 1,time_run_servo[i]);
                            }
                        }
                        EEPROM.commit();
                        sendDataMinMaxCurrenttoApp();
                    }
                    else if(type == "step")
                    {
                        //open step 1;
                        for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                        {
                            setup_motor.open_step_1[i] = rootData["1"][i];
                            // setup_motor.open_step_1[i] = data.toInt();
                            // ECHOLN(setup_motor.open_step_1[i]);
                            EEPROM.write(EEPROM_OPEN_STEP_1_MTOR_1 + i,setup_motor.open_step_1[i]);
                        }
                        // EEPROM.commit();
                        //open step 2;
                        for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                        {
                            setup_motor.open_step_2[i] = rootData["1"][MAX_NUMBER_MOTOR+i];
                            // setup_motor.open_step_2[i] = data.toInt();
                            // ECHOLN(setup_motor.open_step_1[i]);
                            EEPROM.write(EEPROM_OPEN_STEP_2_MTOR_1 + i,setup_motor.open_step_2[i]);
                        }
                        // EEPROM.commit();
                        //open step 3;
                        for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                        {
                            String data = rootData["1"][2*MAX_NUMBER_MOTOR+i];
                            setup_motor.open_step_3[i] = data.toInt();
                            EEPROM.write(EEPROM_OPEN_STEP_3_MTOR_1 + i,setup_motor.open_step_3[i]);
                        }
                        // EEPROM.commit();
                        //close step 1;
                        for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                        {
                            String data = rootData["1"][3*MAX_NUMBER_MOTOR+i];
                            setup_motor.close_step_1[i] = data.toInt();
                            EEPROM.write(EEPROM_CLOSE_STEP_1_MTOR_1 + i,setup_motor.close_step_1[i]);
                        }
                        // EEPROM.commit();
                        //close step 2;
                        for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                        {
                            String data = rootData["1"][4*MAX_NUMBER_MOTOR+i];
                            setup_motor.close_step_2[i] = data.toInt();
                            EEPROM.write(EEPROM_CLOSE_STEP_2_MTOR_1 + i,setup_motor.close_step_2[i]);
                        }
                        // EEPROM.commit();
                        //close step 3;
                        for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                        {
                            String data = rootData["1"][5*MAX_NUMBER_MOTOR+i];
                            setup_motor.close_step_3[i] = data.toInt();
                            EEPROM.write(EEPROM_CLOSE_STEP_3_MTOR_1 + i,setup_motor.close_step_3[i]);
                        }
                        EEPROM.commit();
                        sendDataSteptoApp();
                    }
					else if(type == "voltage")
                    {
                        for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                        {
                            set_voltage_motor[i] = rootData["1"][i];
                            // ECHOLN(set_voltage_motor[i]);
                            EEPROM.write(EEPROM_SET_VOLTAGE_MOTOR_1 + i, set_voltage_motor[i]);
                        }
                        EEPROM.commit();
					}
                }
                if(type == "request_data")
                {
                    sendDataMinMaxCurrenttoApp();
                }
                else if(type == "request_step")
                {
                    sendDataSteptoApp();
                }
                else if(type == "request_voltage")
                {
                    // ECHOLN("request_voltage");
                    sendDataVoltagetoApp();
                }
				else if(type == "reset_power")
				{
					// ECHOLN("reset_power");
					setup_motor.total_power = 0;
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
         || int(setup_motor.value_current[MOTOR_1]) > (setup_motor.define_max_current[MOTOR_1]))
//        if(int(setup_motor.value_current[MOTOR_1]) > (setup_motor.define_max_current[MOTOR_1]))
        {   
            count_to_start_check_current[MOTOR_1] = 0;
            ECHOLN("Qua Tai Motor 1");
            set_stop_motor(MOTOR_1);
            stop_motor(MOTOR_1);
            // if(is_done_step())
            // {
            //     run_motor.mode_run_open_step++;
            //     run_motor.mode_run_close_step++;
            //     run_motor.beginChangeStep = true;
            // }
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
         || int(setup_motor.value_current[MOTOR_2]) > (setup_motor.define_max_current[MOTOR_2]))
//        if(int(setup_motor.value_current[MOTOR_2]) > (setup_motor.define_max_current[MOTOR_2]))
        {   
            count_to_start_check_current[MOTOR_2] = 0;
            ECHOLN("Qua Tai Motor 2");
            set_stop_motor(MOTOR_2);
            stop_motor(MOTOR_2);
            // if(is_done_step())
            // {
            //     run_motor.mode_run_open_step++;
            //     run_motor.mode_run_close_step++;
            //     run_motor.beginChangeStep = true;
            // }
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
        || int(setup_motor.value_current[MOTOR_3]) > (setup_motor.define_max_current[MOTOR_3]))
        // if(int(setup_motor.value_current[MOTOR_3]) > (setup_motor.define_max_current[MOTOR_3]))
        {   
            count_to_start_check_current[MOTOR_3] = 0;
            ECHOLN("Qua Tai Motor 3");
            set_stop_motor(MOTOR_3);
            stop_motor(MOTOR_3);
            // if(is_done_step())
            // {
            //     run_motor.mode_run_open_step++;
            //     run_motor.mode_run_close_step++;
            //     run_motor.beginChangeStep = true;
            // }
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
        || int(setup_motor.value_current[MOTOR_4]) > (setup_motor.define_max_current[MOTOR_4]))
        // if(int(setup_motor.value_current[MOTOR_4]) > (setup_motor.define_max_current[MOTOR_4]))
        {   
            count_to_start_check_current[MOTOR_4] = 0;
            ECHOLN("Qua Tai Motor 4");
            set_stop_motor(MOTOR_4);
            stop_motor(MOTOR_4);
            // if(is_done_step())
            // {
            //     run_motor.mode_run_open_step++;
            //     run_motor.mode_run_close_step++;
            //     run_motor.beginChangeStep = true;
            // }
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
        || int(setup_motor.value_current[MOTOR_5]) > (setup_motor.define_max_current[MOTOR_5]))
        // if(int(setup_motor.value_current[MOTOR_5]) > (setup_motor.define_max_current[MOTOR_5]))
        {   
            count_to_start_check_current[MOTOR_5] = 0;
            ECHOLN("Qua Tai Motor 5");
            set_stop_motor(MOTOR_5);
            stop_motor(MOTOR_5);
            // if(is_done_step())
            // {
            //     run_motor.mode_run_open_step++;
            //     run_motor.mode_run_close_step++;
            //     run_motor.beginChangeStep = true;
            // }
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
        || int(setup_motor.value_current[MOTOR_6]) > (setup_motor.define_max_current[MOTOR_6]))
        // if(int(setup_motor.value_current[MOTOR_6]) > (setup_motor.define_max_current[MOTOR_6]))
        {   
            count_to_start_check_current[MOTOR_6] = 0;
            ECHOLN("Qua Tai Motor 6");
            set_stop_motor(MOTOR_6);
            stop_motor(MOTOR_6);
            // if(is_done_step())
            // {
            //     run_motor.mode_run_open_step++;
            //     run_motor.mode_run_close_step++;
            //     run_motor.beginChangeStep = true;
            // }
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
        || int(setup_motor.value_current[MOTOR_7]) > (setup_motor.define_max_current[MOTOR_7]))
        // if(int(setup_motor.value_current[MOTOR_7]) > (setup_motor.define_max_current[MOTOR_7]))
        {   
            count_to_start_check_current[MOTOR_7] = 0;
            ECHOLN("Qua Tai Motor 7");
            set_stop_motor(MOTOR_7);
            stop_motor(MOTOR_7);
            // if(is_done_step())
            // {
            //     run_motor.mode_run_open_step++;
            //     run_motor.mode_run_close_step++;
            //     run_motor.beginChangeStep = true;
            // }
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
        || int(setup_motor.value_current[MOTOR_8]) > (setup_motor.define_max_current[MOTOR_8]))
        // if(int(setup_motor.value_current[MOTOR_8]) > (setup_motor.define_max_current[MOTOR_8]))
        {   
            count_to_start_check_current[MOTOR_8] = 0;
            ECHOLN("Qua Tai Motor 8");
            set_stop_motor(MOTOR_8);
            stop_motor(MOTOR_8);
            // if(is_done_step())
            // {
            //     run_motor.mode_run_open_step++;
            //     run_motor.mode_run_close_step++;
            //     run_motor.beginChangeStep = true;
            // }
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
        || int(setup_motor.value_current[MOTOR_9]) > (setup_motor.define_max_current[MOTOR_9]))
        // if(int(setup_motor.value_current[MOTOR_9]) > (setup_motor.define_max_current[MOTOR_9]))
        {   
            count_to_start_check_current[MOTOR_9] = 0;
            ECHOLN("Qua Tai Motor 9");
            set_stop_motor(MOTOR_9);
            stop_motor(MOTOR_9);
            // if(is_done_step())
            // {
            //     run_motor.mode_run_open_step++;
            //     run_motor.mode_run_close_step++;
            //     run_motor.beginChangeStep = true;
            // }
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
            vTaskDelay(10/portTICK_RATE_MS);
        }
        btn_in_control_motor[MOTOR_1] ++;
        if(btn_in_control_motor[MOTOR_1] == 1)
        {
            if(select_servo[MOTOR_1]){
                xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                pwmController.setChannelPWM(MOTOR_1, pwmServo.pwmForAngle(setup_motor.define_end_angle[MOTOR_1] - 90));
                xSemaphoreGive( xMutexI2C );
            }
            else{
                set_open_motor(MOTOR_1);
            }
        }
        else if(btn_in_control_motor[MOTOR_1] == 2)
        {
            set_stop_motor(MOTOR_1);
            stop_motor(MOTOR_1);
            checkCurrentMotor1.stop();
        }
        else if(btn_in_control_motor[MOTOR_1] == 3)
        {
            if(select_servo[MOTOR_1]){
                xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                pwmController.setChannelPWM(MOTOR_1, pwmServo.pwmForAngle(setup_motor.define_start_angle[MOTOR_1] - 90));
                xSemaphoreGive( xMutexI2C );
            }
            else{
                set_close_motor(MOTOR_1);
            }
        }
        else if(btn_in_control_motor[MOTOR_1] == 4)
        {
            btn_in_control_motor[MOTOR_1] = 0;
            set_stop_motor(MOTOR_1);
            stop_motor(MOTOR_1);
            checkCurrentMotor1.stop();
        }
    }

    if(!digitalRead(BTN_IN_M2))
    {
        while (!digitalRead(BTN_IN_M2))
        {
            vTaskDelay(10/portTICK_RATE_MS);
        }
        btn_in_control_motor[MOTOR_2] ++;
        if(btn_in_control_motor[MOTOR_2] == 1)
        {
            if(select_servo[MOTOR_2]){
                xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                pwmController.setChannelPWM(MOTOR_2, pwmServo.pwmForAngle(setup_motor.define_end_angle[MOTOR_2] - 90));
                xSemaphoreGive( xMutexI2C );
            }
            else{
                set_open_motor(MOTOR_2);
            }
        }
        else if(btn_in_control_motor[MOTOR_2] == 2)
        {
            set_stop_motor(MOTOR_2);
            stop_motor(MOTOR_2);
            checkCurrentMotor2.stop();
        }
        else if(btn_in_control_motor[MOTOR_2] == 3)
        {
            if(select_servo[MOTOR_2]){
                xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                pwmController.setChannelPWM(MOTOR_2, pwmServo.pwmForAngle(setup_motor.define_start_angle[MOTOR_2] - 90));
                xSemaphoreGive( xMutexI2C );
            }
            else{
                set_close_motor(MOTOR_2);
            }
        }
        else if(btn_in_control_motor[MOTOR_2] == 4)
        {
            btn_in_control_motor[MOTOR_2] = 0;
            set_stop_motor(MOTOR_2);
            stop_motor(MOTOR_2);
            checkCurrentMotor2.stop();
        }
    }

    if(!digitalRead(BTN_IN_M3))
    {
        while (!digitalRead(BTN_IN_M3))
        {
            vTaskDelay(10/portTICK_RATE_MS);
        }
        btn_in_control_motor[MOTOR_3] ++;
        if(btn_in_control_motor[MOTOR_3] == 1)
        {
            if(select_servo[MOTOR_3]){
                xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                pwmController.setChannelPWM(MOTOR_3, pwmServo.pwmForAngle(setup_motor.define_end_angle[MOTOR_3] - 90));
                xSemaphoreGive( xMutexI2C );
            }
            else{
                set_open_motor(MOTOR_3);
            }
        }
        else if(btn_in_control_motor[MOTOR_3] == 2)
        {
            set_stop_motor(MOTOR_3);
            stop_motor(MOTOR_3);
            checkCurrentMotor3.stop();
        }
        else if(btn_in_control_motor[MOTOR_3] == 3)
        {
            if(select_servo[MOTOR_3]){
                xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                pwmController.setChannelPWM(MOTOR_3, pwmServo.pwmForAngle(setup_motor.define_start_angle[MOTOR_3] - 90));
                xSemaphoreGive( xMutexI2C );
            }
            else{
                set_close_motor(MOTOR_3);
            }
        }
        else if(btn_in_control_motor[MOTOR_3] == 4)
        {
            btn_in_control_motor[MOTOR_3] = 0;
            set_stop_motor(MOTOR_3);
            stop_motor(MOTOR_3);
            checkCurrentMotor3.stop();
        }
    }

    if(!digitalRead(BTN_IN_M4))
    {
        while (!digitalRead(BTN_IN_M4))
        {
            vTaskDelay(10/portTICK_RATE_MS);
        }
        btn_in_control_motor[MOTOR_4] ++;
        if(btn_in_control_motor[MOTOR_4] == 1)
        {
            if(select_servo[MOTOR_4]){
                xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                pwmController.setChannelPWM(MOTOR_4, pwmServo.pwmForAngle(setup_motor.define_end_angle[MOTOR_4] - 90));
                xSemaphoreGive( xMutexI2C );
            }
            else{
                set_open_motor(MOTOR_4);
            }
        }
        else if(btn_in_control_motor[MOTOR_4] == 2)
        {
            set_stop_motor(MOTOR_4);
            stop_motor(MOTOR_4);
            checkCurrentMotor4.stop();
        }
        else if(btn_in_control_motor[MOTOR_4] == 3)
        {
            if(select_servo[MOTOR_4]){
                xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                pwmController.setChannelPWM(MOTOR_4, pwmServo.pwmForAngle(setup_motor.define_start_angle[MOTOR_4] - 90));
                xSemaphoreGive( xMutexI2C );
            }
            else{
                set_close_motor(MOTOR_4);
            }
        }
        else if(btn_in_control_motor[MOTOR_4] == 4)
        {
            btn_in_control_motor[MOTOR_4] = 0;
            set_stop_motor(MOTOR_4);
            stop_motor(MOTOR_4);
            checkCurrentMotor4.stop();
        }
    }

    if(!digitalRead(BTN_IN_M5))
    {
        while (!digitalRead(BTN_IN_M5))
        {
            vTaskDelay(10/portTICK_RATE_MS);
        }
        btn_in_control_motor[MOTOR_5] ++;
        if(btn_in_control_motor[MOTOR_5] == 1)
        {
            if(select_servo[MOTOR_5]){
                xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                pwmController.setChannelPWM(MOTOR_5, pwmServo.pwmForAngle(setup_motor.define_end_angle[MOTOR_5] - 90));
                xSemaphoreGive( xMutexI2C );
            }
            else{
                set_open_motor(MOTOR_5);
            }
        }
        else if(btn_in_control_motor[MOTOR_5] == 2)
        {
            set_stop_motor(MOTOR_5);
            stop_motor(MOTOR_5);
            checkCurrentMotor5.stop();
        }
        else if(btn_in_control_motor[MOTOR_5] == 3)
        {
            if(select_servo[MOTOR_5]){
                xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                pwmController.setChannelPWM(MOTOR_5, pwmServo.pwmForAngle(setup_motor.define_start_angle[MOTOR_5] - 90));
                xSemaphoreGive( xMutexI2C );
            }
            else{
                set_close_motor(MOTOR_5);
            }
        }
        else if(btn_in_control_motor[MOTOR_5] == 4)
        {
            btn_in_control_motor[MOTOR_5] = 0;
            set_stop_motor(MOTOR_5);
            stop_motor(MOTOR_5);
            checkCurrentMotor5.stop();
        }
    }

    if(!digitalRead(BTN_IN_M6))
    {
        while (!digitalRead(BTN_IN_M6))
        {
            vTaskDelay(10/portTICK_RATE_MS);
        }
        btn_in_control_motor[MOTOR_6] ++;
        if(btn_in_control_motor[MOTOR_6] == 1)
        {
            if(select_servo[MOTOR_6]){
                xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                pwmController.setChannelPWM(MOTOR_6, pwmServo.pwmForAngle(setup_motor.define_end_angle[MOTOR_6] - 90));
                xSemaphoreGive( xMutexI2C );
            }
            else{
                set_open_motor(MOTOR_6);
            }
        }
        else if(btn_in_control_motor[MOTOR_6] == 2)
        {
            set_stop_motor(MOTOR_6);
            stop_motor(MOTOR_6);
            checkCurrentMotor6.stop();
        }
        else if(btn_in_control_motor[MOTOR_6] == 3)
        {
            if(select_servo[MOTOR_6]){
                xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                pwmController.setChannelPWM(MOTOR_6, pwmServo.pwmForAngle(setup_motor.define_start_angle[MOTOR_6] - 90));
                xSemaphoreGive( xMutexI2C );
            }
            else{
                set_close_motor(MOTOR_6);
            }
        }
        else if(btn_in_control_motor[MOTOR_6] == 4)
        {
            btn_in_control_motor[MOTOR_6] = 0;
            set_stop_motor(MOTOR_6);
            stop_motor(MOTOR_6);
            checkCurrentMotor6.stop();
        }
    }
    if(!digitalRead(BTN_IN_M7))
    {
        while (!digitalRead(BTN_IN_M7))
        {
            vTaskDelay(10/portTICK_RATE_MS);
        }
        btn_in_control_motor[MOTOR_7] ++;
        if(btn_in_control_motor[MOTOR_7] == 1)
        {
            if(select_servo[MOTOR_7]){
                xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                pwmController.setChannelPWM(MOTOR_7, pwmServo.pwmForAngle(setup_motor.define_end_angle[MOTOR_7] - 90));
                xSemaphoreGive( xMutexI2C );
            }
            else{
                set_open_motor(MOTOR_7);
            }
        }
        else if(btn_in_control_motor[MOTOR_7] == 2)
        {
            set_stop_motor(MOTOR_7);
            stop_motor(MOTOR_7);
            checkCurrentMotor7.stop();
        }
        else if(btn_in_control_motor[MOTOR_7] == 3)
        {
            if(select_servo[MOTOR_7]){
                xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                pwmController.setChannelPWM(MOTOR_7, pwmServo.pwmForAngle(setup_motor.define_start_angle[MOTOR_7] - 90));
                xSemaphoreGive( xMutexI2C );
            }
            else{
                set_close_motor(MOTOR_7);
            }
        }
        else if(btn_in_control_motor[MOTOR_7] == 4)
        {
            btn_in_control_motor[MOTOR_7] = 0;
            set_stop_motor(MOTOR_7);
            stop_motor(MOTOR_7);
            checkCurrentMotor7.stop();
        }
    }
    if(!digitalRead(BTN_IN_M8))
    {
        while (!digitalRead(BTN_IN_M8))
        {
            vTaskDelay(10/portTICK_RATE_MS);
        }
        btn_in_control_motor[MOTOR_8] ++;
        if(btn_in_control_motor[MOTOR_8] == 1)
        {
            if(select_servo[MOTOR_8]){
                xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                pwmController.setChannelPWM(MOTOR_8, pwmServo.pwmForAngle(setup_motor.define_end_angle[MOTOR_8] - 90));
                xSemaphoreGive( xMutexI2C );
            }
            else{
                set_open_motor(MOTOR_8);
            }
        }
        else if(btn_in_control_motor[MOTOR_8] == 2)
        {
            set_stop_motor(MOTOR_8);
            stop_motor(MOTOR_8);
            checkCurrentMotor8.stop();
        }
        else if(btn_in_control_motor[MOTOR_8] == 3)
        {
            if(select_servo[MOTOR_8]){
                xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                pwmController.setChannelPWM(MOTOR_8, pwmServo.pwmForAngle(setup_motor.define_start_angle[MOTOR_8] - 90));
                xSemaphoreGive( xMutexI2C );
            }
            else{
                set_close_motor(MOTOR_8);
            }
        }
        else if(btn_in_control_motor[MOTOR_8] == 4)
        {
            btn_in_control_motor[MOTOR_8] = 0;
            set_stop_motor(MOTOR_8);
            stop_motor(MOTOR_8);
            checkCurrentMotor8.stop();
        }
    }
    if(!digitalRead(BTN_IN_M9))
    {
        while (!digitalRead(BTN_IN_M9))
        {
            vTaskDelay(10/portTICK_RATE_MS);
        }
        btn_in_control_motor[MOTOR_9] ++;
        if(btn_in_control_motor[MOTOR_9] == 1)
        {
            if(select_servo[MOTOR_9]){
                xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                pwmController.setChannelPWM(MOTOR_9, pwmServo.pwmForAngle(setup_motor.define_end_angle[MOTOR_9] - 90));
                xSemaphoreGive( xMutexI2C );
            }
            else{
                set_open_motor(MOTOR_9);
            }
        }
        else if(btn_in_control_motor[MOTOR_9] == 2)
        {
            set_stop_motor(MOTOR_9);
            stop_motor(MOTOR_9);
            checkCurrentMotor9.stop();
        }
        else if(btn_in_control_motor[MOTOR_9] == 3)
        {
            if(select_servo[MOTOR_9]){
                xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                pwmController.setChannelPWM(MOTOR_9, pwmServo.pwmForAngle(setup_motor.define_start_angle[MOTOR_9] - 90));
                xSemaphoreGive( xMutexI2C );
            }
            else{
                set_close_motor(MOTOR_9);
            }
        }
        else if(btn_in_control_motor[MOTOR_9] == 4)
        {
            btn_in_control_motor[MOTOR_9] = 0;
            set_stop_motor(MOTOR_9);
            stop_motor(MOTOR_9);
            checkCurrentMotor9.stop();
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

void offLedServo(){
    static bool start_check_time[MAX_NUMBER_MOTOR] = {false,false,false,false,false,false,false,false,false};
    static uint32_t check_time[MAX_NUMBER_MOTOR];
    for(int i = MOTOR_1; i < MAX_NUMBER_MOTOR; i++){
        if(run_motor.off_led_servo[i]){
            run_motor.off_led_servo[i] = false;
            start_check_time[i] = true;
            check_time[i] = millis();
        }

        if(start_check_time[i] && millis() > (check_time[i] + time_run_servo[i])){
            start_check_time[i] = false;
            stop_led(i);
        }
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
    TickerControlMotor.update();
}

void checkButtonConfigModeRun()
{
    // static bool check_mode_setup = true;
    static unsigned long time_check_button = 0;

    //hold to config mode
    if(digitalRead(BTN_MODE_SETUP)){
        time_check_button = millis();
    }
    if(!digitalRead(BTN_MODE_SETUP) && (time_check_button + CONFIG_HOLD_TIME) <= millis()){
        time_check_button = millis();
        // blinkMotorOnStart.stop();
        for(int i = MOTOR_1; i < MAX_NUMBER_MOTOR; i++)
        {
            stop_led(i);
        }
        
        if(APP_FLAG(MODE_CONFIG))
        {
            ECHOLN("MODE_WAIT_RUNNING (AUTO)");
            APP_FLAG_CLEAR(MODE_CONFIG);
            APP_FLAG_SET(MODE_WAIT_RUNNING);
            set_led_B(ON_LED);
            set_led_G(OFF_LED);
            set_led_R(OFF_LED);
        }
        else
        {
            ECHOLN("MODE_CONFIG");
            APP_FLAG_CLEAR(MODE_WAIT_RUNNING);
            APP_FLAG_SET(MODE_CONFIG);
            set_led_R(ON_LED);
            set_led_B(OFF_LED);
            set_led_G(OFF_LED);
        }

    }
}


void checkPwmRxControlLed()
{
    static uint32_t time_check = 0;
    if(millis() - time_check >= 100)
    {
        time_check = millis();
        run_motor.pwm_value_led1 = 0;
        run_motor.pwm_value_led2 = 0;
        for(int i = 0; i < 5; i++){
            run_motor.pwm_value_led1 += pulseIn(BTN_IN_LED_1, HIGH, TIME_OUT_PULSEIN);
            run_motor.pwm_value_led2 += pulseIn(BTN_IN_LED_2, HIGH, TIME_OUT_PULSEIN);
        }
        run_motor.pwm_value_led1 = run_motor.pwm_value_led1/5;
        run_motor.pwm_value_led2 = run_motor.pwm_value_led2/5;
        // ECHO(run_motor.pwm_value_led1);
        // ECHO(" - ");
        // ECHOLN(run_motor.pwm_value_led2);
        
        if(run_motor.pwm_value_led1 > 1700 && run_motor.pwm_value_led1 < MAX_VALUE_READ_RX)
        {
            on_led_mosfet(LED_MOSFET_1);
        }
        else if(run_motor.pwm_value_led1 < 1300 && run_motor.pwm_value_led1 > MIN_VALUE_READ_RX)
        {
            off_led_mosfet(LED_MOSFET_1);
        }
        if(run_motor.pwm_value_led2 > 1700  && run_motor.pwm_value_led2 < MAX_VALUE_READ_RX)
        {
            on_led_mosfet(LED_MOSFET_2);
        }
        else if(run_motor.pwm_value_led2 < 1300  && run_motor.pwm_value_led2 > MIN_VALUE_READ_RX)
        {
            off_led_mosfet(LED_MOSFET_2);
        }
    }
    
}


void checkPwmRxControlRun()
{
    static uint32_t time_check = 0;
    if(millis() - time_check >= 100)
    {
        time_check = millis();
        run_motor.pwm_value_mode_run = 0;
        for(int i = 0; i < 5; i++){
            run_motor.pwm_value_mode_run += pulseIn(BTN_MODE_RUN, HIGH, TIME_OUT_PULSEIN);
        }
        run_motor.pwm_value_mode_run = run_motor.pwm_value_mode_run/5;
        ECHOLN(run_motor.pwm_value_mode_run);
        // < 1500: CLOSE
        // > 1500: OPEN
        
        if(run_motor.pwm_value_mode_run > 1650 && !run_motor.is_rx_position_open)
        {
            run_motor.is_rx_position_open = true;
            run_motor.start_run_step_open = true; 
        }
        else if(run_motor.pwm_value_mode_run < 1450 && run_motor.is_rx_position_open)
        {
            run_motor.is_rx_position_open = false;
            run_motor.start_run_step_close = true;
        }

    }
}

void CheckMotorInit()
{
    for(int i = MOTOR_1; i < MAX_NUMBER_MOTOR; i++)
    {
		if(select_motor[i]){
			open_motor(i);
		}
    }
    for(int i = MOTOR_1; i < MAX_NUMBER_MOTOR; i++)
    {
		if(select_servo[i]){
			open_led(i);
            // close_led(i);
		}
    }
    delay(20);

    for(int i = MOTOR_1; i < MAX_NUMBER_MOTOR; i++)
    {
        setup_motor.value_current[i] = abs(ina219[i].getCurrent_mA());
        ECHO("motor ");
        ECHO(i);
        ECHO(": ");
        ECHOLN(setup_motor.value_current[i]);
        
		if(select_motor[i]){
            set_stop_motor(i);
            stop_motor(i);
			if(setup_motor.value_current[i] > MIN_CURRENT_MOTOR_CHECK_START)
			{
				setup_motor.isMotorOn[i] = true;
				open_led(i);
			}
			else{
				close_led(i);
			}
		}
    }
    // blinkMotorOnStart.start();
    
}


void ReadIna219Data(void *pvParameters){
	for( ;; )
	{
        // Serial.println("lowPriorityTask gains key");
        xSemaphoreTake( xMutexI2C, portMAX_DELAY );
        /* even low priority task delay high priority still in Block state */
        readValueIna219();
        xSemaphoreGive( xMutexI2C );
		vTaskDelay(1/portTICK_PERIOD_MS);
	}
	
}

void ReadPulseIn(void *pvParameters){
	for( ;; )
	{
		checkPwmRxControlLed();
        if(!APP_FLAG(MODE_CONFIG)){
            if(!run_motor.is_get_position_rx_begin){
                run_motor.pwm_value_mode_run = 0;
                for(int i = 0; i < 5; i++){
                    run_motor.pwm_value_mode_run += pulseIn(BTN_MODE_RUN, HIGH, TIME_OUT_PULSEIN);
                }
                run_motor.pwm_value_mode_run = run_motor.pwm_value_mode_run/5;
                ECHOLN(run_motor.pwm_value_mode_run);
                if(run_motor.pwm_value_mode_run != 0){
                    if(run_motor.pwm_value_mode_run > 1500)
                    {
                        run_motor.is_rx_position_open = true; 
                    }
                    else
                    {
                        run_motor.is_rx_position_open = false; 
                    }
                    run_motor.is_get_position_rx_begin = true;
                }
            }
            else{
                checkPwmRxControlRun();
            }
        }
		vTaskDelay(100/portTICK_RATE_MS);
	}
	
}

void SetStepRunning(void *pvParameters){
    
    for(;;){
        if(!APP_FLAG(MODE_CONFIG))
        {
            if(run_motor.start_run_step_open)
            {
                ECHOLN("START MODE RUN OPEN STEP 1");
                APP_FLAG_SET(MODE_RUNNING);
                set_led_B(false);
                set_led_G(true);
                uint16_t time_delay = 0;
                for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                {
                    switch (setup_motor.open_step_1[i])
                    {
                    case MOTOR_STOP:
                        set_stop_motor(i);
                        stop_motor(i);
                        break;
                    case MOTOR_OPEN:
                        if(select_motor[i]){
                            set_open_motor(i);
                        }
                        if(select_servo[i]){
                            xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                            pwmController.setChannelPWM(i, pwmServo.pwmForAngle(setup_motor.define_end_angle[i] - 90));
                            xSemaphoreGive( xMutexI2C );
                            if(time_run_servo[i] > time_delay){
                                time_delay = time_run_servo[i];
                            }
                        }
                        
                        break;
                    case MOTOR_CLOSE:
                        if(select_motor[i]){
                            set_close_motor(i);
                        }
                        if(select_servo[i]){
                            xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                            pwmController.setChannelPWM(i, pwmServo.pwmForAngle(setup_motor.define_start_angle[i] - 90));
                            xSemaphoreGive( xMutexI2C );
                            if(time_run_servo[i] > time_delay){
                                time_delay = time_run_servo[i];
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
                vTaskDelay(time_delay/portTICK_RATE_MS);
                while (!is_done_step())
                {
                    vTaskDelay(1/portTICK_RATE_MS);
                }
                

                time_delay = 0;
                ECHOLN("START MODE RUN OPEN STEP 2");
                for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                {
                    switch (setup_motor.open_step_2[i])
                    {
                    case MOTOR_STOP:
                        set_stop_motor(i);
                        stop_motor(i);
                        break;
                    case MOTOR_OPEN:
                        if(select_motor[i]){
                            set_open_motor(i);
                        }
                        if(select_servo[i]){
                            xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                            pwmController.setChannelPWM(i, pwmServo.pwmForAngle(setup_motor.define_end_angle[i] - 90));
                            xSemaphoreGive( xMutexI2C );
                            if(time_run_servo[i] > time_delay){
                                time_delay = time_run_servo[i];
                            }
                        }
                        break;
                    case MOTOR_CLOSE:
                        if(select_motor[i]){
                            set_close_motor(i);
                        }
                        if(select_servo[i]){
                            xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                            pwmController.setChannelPWM(i, pwmServo.pwmForAngle(setup_motor.define_start_angle[i] - 90));
                            xSemaphoreGive( xMutexI2C );
                            if(time_run_servo[i] > time_delay){
                                time_delay = time_run_servo[i];
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
                vTaskDelay(time_delay/portTICK_RATE_MS);
                while (!is_done_step())
                {
                    vTaskDelay(1/portTICK_RATE_MS);
                }

                time_delay = 0;
                ECHOLN("START MODE RUN OPEN STEP 3");
                for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                {
                    switch (setup_motor.open_step_3[i])
                    {
                    case MOTOR_STOP:
                        set_stop_motor(i);
                        stop_motor(i);
                        break;
                    case MOTOR_OPEN:
                        if(select_motor[i]){
                            set_open_motor(i);
                        }
                        if(select_servo[i]){
                            xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                            pwmController.setChannelPWM(i, pwmServo.pwmForAngle(setup_motor.define_end_angle[i] - 90));
                            xSemaphoreGive( xMutexI2C );
                            if(time_run_servo[i] > time_delay){
                                time_delay = time_run_servo[i];
                            }
                        }
                        break;
                    case MOTOR_CLOSE:
                        if(select_motor[i]){
                            set_close_motor(i);
                        }
                        if(select_servo[i]){
                            xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                            pwmController.setChannelPWM(i, pwmServo.pwmForAngle(setup_motor.define_start_angle[i] - 90));
                            xSemaphoreGive( xMutexI2C );
                            if(time_run_servo[i] > time_delay){
                                time_delay = time_run_servo[i];
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
                vTaskDelay(time_delay/portTICK_RATE_MS);
                while (!is_done_step())
                {
                    vTaskDelay(1/portTICK_RATE_MS);
                }
                ECHOLN("DONE RUN OPEN MODE");
                APP_FLAG_CLEAR(MODE_RUNNING);
                APP_FLAG_SET(MODE_WAIT_RUNNING);
                set_led_G(false);
                set_led_B(true);



                // run_motor.mode_run_close_step = CLOSE_STEP_1;
                // switch (run_motor.mode_run_open_step)
                // {
                // case OPEN_STEP_1:
                //     if(run_motor.beginChangeStep)
                //     {
                //         ECHOLN("START MODE RUN OPEN STEP 1");
                //         APP_FLAG_SET(MODE_RUNNING);
                //         set_led_B(false);
                //         set_led_G(true);
                //         run_motor.beginChangeStep = false;
                //         for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                //         {
                //             switch (setup_motor.open_step_1[i])
                //             {
                //             case MOTOR_STOP:
                //                 set_stop_motor(i);
                //                 stop_motor(i);
                //                 break;
                //             case MOTOR_OPEN:
                //                 set_open_motor(i);
                //                 break;
                //             case MOTOR_CLOSE:
                //                 set_close_motor(i);
                //                 break;
                //             default:
                //                 break;
                //             }
                //         }
                //     }
                //     break;
                // case OPEN_STEP_2:
                //     if(run_motor.beginChangeStep)
                //     {
                //         ECHOLN("START MODE RUN OPEN STEP 2");
                //         run_motor.beginChangeStep = false;
                //         for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                //         {
                //             switch (setup_motor.open_step_2[i])
                //             {
                //             case MOTOR_STOP:
                //                 set_stop_motor(i);
                //                 stop_motor(i);
                //                 break;
                //             case MOTOR_OPEN:
                //                 set_open_motor(i);
                //                 break;
                //             case MOTOR_CLOSE:
                //                 set_close_motor(i);
                //                 break;
                //             default:
                //                 break;
                //             }
                //         }
                //     }
                //     break;
                // case OPEN_STEP_3:
                //     if(run_motor.beginChangeStep)
                //     {
                //         ECHOLN("START MODE RUN OPEN STEP 3");
                //         run_motor.beginChangeStep = false;
                //         for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                //         {
                //             switch (setup_motor.open_step_3[i])
                //             {
                //             case MOTOR_STOP:
                //                 set_stop_motor(i);
                //                 stop_motor(i);
                //                 break;
                //             case MOTOR_OPEN:
                //                 set_open_motor(i);
                //                 break;
                //             case MOTOR_CLOSE:
                //                 set_close_motor(i);
                //                 break;
                //             default:
                //                 break;
                //             }
                //         }
                //     }
                //     break;
                // case DONE_MODE_OPEN:
                //     ECHOLN("DONE RUN OPEN MODE");
                //     APP_FLAG_CLEAR(MODE_RUNNING);
                //     APP_FLAG_SET(MODE_WAIT_RUNNING);
                //     set_led_G(false);
                //     set_led_B(true);
                //     run_motor.mode_run_open_step++;
                //     run_motor.start_run_step_open = false;
                //     break;
                // default:
                //     break;
                // }
            }
            //-----------------------------------------------
            else if(run_motor.start_run_step_close)
            {
                ECHOLN("START MODE RUN OPEN STEP 1");
                APP_FLAG_SET(MODE_RUNNING);
                set_led_B(false);
                set_led_G(true);
                uint16_t time_delay = 0;
                for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                {
                    switch (setup_motor.close_step_1[i])
                    {
                    case MOTOR_STOP:
                        set_stop_motor(i);
                        stop_motor(i);
                        break;
                    case MOTOR_OPEN:
                        if(select_motor[i]){
                            set_open_motor(i);
                        }
                        if(select_servo[i]){
                            xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                            pwmController.setChannelPWM(i, pwmServo.pwmForAngle(setup_motor.define_end_angle[i] - 90));
                            xSemaphoreGive( xMutexI2C );
                            if(time_run_servo[i] > time_delay){
                                time_delay = time_run_servo[i];
                            }
                        }
                        
                        break;
                    case MOTOR_CLOSE:
                        if(select_motor[i]){
                            set_close_motor(i);
                        }
                        if(select_servo[i]){
                            xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                            pwmController.setChannelPWM(i, pwmServo.pwmForAngle(setup_motor.define_start_angle[i] - 90));
                            xSemaphoreGive( xMutexI2C );
                            if(time_run_servo[i] > time_delay){
                                time_delay = time_run_servo[i];
                            }
                        }
                        
                        break;
                    default:
                        break;
                    }
                }
                vTaskDelay(time_delay/portTICK_RATE_MS);
                while (!is_done_step())
                {
                    vTaskDelay(1/portTICK_RATE_MS);
                }

                time_delay = 0;
                ECHOLN("START MODE RUN OPEN STEP 2");
                for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                {
                    switch (setup_motor.close_step_2[i])
                    {
                    case MOTOR_STOP:
                        set_stop_motor(i);
                        stop_motor(i);
                        break;
                    case MOTOR_OPEN:
                        if(select_motor[i]){
                            set_open_motor(i);
                        }
                        if(select_servo[i]){
                            xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                            pwmController.setChannelPWM(i, pwmServo.pwmForAngle(setup_motor.define_end_angle[i] - 90));
                            xSemaphoreGive( xMutexI2C );
                            if(time_run_servo[i] > time_delay){
                                time_delay = time_run_servo[i];
                            }
                        }
                        break;
                    case MOTOR_CLOSE:
                        if(select_motor[i]){
                            set_close_motor(i);
                        }
                        if(select_servo[i]){
                            xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                            pwmController.setChannelPWM(i, pwmServo.pwmForAngle(setup_motor.define_start_angle[i] - 90));
                            xSemaphoreGive( xMutexI2C );
                            if(time_run_servo[i] > time_delay){
                                time_delay = time_run_servo[i];
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
                vTaskDelay(time_delay/portTICK_RATE_MS);
                while (!is_done_step())
                {
                    vTaskDelay(1/portTICK_RATE_MS);
                }

                time_delay = 0;
                ECHOLN("START MODE RUN OPEN STEP 3");
                for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
                {
                    switch (setup_motor.close_step_3[i])
                    {
                    case MOTOR_STOP:
                        set_stop_motor(i);
                        stop_motor(i);
                        break;
                    case MOTOR_OPEN:
                        if(select_motor[i]){
                            set_open_motor(i);
                        }
                        if(select_servo[i]){
                            xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                            pwmController.setChannelPWM(i, pwmServo.pwmForAngle(setup_motor.define_end_angle[i] - 90));
                            xSemaphoreGive( xMutexI2C );
                            if(time_run_servo[i] > time_delay){
                                time_delay = time_run_servo[i];
                            }
                        }
                        break;
                    case MOTOR_CLOSE:
                        if(select_motor[i]){
                            set_close_motor(i);
                        }
                        if(select_servo[i]){
                            xSemaphoreTake( xMutexI2C, portMAX_DELAY );
                            pwmController.setChannelPWM(i, pwmServo.pwmForAngle(setup_motor.define_start_angle[i] - 90));
                            xSemaphoreGive( xMutexI2C );
                            if(time_run_servo[i] > time_delay){
                                time_delay = time_run_servo[i];
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
                vTaskDelay(time_delay/portTICK_RATE_MS);
                while (!is_done_step())
                {
                    vTaskDelay(1/portTICK_RATE_MS);
                }
                ECHOLN("DONE RUN OPEN MODE");
                APP_FLAG_CLEAR(MODE_RUNNING);
                APP_FLAG_SET(MODE_WAIT_RUNNING);
                set_led_G(false);
                set_led_B(true);






            //     run_motor.mode_run_open_step = OPEN_STEP_1;
            //     switch (run_motor.mode_run_close_step)
            //     {
            //     case CLOSE_STEP_1:
            //         if(run_motor.beginChangeStep)
            //         {
            //             APP_FLAG_SET(MODE_RUNNING);
            //             ECHOLN("START MODE RUN CLOSE STEP 1");
            //             set_led_B(false);
            //             set_led_G(true);
            //             run_motor.beginChangeStep = false;
            //             for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
            //             {
            //                 switch (setup_motor.close_step_1[i])
            //                 {
            //                 case MOTOR_STOP:
            //                     set_stop_motor(i);
            //                     stop_motor(i);
            //                     break;
            //                 case MOTOR_OPEN:
            //                     set_open_motor(i);
            //                     break;
            //                 case MOTOR_CLOSE:
            //                     set_close_motor(i);
            //                     break;
            //                 default:
            //                     break;
            //                 }
            //             }
            //         }
            //         break;
            //     case CLOSE_STEP_2:
            //         if(run_motor.beginChangeStep)
            //         {
            //             ECHOLN("START MODE RUN CLOSE STEP 2");
            //             run_motor.beginChangeStep = false;
            //             for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
            //             {
            //                 switch (setup_motor.close_step_2[i])
            //                 {
            //                 case MOTOR_STOP:
            //                     set_stop_motor(i);
            //                     stop_motor(i);
            //                     break;
            //                 case MOTOR_OPEN:
            //                     set_open_motor(i);
            //                     break;
            //                 case MOTOR_CLOSE:
            //                     set_close_motor(i);
            //                     break;
            //                 default:
            //                     break;
            //                 }
            //             }
            //         }
            //         break;
            //     case CLOSE_STEP_3:
            //         if(run_motor.beginChangeStep)
            //         {
            //             ECHOLN("START MODE RUN CLOSE STEP 3");
            //             run_motor.beginChangeStep = false;
            //             for(int i = 0; i < MAX_NUMBER_MOTOR; i++)
            //             {
            //                 switch (setup_motor.close_step_3[i])
            //                 {
            //                 case MOTOR_STOP:
            //                     set_stop_motor(i);
            //                     stop_motor(i);
            //                     break;
            //                 case MOTOR_OPEN:
            //                     set_open_motor(i);
            //                     break;
            //                 case MOTOR_CLOSE:
            //                     set_close_motor(i);
            //                     break;
            //                 default:
            //                     break;
            //                 }
            //             }
            //         }
            //         break;
            //     case DONE_MODE_CLOSE:
            //         ECHOLN("DONE RUN CLOSE MODE");
            //         APP_FLAG_CLEAR(MODE_RUNNING);
            //         APP_FLAG_SET(MODE_WAIT_RUNNING);
            //         set_led_G(false);
            //         set_led_B(true);
            //         run_motor.mode_run_close_step++;
            //         run_motor.start_run_step_close = false;
            //         break;
            //     default:
            //         break;
            //     }
            }
        }
        vTaskDelay(100/portTICK_RATE_MS);
    }
}

void testControlMotor() {
	static long counterUS = 0;  
	counterUS ++;
	if (counterUS == 100) {
		counterUS = 0;
	}
    for(int i = 0; i < MAX_NUMBER_MOTOR; i++){
        if(statusCurrentMotor[i] == MOTOR_OPEN || statusCurrentMotor[i] == MOTOR_CLOSE){
            if(statusCurrentMotor[i] == MOTOR_OPEN){
                if(counterUS == 0){
                    open_motor(i);
                }
            }
            else if(statusCurrentMotor[i] == MOTOR_CLOSE){
                if(counterUS == 0){
                    close_motor(i);
                }
            }
            switch (set_voltage_motor[i])
            {
            case PWM_MOTOR_11V:
                if(counterUS == PWM_VOLATGE_MOTOR_11V){
                    stop_motor(i);
                }
                break;
            case PWM_MOTOR_10V:
                if(counterUS == PWM_VOLATGE_MOTOR_10V){
                    stop_motor(i);
                }
                break;
            case PWM_MOTOR_9V:
                if(counterUS == PWM_VOLATGE_MOTOR_9V){
                    stop_motor(i);
                }
                break;
            case PWM_MOTOR_8V:
                if(counterUS == PWM_VOLATGE_MOTOR_8V){
                    stop_motor(i);
                }
                break;
            case PWM_MOTOR_7V:
                if(counterUS == PWM_VOLATGE_MOTOR_7V){
                    stop_motor(i);
                }
                break;
            case PWM_MOTOR_6V:
                if(counterUS == PWM_VOLATGE_MOTOR_6V){
                    stop_motor(i);
                }
                break;
            default:
                break;
            }
        }
        else if (statusCurrentMotor[i] == MOTOR_STOP)
        {
            /* code */
        }
        
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
    bluetoothInit();
    CheckMotorInit();

    set_led_B(ON_LED);
    APP_FLAG_SET(MODE_WAIT_RUNNING);
    xTaskCreatePinnedToCore(
		ReadIna219Data,    /* Function to implement the task */
		"ReadIna219Data",  /* Name of the task */
		4096,             /* Stack size in words */
		NULL,             /* Task input parameter */
		0,                /* Priority of the task */
		NULL,             /* Task handle. */
		0);               /* Core where the task should run */
    xTaskCreatePinnedToCore(
		ReadPulseIn,    /* Function to implement the task */
		"ReadPulseIn",  /* Name of the task */
		4096,             /* Stack size in words */
		NULL,             /* Task input parameter */
		0,                /* Priority of the task */
		NULL,             /* Task handle. */
		0);               /* Core where the task should run */
    xTaskCreatePinnedToCore(
		SetStepRunning,    /* Function to implement the task */
		"SetStepRunning",  /* Name of the task */
		4096,             /* Stack size in words */
		NULL,             /* Task input parameter */
		0,                /* Priority of the task */
		NULL,             /* Task handle. */
		0);               /* Core where the task should run */
    
    TickerControlMotor.start();
}


void loop()
{
    tickerUpdate();
    checkStartCalCurrent();
    offLedServo();
    checkButtonConfigModeRun();
    // checkPwmRxControlLed();

    if(APP_FLAG(SEND_CURRENT_MIN_MAX))
    {
		APP_FLAG_CLEAR(SEND_CURRENT_MIN_MAX);
		APP_FLAG_SET(SEND_DATA_VOLTAGE);
		APP_FLAG_SET(SEND_DATA_STEP);
        sendDataMinMaxCurrenttoApp();
        sendDatatoAppTicker.start();
        run_motor.time_delay_send_step_after_send_current = millis();
    }
    if(millis() == (run_motor.time_delay_send_step_after_send_current + 500))
    {
        sendDataMinMaxCurrenttoApp();
    }
	if(APP_FLAG(SEND_DATA_VOLTAGE) && millis() >= (run_motor.time_delay_send_step_after_send_current + 900))
    {
		APP_FLAG_CLEAR(SEND_DATA_VOLTAGE);
        sendDataVoltagetoApp();
    }
    if(APP_FLAG(SEND_DATA_STEP) && millis() >= (run_motor.time_delay_send_step_after_send_current + 1300))
    {
		APP_FLAG_CLEAR(SEND_DATA_STEP);
        sendDataSteptoApp();
    }


    if(APP_FLAG(MODE_CONFIG)){
        checkButtonControl();
    }
    
}
