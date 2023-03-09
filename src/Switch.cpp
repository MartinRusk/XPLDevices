#include <Arduino.h>
#include <XPLDirect.h>
#include "Switch.h"

#define DEBOUNCE_DELAY 100

Switch::Switch(uint8_t mux, uint8_t pin)
{
  _mux = mux;
  _pin = pin;
  _state = switchOff;
  _cmdOff = -1;
  _cmdOn = -1;
  pinMode(_pin, INPUT_PULLUP);
}

Switch::Switch(uint8_t pin) : Switch (NOT_USED, pin)
{
}

bool Switch::handle()
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
      return true;
    }
  }
  return false;
}

bool Switch::on()
{
  return (_state == switchOn);
}

bool Switch::off()
{
  return (_state == switchOff);
}

void Switch::setCommand(int cmdOn, int cmdOff)
{
  _cmdOn = cmdOn;
  _cmdOff = cmdOff;
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

void Switch::handleCommand()
{
  if (handle())
  {
    XP.commandTrigger(getCommand());
  }
}

Switch2::Switch2(uint8_t mux, uint8_t pin1, uint8_t pin2) : Switch(mux, pin1)
{
  _pin2 = pin2;
  _cmdOn2 = -1;
  if (_mux == NOT_USED)
  {
    pinMode(_pin2, INPUT_PULLUP);
  }
}

Switch2::Switch2(uint8_t pin1, uint8_t pin2) : Switch2 (NOT_USED, pin1, pin2)
{
}

bool Switch2::handle()
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
    else if (DigitalIn.getBit(_mux, _pin2))
    {
      input = switchOn2;
    }
    if (input != _state)
    {
      _debounce = DEBOUNCE_DELAY;
      _lastState = _state;
      _state = input;
      return true;
    }
  }
  return false;
}

bool Switch2::on2()
{
  return (_state == switchOn2);
}

void Switch2::setCommand(int cmdOn, int cmdOff, int cmdOn2, int cmdOff2)
{
  _cmdOn = cmdOn;
  _cmdOff = cmdOff;
  _cmdOn2 = cmdOn2;
  _cmdOff2 = cmdOff2;
}

int Switch2::getCommand()
{
  if (_state == switchOn)
  {
    return _cmdOn;
  }
  if (_state == switchOff && _lastState == switchOn)
  {
    return _cmdOff;
  }
  if (_state == switchOff && _lastState == switchOn2)
  {
    return _cmdOff2;
  }
  if (_state == switchOn2)
  {
    return _cmdOn2;
  }
  return -1;
}