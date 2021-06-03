#include "Led.h"


Led::Led(int bluePin, int greenPin, int redPin, int _blinkTime) : _bluePin(bluePin), _greenPin(greenPin), _redPin(redPin)
{

}

void Led::init()
{
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);
    _state=OFF;
    _blinkTime=200;
}

void Led::setState(int state)
{
    _state=state;
    switch (_state)
    {
    case RED:
        digitalWrite(_redPin, HIGH);
        digitalWrite(_greenPin, LOW);
        digitalWrite(_bluePin, LOW);
        break;
    
    case BLUE:
        digitalWrite(_redPin, LOW);
        digitalWrite(_greenPin, LOW);
        digitalWrite(_bluePin, HIGH);
        break;

    case GREEN:
        digitalWrite(_redPin, LOW);
        digitalWrite(_greenPin, HIGH);
        digitalWrite(_bluePin, LOW);
        break;
    
    default:
        digitalWrite(_redPin, LOW);
        digitalWrite(_greenPin, LOW);
        digitalWrite(_bluePin, LOW);
        break;
    }
}

int Led::getState()
{
    return _state;
}

void Led::blink(int color)
{
    this->turnOn(color);
    delay(_blinkTime);
    this->turnOff(color);
}

void Led::turnOn(int color)
{
    switch (color)
    {
    case RED:
        digitalWrite(_redPin, HIGH);
        break;
    
    case BLUE:
        digitalWrite(_bluePin, HIGH);
        break;

    case GREEN:
        digitalWrite(_greenPin, HIGH);
        break;
    default:
        break;
    }
}

void Led::turnOff(int color)
{
    switch (color)
    {
    case RED:
        digitalWrite(_redPin, LOW);
        break;
    
    case BLUE:
        digitalWrite(_bluePin, LOW);
        break;

    case GREEN:
        digitalWrite(_greenPin, LOW);
        break;
    default:
        break;
    }
}

Led::~Led()
{
}
