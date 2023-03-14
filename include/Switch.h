#ifndef Switch_h
#define Switch_h
#include <DigitalIn.h>

class Switch
{
public:
  Switch(uint8_t mux, uint8_t pin);
  Switch(uint8_t pin) : Switch (NOT_USED, pin) {};
  bool handle();
  void handleXP() { handle(); processCommand(); };
  bool on()       { return _state == switchOn; };
  bool off()      { return _state == switchOff; };
  void setCommand(int cmdOn, int cmdOff);
  int getCommand();
  void processCommand();
  float value(float onValue, float offValue) { return on() ? onValue : offValue; };

private:
  enum SwState_t
  {
    switchOff,
    switchOn
  };
  uint8_t _mux;
  uint8_t _pin;
  uint8_t _debounce;
  uint8_t _state;
  bool _transition;
  int _cmdOff;
  int _cmdOn;
};

class Switch2
{
public:
  Switch2(uint8_t mux, uint8_t pin1, uint8_t pin2);
  Switch2(uint8_t pin1, uint8_t pin2) : Switch2(NOT_USED, pin1, pin2) {}
  bool handle();
  void handleXP() { handle(); processCommand(); };
  bool on()       { return _state == switchOn; };
  bool off()      { return _state == switchOff; };
  bool on2()      { return _state == switchOn2; };
  void setCommand(int cmdOn, int cmdOff);
  int getCommand();
  void processCommand();
  float value(float onValue, float offValue, float on2value) { return (on() ? onValue : on2() ? on2value : offValue); };

private:
  enum SwState_t
  {
    switchOff,
    switchOn,
    switchOn2
  };
  uint8_t _mux;
  uint8_t _pin1;
  uint8_t _pin2;
  uint8_t _lastState;
  uint8_t _debounce;
  uint8_t _state;
  bool _transition;
  int _cmdOff;
  int _cmdOn;
};

#endif
