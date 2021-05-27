#include "Joystick.h"

Joystick::Joystick(int speedPin, int directionPin, int refPin, Led *led) : _speedPin(speedPin), _directionPin(directionPin), _refPin(refPin), _led(led)
{

}

void Joystick::init(int threshold)
{
    _state= JOYSTCIKMIDDLE;
    _speed=0;
    _direction=0;
    _ref=0;
    _threshold=threshold;
    _backInMiddleFlag=false;
}

int Joystick::getState()
{
    _speed=analogRead(_speedPin);
    _direction=analogRead(_directionPin);
    _ref=analogRead(_refPin);
    if ((abs(_speed-_ref)>_threshold) ^ (abs(_direction-_ref)>_threshold))
    {
        if (abs(_speed-_ref)>_threshold)
        {
            if (_speed>_ref)
            {
                _led->blink(GREEN);
                return JOYSTCIKUP;
            }
            else
            {
                _led->blink(GREEN);
                return JOYSTCIKDOWN;
            }
        }
        else
        {
            if (_direction>_ref)
            {
                _led->blink(GREEN);
                return JOYSTCIKLEFT;
            }
            else
            {
                _led->blink(GREEN);
                return JOYSTCIKRIGHT;
            }
        }
    }
    else
    {
        _backInMiddleFlag=true;
        return JOYSTCIKMIDDLE;
    }
}

bool Joystick::isBackInMiddle()
{
    return _backInMiddleFlag;
}

void Joystick::needToBackInMiddle()
{
    _backInMiddleFlag=false;
}

Joystick::~Joystick()
{

}