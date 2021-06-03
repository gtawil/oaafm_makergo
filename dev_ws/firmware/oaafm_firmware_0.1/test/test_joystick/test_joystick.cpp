/**
 * @file test_joystick.cpp
 * @author your name (you@domain.com)
 * @brief Unit test for joystick or analogical command
 * @version 0.1
 * @date 2021-06-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "Arduino.h"

#include "Joystick.h"
#include "Config.h"

Led led(BLUE_LED_PIN, GREEN_LED_PIN, RED_LED_PIN);
Joystick joystick(JOYSTICK_SPEED_PIN, JOYSTICK_DIRECTION_PIN, JOYSTICK_REF_PIN, &led);


void setup()
{
    Serial.begin(57600);
    joystick.init(JOYSTICK_THRESHOLD);
}

void loop()
{
    JoysitckFlag command=joystick.getCommand();
    Serial.println();
    Serial.println(command);
    Serial.println();
    switch (command)
    {
    case jRight:
        Serial.println("Right");
        break;

    case jLeft:
        Serial.println("Left");
        break;

    case jUp:
        Serial.println("Up");
        break;

    case jDown:
        Serial.println("Down");
        break;
    
    default:
        break;
    }
    Serial.println();
}