#ifndef Encoder_h
#define Encoder_h
#include <DigitalIn.h>

enum EncCmd_t
{
  encCmdUp,
  encCmdDown,
  encCmdPush
};

enum EncPulse_t
{
  enc1Pulse = 1,
  enc2Pulse = 2,
  enc4Pulse = 4
};

class Encoder
{
public:
  Encoder(uint8_t mux, uint8_t pin1, uint8_t pin2, uint8_t pin3, EncPulse_t pulses);
  Encoder(uint8_t pin1, uint8_t pin2, uint8_t pin3, EncPulse_t pulses) : Encoder(NOT_USED, pin1, pin2, pin3, pulses) {}
  void handle();
  void handleXP()   { handle(); processCommand(); };
  int16_t pos()     { return _count; };
  bool up()         { return _count >= _pulses ? (_count -= _pulses, true) : false; };
  bool down()       { return _count <= -_pulses ? (_count += _pulses, true) : false; };
  bool pressed()    { return _transition == transPressed  ? (_transition = transNone, true) : false; };
  bool released()   { return _transition == transReleased ? (_transition = transNone, true) : false; };
  bool engaged()    { return _state > 0; };
  void setCommand(int cmdUp, int cmdDown, int cmdPush);
  void setCommand(int cmdUp, int cmdDown) { setCommand(cmdUp, cmdDown, -1); };
  int getCommand(EncCmd_t cmd);
  void processCommand();
private:
  enum
  {
    transNone,
    transPressed,
    transReleased
  };
  uint8_t _mux;
  uint8_t _pin1, _pin2, _pin3;
  int8_t _count;
  uint8_t _pulses;
  uint8_t _state;
  uint8_t _debounce;
  uint8_t _transition;
  int _cmdUp;
  int _cmdDown;
  int _cmdPush;
};

#endif
