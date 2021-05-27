#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "Arduino.h"
#include "Led.h"

#define JOYSTCIKMIDDLE 0
#define JOYSTCIKUP 1
#define JOYSTCIKDOWN 2
#define JOYSTCIKLEFT 3
#define JOYSTCIKRIGHT 4

class Joystick
{
    private:
        int _speedPin;
        int _directionPin;
        int _refPin;
        int _speed;
        int _direction;
        int _ref;
        int _state;
        bool _backInMiddleFlag;
        int _threshold;
        unsigned long int _timeStamp;
        Led *_led;
    
    public:
        Joystick(int speedPin, int directionPin, int refPin, Led *led);

        void init(int threshold);

        int getState();
        bool isBackInMiddle();
        void needToBackInMiddle();

        //int calibration();

        ~Joystick();
};

#endif