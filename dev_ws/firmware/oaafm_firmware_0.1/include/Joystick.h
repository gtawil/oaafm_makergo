#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "Arduino.h"
#include "Led.h"

enum Direction
{
    Middle,
    Up,
    UpRight,
    UpLeft,
    Down,
    DownRight,
    DownLeft,
    Left,
    Right
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
        Direction _state;
        Direction _lastState;
        Direction _command;
        int _threshold;
        unsigned long int _timeStamp;
        Led *_led;
    
    public:
        Joystick(int speedPin, int directionPin, int refPin, Led *led);

        void init(int threshold);

        Direction getCommand();
        void readState();
        /*
        TO DO
        int calibration();
        */
        ~Joystick();
};

#endif