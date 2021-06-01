#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "Arduino.h"
#include "Led.h"

enum JoysitckFlag
{
    jMiddle,
    jUp,
    jDown,
    jLeft,
    jRight
};

class Joystick
{
    private:
        int _speedPin;
        int _directionPin;
        int _refPin;
        int _speed;
        int _direction;
        int _ref;
        JoysitckFlag _state;
        JoysitckFlag _lastState;
        JoysitckFlag _command;
        int _threshold;
        unsigned long int _timeStamp;
        Led *_led;
    
    public:
        Joystick(int speedPin, int directionPin, int refPin, Led *led);

        void init(int threshold);

        JoysitckFlag getCommand();
        void readState();
        /*
        TO DO
        int calibration();
        */
        ~Joystick();
};

#endif