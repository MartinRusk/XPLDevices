#include <Arduino.h>
#include <XPLDirect.h>
#include "Switch.h"

#ifndef DEBOUNCE_DELAY
#define DEBOUNCE_DELAY 20
#endif

Switch::Switch(uint8_t mux, uint8_t pin)
{
  _mux = mux;
  _pin = pin;
  _state = switchOff;
  _cmdOff = -1;
  _cmdOn = -1;
  pinMode(_pin, INPUT_PULLUP);
}

void Switch::handle()
{
  if (_debounce > 0)
  {
    _debounce--;
  }
  else 
  {
    SwState_t input = switchOff;
    if (DigitalIn.getBit(_mux, _pin))
    {
      input = switchOn;
    }
    if (input != _state)
    {
      _debounce = DEBOUNCE_DELAY;
      _state = input;
      _transition = true;
    }
  }
}

void Switch::setCommand(int cmdOn, int cmdOff)
{
  _cmdOn = cmdOn;
  _cmdOff = cmdOff;
}

void Switch::setCommand(XPString_t *cmdNameOn, XPString_t *cmdNameOff)
{
  _cmdOn = XP.registerCommand(cmdNameOn);
  _cmdOff = XP.registerCommand(cmdNameOff);
}

int Switch::getCommand()
{
  switch (_state)
  {
  case switchOff:
    return _cmdOff;
    break;
  case switchOn:
    return _cmdOn;
    break;
  default:
    return -1;
    break;
  }
}

void Switch::processCommand()
{
  if (_transition)
  {
    XP.commandTrigger(getCommand());
    _transition = false;
  }
}

// Switch 2

Switch2::Switch2(uint8_t mux, uint8_t pin1, uint8_t pin2)
{
  _mux = mux;
  _pin1 = pin1;
  _pin2 = pin2;
  _state = switchOff;
  _cmdOff = -1;
  _cmdOn1 = -1;
  _cmdOn2 = -1;
  if (_mux == NOT_USED)
  {
    pinMode(_pin1, INPUT_PULLUP);
    pinMode(_pin2, INPUT_PULLUP);
  }
}

void Switch2::handle()
{
  if (_debounce > 0)
  {
    _debounce--;
  }
  else
  {
    SwState_t input = switchOff;
    if (DigitalIn.getBit(_mux, _pin1))
    {
      input = switchOn1;
    }
    else if (DigitalIn.getBit(_mux, _pin2))
    {
      input = switchOn2;
    }
    if (input != _state)
    {
      _debounce = DEBOUNCE_DELAY;
      _lastState = _state;
      _state = input;
      _transition = true;
    }
  }
}

void Switch2::setCommand(int cmdUp, int cmdDown)
{
  _cmdOn1 = cmdUp;
  _cmdOff = cmdDown;
  _cmdOn2 = -1;
}

void Switch2::setCommand(XPString_t *cmdNameUp, XPString_t *cmdNameDown)
{
  _cmdOn1 = XP.registerCommand(cmdNameUp);
  _cmdOff = XP.registerCommand(cmdNameDown);
  _cmdOn2 = -1;
}

void Switch2::setCommand(int cmdOn1, int cmdOff, int cmdOn2)
{
  _cmdOn1 = cmdOn1;
  _cmdOff = cmdOff;
  _cmdOn2 = cmdOn2;
}

void Switch2::setCommand(XPString_t *cmdNameOn1, XPString_t *cmdNameOff, XPString_t *cmdNameOn2)
{
  _cmdOn1 = XP.registerCommand(cmdNameOn1);
  _cmdOff = XP.registerCommand(cmdNameOff);
  _cmdOn2 = XP.registerCommand(cmdNameOn2);
}

int Switch2::getCommand()
{
  if (_cmdOn2 == -1)
  {
    if (_state == switchOn1)
    {
      return _cmdOn1;
    }
    if (_state == switchOff && _lastState == switchOn1)
    {
      return _cmdOff;
    }
    if (_state == switchOn2)
    {
      return _cmdOff;
    }
    if (_state == switchOff && _lastState == switchOn2)
    {
      return _cmdOn1;
    }
  }
  else
  {
    if (_state == switchOn1)
    {
      return _cmdOn1;
    }
    if (_state == switchOff)
    {
      return _cmdOff;
    }
    if (_state == switchOn2)
    {
      return _cmdOn2;
    }
  }
  return -1;
}

void Switch2::processCommand()
{
  if (_transition)
  {
    XP.commandTrigger(getCommand());
    _transition = false;
  }
}
