#include <Arduino.h>
#include "AppCommunication.h"
#include "Config.h"

Led led(BLUE_LED_PIN, GREEN_LED_PIN, RED_LED_PIN);

AppCommunication app(&BT_SERIAL, &led);

void setup()
{
    // put your setup code here, to run once:
    led.init();

    Serial.begin(9600);

    app.init();

    while (app.getMessageLabel() != "deployed")
    {
        app.readBluetoothData();
    }
    app.nextMessage();
}

void loop()
{
    // put your main code here, to run repeatedly:
    app.readBluetoothData();
    delay(800);
    if (app.messageIsWaiting())
    {
        Serial.print(app.getMessageLabel());
        Serial.print(app.getMessageValue());
        app.nextMessage();
    }
}