/**
 *  \file main.cpp
 *  \brief Main file for Open Assistive Arm For Meals
 *  \version 0.1.01
 *  \date 02/03/2021
 *  \author Georges Tawil
*/

#include <Arduino.h>

/*
******************************************************************************
                Import files
******************************************************************************
*/
#include "Config.h"
#include <Dynamixel2Arduino.h>
#include "Meal.h"
#include "Led.h"

/**
 * -------------------------------
 *      Define Virtual Object
 * -------------------------------
 * Physical object
 *      Led indicator
 *      arm switch
 *      user switch 1
 *      user switch 2
 *      user joystick
 *      Dynamixel serial communication
 * Virtual object 
 *      Arm
 *      Plate
 *      Meal
 */

/**
 * @brief Create Led object
 * this is the main visual indicator for the user, it shows the state of the device
 * 
 * @return Led 
 */
Led _led(BLUE_LED_PIN, GREEN_LED_PIN, RED_LED_PIN);

/**
 * @brief Create arm button
 * this is the switch on the arm used for saving mouth's position, calibration sequence
 * 
 * @return Button 
 */
Button _armSwitch(ARM_SWITCH_PIN, &_led);

/**
 * @brief Create an user button 1
 * this is first input of the device connected to 3.5mm jack
 * 
 * @return Button 
 */
Button _userSwitch1(USER_SWITCH1_PIN, &_led);

/**
 * @brief Create an user button 2
 * this is second input of the device connected to 3.5mm jack
 * 
 * @return userSwitch1 
 */
Button _userSwitch2(USER_SWITCH2_PIN, &_led);

/**
 * @brief Create an joystick (analogic input)
 * this is the third input of the device to connect special command with 9 sub D, this is for analogic input
 * 
 * @return userSwitch2
 */
Joystick _userJoystick(JOYSTICK_SPEED_PIN, JOYSTICK_DIRECTION_PIN, JOYSTICK_REF_PIN, &_led);

/**
 * @brief Create the serial communication with actuator
 * this is etablish to communicate with actuator with 57600 baudrates serial communication
 * 
 * @return userJoystick
 */
Dynamixel2Arduino _dxlSerial(DXL_SERIAL, DXL_DIR_PIN);

/**
 * @brief Create All joints
 * 
 */
Joint joint1 = Joint(M_XC430_W240_T, &_dxlSerial);
Joint joint2 = Joint(M_XM540_W270_T, &_dxlSerial);
DependantJoint joint3 = DependantJoint(M_XM430_W350_T, &_dxlSerial, &joint2);
Joint joint4 = Joint(M_2XL430_W250_T_A, &_dxlSerial);
Joint joint5 = Joint(M_2XL430_W250_T_B, &_dxlSerial);
Joint joint6 = Joint(M_XL_320, &_dxlSerial);
/**
 * @brief Create the arm
 * this is virtual object arm include arm switch and actuators (Dynamixel serial communication)
 * 
 * @return Arm 
 */
Arm _arm(&joint1, &joint2, &joint3, &joint4, &joint5, &joint6, &_armSwitch);

/**
 * @brief Create a plate
 * this is the virtual object that define the plate with all is picking position
 * 
 * @return Button 
 */
Plate _currentPlate(OFFSET, NUMBER_OF_POSITION, NUMBER_OF_LINE, NUMBER_OF_COLUMN, PICKING_POSITION_LIST, REMOVE_EXCESS_POSITION, FINISHED_TRAJECTORY_POSITION);

/**
 * @brief Create an meal
 * this is the virtual object which store all parameters for using
 * 
 * @return Button 
 */
Meal _currentMeal(&_arm, &_armSwitch, &_userSwitch1, &_userSwitch2, &_userJoystick, &_currentPlate);

/**
 * -----------------------------------
 *      Global Variable
 * -----------------------------------
 *      Mode
 *      Security
 * 
 */

/**
 * Global variable for the device to define using mode
 *      home : 
 *      meal : when the user is having his lunch
 *      calibration : when the is in calibration mode for all joint, needed when the timing belt jump after an collision or first time starting the device
 *      test : this is a mode for technical supplies
 *      poweroff : after using all motor is turning off and the device is ready to be out of electrical power supply
 * 
 */
