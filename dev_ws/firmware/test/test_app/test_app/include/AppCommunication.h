#ifndef AppCommunication_h
#define AppCommunication_h

#include <ArduinoSTL.h>
#include <queue>
#include <iterator>
#include <string>
#include <serstream>

#define LISTEN 1
#define SPEAK 2

struct appMessage
{
  String label;
  int value;
};

struct UsingProfil
{
  int mode;
  int speed;
  int plate;
  int timeShortPushed;
  int timeLongPushed;
};


class AppCommunication
{
private:
  HardwareSerial *_serial;
  Led *_led;
  bool _pairedFlag;
  appMessage _message;
  UsingProfil _usingProfil;
  bool _profilFlag;
  std::queue<appMessage> _messageList;
  int _messageNumber;
  int _state;

public:
  AppCommunication(HardwareSerial *_serial, Led *led);
  void init();
  bool isConnected();
  int getState();

  appMessage *getMessage();
  String getMessageLabel();
  int getMessageValue();

  appMessage *getNextMessage();
  void nextMessage();

  void waitingForProfil();
  UsingProfil *getProfil();

  void readBluetoothData();

  ~AppCommunication();
};

#endif
