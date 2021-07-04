#include <Arduino.h>
#include "AppCommunication.h"
#include "Config.h"

Led led(ledBluePin, ledGreenPin, ledRedPin);

AppCommunication app(&BT_SERIAL, &led);

void setup() {
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

void loop() {
  // put your main code here, to run repeatedly:
  app.readBluetoothData();
  Serial.print(app.getMessageLabel());
  app.nextMessage();
}