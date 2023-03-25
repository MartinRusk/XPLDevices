#include <Arduino.h>
#include <XPLDirect.h>
#include "Encoder.h"

#ifndef DEBOUNCE_DELAY
#define DEBOUNCE_DELAY 20
#endif

// Encoder with button functionality on MUX
Encoder::Encoder(uint8_t mux, uint8_t pin1, uint8_t pin2, uint8_t pin3, EncPulse_t pulses)
{
  _mux = mux;
  _pin1 = pin1;
  _pin2 = pin2;
  _pin3 = pin3;
  _pulses = pulses;
  _count = 0;
  _state = 0;
  _transition = transNone;
  _cmdUp = -1;
  _cmdDown = -1;
  _cmdPush = -1;
  pinMode(_pin1, INPUT_PULLUP);
  pinMode(_pin2, INPUT_PULLUP);
  if (_pin3 != NOT_USED)
  {
    pinMode(_pin3, INPUT_PULLUP);
  }
}

// real time handling
void Encoder::handle()
{
  // collect new state
  _state = ((_state & 0x03) << 2) | (DigitalIn.getBit(_mux, _pin2) << 1) | (DigitalIn.getBit(_mux, _pin1));
  // evaluate state change
  if (_state == 1 || _state == 7 || _state == 8 || _state == 14)
  {
    _count++;
  }
  if (_state == 2 || _state == 4 || _state == 11 || _state == 13)
  {
    _count--;
  }
  if (_state == 3 || _state == 12)
  {
    _count += 2;
  }
  if (_state == 6 || _state == 9)
  {
    _count -= 2;
  }

  // optional button functionality
  if (_pin3 != NOT_USED)
  {
    if (DigitalIn.getBit(_mux, _pin3))
    {
      if (_debounce == 0)
      {
        _debounce = DEBOUNCE_DELAY;
        _transition = transPressed;
      }
    }
    else if (_debounce > 0)
    {
      if (--_debounce == 0)
      {
        _transition = transReleased;
      }
    }
  }
}

void Encoder::setCommand(int cmdUp, int cmdDown, int cmdPush)
{
  _cmdUp = cmdUp;
  _cmdDown = cmdDown;
  _cmdPush = cmdPush;
}

void Encoder::setCommand(XPString_t *cmdNameUp, XPString_t *cmdNameDown, XPString_t *cmdNamePush)
{
  _cmdUp = XP.registerCommand(cmdNameUp);
  _cmdDown = XP.registerCommand(cmdNameDown);
  _cmdPush = XP.registerCommand(cmdNamePush);
}

void Encoder::setCommand(int cmdUp, int cmdDown)
{
  _cmdUp = cmdUp;
  _cmdDown = cmdDown;
  _cmdPush = -1;
}

void Encoder::setCommand(XPString_t *cmdNameUp, XPString_t *cmdNameDown)
{
  _cmdUp = XP.registerCommand(cmdNameUp);
  _cmdDown = XP.registerCommand(cmdNameDown);
  _cmdPush = -1;
}

int Encoder::getCommand(EncCmd_t cmd)
{
  switch (cmd)
  {
  case encCmdUp:
    return _cmdUp;
    break;
  case encCmdDown:
    return _cmdDown;
    break;
  case encCmdPush:
    return _cmdPush;
    break;
  default:
    return -1;
    break;
  }
}

void Encoder::processCommand()
{
  if (up())
  {
    XP.commandTrigger(_cmdUp);
  }
  if (down())
  {
    XP.commandTrigger(_cmdDown);
  }
  if (_cmdPush >= 0)
  {
    if (pressed())
    {
      XP.commandStart(_cmdPush);
    }
    if (released())
    {
      XP.commandEnd(_cmdPush);
    }
  }
}
