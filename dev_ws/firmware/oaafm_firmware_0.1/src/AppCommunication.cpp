#include "AppCommunication.h"

AppCommunication::AppCommunication(HardwareSerial *serial, Led *led) : _serial(serial), _led(led), _messageList()
{
    _pairedFlag = false;
    _messageNumber=0;
}

void AppCommunication::init()
{
    _serial->begin(9600);
    _message=appMessage();
    _message.label="";
    _message.value=0;
    while (this->isConnected() == false)
    {
        this->readBluetoothData();
        if (this->getMessageLabel() == "connected")
        {
            if (this->getMessageValue()==1)
            {
                this->nextMessage();
                _pairedFlag = true;
            }
        }
        else
        {
            _led->blink(BLUE);
            delay(200);
        }
    }
}

bool AppCommunication::isConnected()
{
  return _pairedFlag;
}

String AppCommunication::getMessageLabel()
{
    return _messageList.front().label;
}

int AppCommunication::getMessageValue()
{
    return _messageList.front().value;
}

void AppCommunication::readBluetoothData()
{
    if (_serial->available())
    {
        String inputString = "";
        long temps = millis();

        inputString = _serial->readString();

        char inputChar[inputString.length() + 1];
        inputString.toCharArray(inputChar, inputString.length() + 1);

        // Read each command pair
        char *command = strtok(inputChar, "&");
        // boucle sur toutes les commandes
        while (command != 0)
        {
            // Split the command in two values
            char *separator = strchr(command, ':');
            if (separator != 0)
            {
                *separator = 0;
                _message.label= command;
                if (_message.label== "usingProfil")
                {
                    ++separator;
                    _usingProfil.mode = atoi(separator);
                    Serial.println();
                    Serial.println(_usingProfil.mode);
                    ++separator;
                    _usingProfil.speed = atoi(separator);
                    Serial.println();
                    Serial.println(_usingProfil.speed);
                    ++separator;
                    _usingProfil.plate = atoi(separator);
                    Serial.println();
                    Serial.println(_usingProfil.plate);
                    ++separator;
                    _usingProfil.timeShortPushed = atoi(separator);
                    Serial.println();
                    Serial.println(_usingProfil.timeShortPushed);
                    ++separator;
                    _usingProfil.timeLongPushed = atoi(separator);
                    Serial.println();
                    Serial.println(_usingProfil.timeLongPushed);
                    
                    _message.value=_usingProfil.mode; //a delete apres
                }
                else
                {
                    ++separator;
                    _message.value=atoi(separator);
                }
                
            }
            // Find the next command in input string
            command = strtok(0, "&");
            Serial.println();
            Serial.println(F("new message"));
            _messageList.push(_message);
        }
    }
}


void AppCommunication::nextMessage()
{
    _messageList.pop();
}

bool AppCommunication::messageIsWaiting()
{
    if (_messageList.empty())
    {
        return false;
    }
    else
    {
        return true;
    }
}


UsingProfil *AppCommunication::getProfil()
{
    return &_usingProfil;
}

AppCommunication::~AppCommunication()
{
}