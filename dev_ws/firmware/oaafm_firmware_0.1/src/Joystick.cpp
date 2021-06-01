#include "Joystick.h"

Joystick::Joystick(int speedPin, int directionPin, int refPin, Led *led) : _speedPin(speedPin), _directionPin(directionPin), _refPin(refPin), _led(led)
{

}

void Joystick::init(int threshold)
{
    _state= jMiddle;
    _lastState= jMiddle;
    _speed=0;
    _direction=0;
    _ref=0;
    _threshold=threshold;
}

void Joystick::readState()
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
                _state=jUp;
            }
            else
            {
                _state=jDown;
            }
        }
        else
        {
            if (_direction>_ref)
            {
                _state=jLeft;
            }
            else
            {
                _state=jRight;
            }
        }
    }
    else
    {
        _state=jMiddle;
    }
}

JoysitckFlag Joystick::getCommand()
{
    this->readState();
    if (_lastState==jMiddle)
    {
        _lastState=_state;
        if (_state!=jMiddle)
        {
            _led->blink(GREEN);
        }
        return _state;
    }
    else
    {
        // nothing to return because the joystick isn't back to the middle
        _lastState=_state;
        return jMiddle;
    }
}

Joystick::~Joystick()
{

}