enum Mode
{
    home,
    meal,
    calibration,
    test,
    powerOff,
    nothing
};
Mode _mode;
Mode modeSelector;
/**
 * Variable to be sure about the security
 * 
 */

bool _homeFlag=false;
bool calibrationFlag=false;

/**
 * ---------------------------------
 *      External Interruption
 * ---------------------------------
 */
void interruptArmSwitch()
{
    _armSwitch.changeState();
}

void interruptUserSwitch1()
{
    _userSwitch1.changeState();
}

void interruptUserSwitch2()
{
    _userSwitch2.changeState();
}

void setup()
{

    modeSelector=meal;
    
    /**
     * ---------------------------------
     *      Object initialisation
     * ---------------------------------
     */

    _armSwitch.init();
    pinMode(_armSwitch.getPin(), INPUT);
    attachInterrupt(digitalPinToInterrupt(_armSwitch.getPin()), interruptArmSwitch, CHANGE);

    _userSwitch1.init();
    pinMode(_userSwitch1.getPin(), INPUT);
    attachInterrupt(digitalPinToInterrupt(_userSwitch1.getPin()), interruptUserSwitch1, CHANGE);

    _userSwitch2.init();
    pinMode(_userSwitch2.getPin(), INPUT);
    attachInterrupt(digitalPinToInterrupt(_userSwitch2.getPin()), interruptUserSwitch2, CHANGE);
    
    _userJoystick.init(JOYSTICK_THRESHOLD);
    _led.init();

    //  DXL Communication initialisation use Serial Port 1
    _dxlSerial.begin(57600);
    _dxlSerial.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
    #ifdef DEBUG
    Serial.println();
    Serial.println(F("Device starting"));
    Serial.println();
    delay(TIME_AFTER_DEBUG_MESSAGE);
    #endif

    /**
     * ---------------------------------
     *      Security Checkup
     * ---------------------------------
     */
    #ifdef DEBUG
    Serial.println();
    Serial.println(F("Security Checkup"));
    Serial.println();
    delay(TIME_AFTER_DEBUG_MESSAGE);

    Serial.println();
    Serial.println(F("DXL Communication Test"));
    Serial.println();
    delay(TIME_AFTER_DEBUG_MESSAGE);
    #endif

    //  Check that actuators are connected
    while (_arm.isConnected() == false)
    {
        _led.blink(RED);
        delay(200);
    }

    #ifdef DEBUG
    Serial.println();
    Serial.println(F("Arm connected"));
    Serial.println();
    delay(TIME_AFTER_DEBUG_MESSAGE);
    #endif

    //  Arm initialisation 
    _arm.init(3, JOINT_LIMIT, CALIBRATION_TAB);

    #ifdef DEBUG
    Serial.println();
    Serial.println(F("Home Test"));
    Serial.println();
    delay(TIME_AFTER_DEBUG_MESSAGE);
    #endif

    //  Check if the arm is in correct position to start
    calibrationFlag=true;
    while (_homeFlag == false)
    {
        _homeFlag = _arm.isInHomePosition();
            if (_userSwitch1.getFlag() == LONGPUSHED)
            {
                _led.blink(BLUE);
                delay(800);
                _led.blink(BLUE);
                delay(800);
                _homeFlag = true;
                calibrationFlag=false;
                modeSelector = calibration;
            }
            else
            {
                _arm.turnTorqueOff(ALL);
                _led.blink(RED);
                delay(800);
            }
    }

    if (calibrationFlag == true)
    {
        #ifdef DEBUG
        Serial.println();
        Serial.println(F("Calibrated & Home ok"));
        Serial.println();
        delay(TIME_AFTER_DEBUG_MESSAGE);
        #endif
    }
    else
    {
        #ifdef DEBUG
        Serial.println();
        Serial.println(F("Not calibrated Following Calibration"));
        Serial.println();
        delay(TIME_AFTER_DEBUG_MESSAGE);
        #endif
    }

    // Arm start moving and move in using position
    
    #ifdef DEBUG
    Serial.println();
    Serial.println(F("Home"));
    Serial.println();
    delay(TIME_AFTER_DEBUG_MESSAGE);
    #endif

    _mode=home;

    if ((modeSelector==calibration) || (modeSelector==test))
    {
        _arm.turnTorqueOff(ALL);
        //nothing need to be changed
    }
    else
    {
        _arm.turnTorqueOn(ALL);
        modeSelector=meal;
    }

    _led.blink(GREEN);
    delay(1000);
    _led.blink(GREEN);
    
}

