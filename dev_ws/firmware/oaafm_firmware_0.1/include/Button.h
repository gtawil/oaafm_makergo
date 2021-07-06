#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "Led.h"

#define NOTHING 0
#define SHORTPUSHED 1
#define LONGPUSHED 10

#define PUSHEDTIMEGAP 900
#define SHORTTIME 200

class Button
{
private:
    int _pin;
    int _state;
    int _lastState;
    unsigned long int _timeStamp; 
    unsigned long int _bounceDelay;
    bool _activationFlag;
    Led *_led;

public:
    Button(int pin);
    Button(int pin, Led *_led);
    
    void init();

    void changeState();

    int getPin();
    int getFlag();
    bool isHolded();

    ~Button();
};

#endif