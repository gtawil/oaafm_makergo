#ifndef LED_H
#define LED_H

#include "Arduino.h"

#define OFF 0
#define RED 1
#define BLUE 2
#define GREEN 3

class Led
{
private:
    int _bluePin;
    int _greenPin;
    int _redPin;
    int _state;
    int _blinkTime;
public:
    Led(int bluePin, int greenPin, int redPin, int _blinkTime=200);

    void init();

    void setState(int state);
    int getState();

    void blink(int color);

    void turnOn(int color);
    void turnOff(int color);

    ~Led();
};

#endif