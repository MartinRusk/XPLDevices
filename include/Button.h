#ifndef Button_h
#define Button_h
#include <DigitalIn.h>

// simple Button
class Button
{
private:
  void _handle(bool input);

public:
  Button(uint8_t mux, uint8_t muxpin);
  Button(uint8_t pin) : Button(NOT_USED, pin){};
  void handle()                 { _handle(true); };
  void handle(bool input)       { _handle(input); };
  void handleXP()               { _handle(true); processCommand(); };
  void handleXP(bool input)     { _handle(input); processCommand(); };
  bool pressed()                { return _transition == transPressed  ? (_transition = transNone, true) : false; };
  bool released()               { return _transition == transReleased ? (_transition = transNone, true) : false; };
  bool engaged()                { return _state > 0; };
  void setCommand(int cmdPush)  { _cmdPush = cmdPush; };
  int getCommand()              { return _cmdPush; };
  void processCommand();

protected:
  enum
  {
    transNone,
    transPressed,
    transReleased
  };
  uint8_t _mux;
  uint8_t _pin;
  uint8_t _state;
  uint8_t _transition;
  int _cmdPush;
};

// Button with automatic Repeat
class RepeatButton : public Button
{
private:
  void _handle(bool input);

public:
  RepeatButton(uint8_t mux, uint8_t muxpin, uint32_t delay);
  RepeatButton(uint8_t pin, uint32_t delay) : RepeatButton(NOT_USED, pin, delay){};

protected:
  uint32_t _delay;
  uint32_t _timer;
};

#endif
