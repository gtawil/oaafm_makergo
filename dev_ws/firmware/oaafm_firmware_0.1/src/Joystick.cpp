#include "Joystick.h"

Joystick::Joystick(int speedPin, int directionPin, int refPin, Led *led) : _speedPin(speedPin), _directionPin(directionPin), _refPin(refPin), _led(led)
{

}

void Joystick::init(int threshold)
{
    _state= Middle;
    _lastState= Middle;
    _speed=0;
    _direction=0;
    _ref=0;
    _threshold=threshold;
}

void Joystick::readState()
{
    delay(50);
    _speed=analogRead(_speedPin);
    _direction=analogRead(_directionPin);
    _ref=analogRead(_refPin);
    if (((abs(_speed-_ref)>_threshold) || (abs(_direction-_ref)>_threshold))==true)
    {
        if (((abs(_speed-_ref)>_threshold) && (abs(_direction-_ref)>_threshold))==true)
        {
            if (_speed>_ref)
            {
                if (_direction>_ref)
                {
                    _state=UpRight;
                }
                else
                {
                    _state=UpLeft;   
                }
            }
            else
            {
                if (_direction>_ref)
                {
                    _state=DownRight;
                }
                else
                {
                    _state=DownLeft;   
                }
            }
        }
        else
        {
            if (abs(_speed-_ref)>_threshold)
            {
                if (_speed>_ref)
                {
                    _state=Up;   
                }
                else
                {
                    _state=Down;
                }
            }
            else if (abs(_direction-_ref)>_threshold)
            {
                if (_direction>_ref)
                {
                    _state=Right;
                }
                else
                {
                    _state=Left;
                }
            }
            else
            {
                _state=Middle;
            }
        }      
    }
    else
    {
        _state=Middle;
    }
}

Direction Joystick::getCommand()
{
    this->readState();
    if (_lastState==Middle)
    {
        _lastState=_state;
        if (_state!=Middle)
        {
            _led->blink(GREEN);
        }
        return _state;
    }
    else
    {
        // nothing to return because the joystick isn't back to the middle
        _lastState=_state;
        return Middle;
    }
}

Joystick::~Joystick()
{

}