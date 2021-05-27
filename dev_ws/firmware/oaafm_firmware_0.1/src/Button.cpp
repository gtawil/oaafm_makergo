#include "Button.h"

Button::Button(int pin) : _pin(pin), _led(0)
{
}

Button::Button(int pin, Led *led) : _pin(pin), _led(led)
{
}

void Button::init()
{
    _state = HIGH;
    _lastState = HIGH;
    _timeStamp = 0;
    _bounceDelay = 0;
    _activationFlag = false;
}

int Button::getPin()
{
    return _pin;
}

void Button::changeState()
{
    _state = digitalRead(_pin);
    if (_state < _lastState)
    {
        _timeStamp = millis();
    }
    else
    {
        _bounceDelay = millis() - _timeStamp;
        _activationFlag = true;
    }
    _lastState = _state;
}

int Button::getFlag()
{
    if (((_state == HIGH) && (_activationFlag == true)) == true)
    {
        Serial.println();
        Serial.println(_bounceDelay);
        _activationFlag = false;
        if (_bounceDelay < 150)
        {
            _bounceDelay = 0;
            return NOTHING;
        }
        else if (_bounceDelay >= 150 && _bounceDelay <= 800)
        {
            _bounceDelay = 0;
            _led->blink(GREEN);
            return SHORTPUSHED;
        }
        else if (_bounceDelay > 800 && _bounceDelay <= 4000)
        {
            _bounceDelay = 0;
            _led->blink(GREEN);
            return LONGPUSHED;
        }
        else if (_bounceDelay > 4000)
        {
            _bounceDelay = 0;
            return NOTHING;
        }
    }
    else
    {
        return NOTHING;
    }
}

bool Button::isHolded()
{
    if ((_state == LOW && (millis() - _timeStamp) > 500))
    {
        return true;
    }
    else
    {
        return false;
    }
}

Button::~Button()
{
}