void loop()
{
    switch (_mode)
    {
        /**-------------------
         *      HOME
         * -------------------
         */
        case home:
            switch (modeSelector)
            {
                case meal:
                    #ifdef DEBUG
                    Serial.println();
                    Serial.println(F("Meal"));
                    Serial.println();
                    delay(TIME_AFTER_DEBUG_MESSAGE);
                    #endif

                    _currentMeal.init();
                    _mode=meal;
                    modeSelector=nothing;
                    
                    break;

                case calibration:
                    #ifdef DEBUG
                    Serial.println();
                    Serial.println(F("Calibration"));
                    Serial.println();
                    delay(TIME_AFTER_DEBUG_MESSAGE);
                    #endif

                    _arm.initCalibration();

                    #ifdef DEBUG
                    Serial.println();
                    Serial.println(F("Ready for Calibration"));
                    Serial.println();
                    delay(TIME_AFTER_DEBUG_MESSAGE);
                    #endif

                    _mode=calibration;
                    modeSelector=nothing;
                    break;

                case test:
                    #ifdef DEBUG
                    Serial.println();
                    Serial.println(F("Test"));
                    Serial.println();
                    delay(TIME_AFTER_DEBUG_MESSAGE);
                    #endif

                    _mode=test;
                    modeSelector=nothing;
                    break;

                case powerOff:
                    _led.turnOn(RED);
                    _mode=powerOff;
                    break;
                
                default:
                    break;
            }
            break;

        /**-------------------
         *      MEAL
         * -------------------
         */
        case meal:
            switch (_currentMeal.getMealState())
            {
                case BEGINING:
                    if (_arm.isDeploy()==false)
                    {
                        #ifdef DEBUG
                        Serial.println();
                        Serial.println(F("Arm will be deployed"));
                        Serial.println();
                        delay(TIME_AFTER_DEBUG_MESSAGE);
                        #endif
                        delay(1000);
                        _arm.deploy();

                        #ifdef DEBUG
                        Serial.println();
                        Serial.println(F("Deployed the arm & press userSwitch1"));
                        Serial.println();
                        delay(TIME_AFTER_DEBUG_MESSAGE);
                        #endif
                    }
                    else
                    {
                        if (_userSwitch1.getFlag() == SHORTPUSHED)
                        {
                            _currentMeal.setMealState(MOUTHPOSITIONSAVING);

                            #ifdef DEBUG
                            Serial.println();
                            Serial.println(F("Save mouth position"));
                            Serial.println();
                            delay(TIME_AFTER_DEBUG_MESSAGE);
                            #endif
                        }
                    }
                    break;

                case MOUTHPOSITIONSAVING:
                    _currentMeal.savingMouthPositionSequence();
                    break;

                case ONGOING:
                    _currentMeal.eatingSequence();
                    break;

                case FINISHED:
                    #ifdef DEBUG
                    Serial.println();
                    Serial.println(F("Meal finished"));
                    Serial.println();
                    delay(TIME_AFTER_DEBUG_MESSAGE);
                    #endif
                    _mode=home;
                    modeSelector=powerOff;
                    break;

                default:
                    break;
            }
            break;

        /**-------------------
         *      CALIBRATION
         * -------------------
         */
        case calibration:
            if (calibrationFlag==true)
            {
                #ifdef DEBUG
                Serial.println();
                Serial.println(F("Calibration done"));
                Serial.println();
                delay(TIME_AFTER_DEBUG_MESSAGE);
                #endif

                _mode=home;
                modeSelector=powerOff;
                //_arm.goToHome();
                //_mode=meal;
                //_currentMeal.init();
            }
            else
            {
                calibrationFlag=_arm.calibrationSequence();
            }   
            break;

        /**-------------------
         *      TEST
         * -------------------
         */
        case test:
            _arm.savingPositionSequence();
            break;

        /**-------------------
         *      POWEROFF
         * -------------------
         */
        case powerOff:
            #ifdef DEBUG
            delay(30000);
            Serial.println();
            Serial.println(F("Turn off device"));
            Serial.println();
            delay(TIME_AFTER_DEBUG_MESSAGE);
            #endif
            break;

        default:
            break;
    }
}


