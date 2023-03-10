#include <Arduino.h>
#include <XPLDirect.h>
#include "Button.h"

#ifndef DEBOUNCE_DELAY
#define DEBOUNCE_DELAY 20
#endif

enum
{
  transNone,
  transPressed,
  transReleased
};

// Buttons
Button::Button(uint8_t mux, uint8_t pin)
{
  _mux = mux;
  _pin = pin;
  _state = 0;
  _transition = 0;
  _cmdPush = -1;
  pinMode(_pin, INPUT_PULLUP);
}

Button::Button(uint8_t pin) : Button(NOT_USED, pin)
{
}

void Button::handle()
{
  handle(true);
}

// use additional bit for input masking
void Button::handle(bool input)
{
  if (DigitalIn.getBit(_mux, _pin) && input)
  {
    if (_state == 0)
    {
      _state = DEBOUNCE_DELAY;
      _transition = transPressed;
    }
  }
  else if (_state > 0)
  {
    if (--_state == 0)
    {
      _transition = transReleased;
    }
  }
}

bool Button::pressed()
{
  if (_transition == transPressed)
  {
    _transition = transNone;
    return true;
  }
  return false;
}

bool Button::released()
{
  if (_transition == transReleased)
  {
    _transition = transNone;
    return true;
  }
  return false;
}

bool Button::engaged()
{
  return _state > 0;
}

void Button::setCommand(int cmdPush)
{
  _cmdPush = cmdPush;
}

int Button::getCommand()
{
  return _cmdPush;
}

void Button::processCommand()
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

void Button::handleCommand()
{
  handle();
  processCommand();
}

void Button::handleCommand(bool input)
{
  handle(input);
  processCommand();
}

RepeatButton::RepeatButton(uint8_t mux, uint8_t pin, uint32_t delay) : Button(mux, pin)
{
  _delay = delay;
  _timer = 0;
}

RepeatButton::RepeatButton(uint8_t pin, uint32_t delay) : RepeatButton(NOT_USED, pin, delay)
{
}

void RepeatButton::handle()
{
  handle(true);
}

void RepeatButton::handle(bool input)
{
  if (DigitalIn.getBit(_mux, _pin) && input)
  {
    if (_state == 0)
    {
      _state = DEBOUNCE_DELAY;
      _transition = transPressed;
      _timer = millis() + _delay;
    }
    else if (_delay > 0 && (millis() >= _timer))
    {
      _state = DEBOUNCE_DELAY;
      _transition = transPressed;
      _timer += _delay;
    }
  }
  else if (_state > 0)
  {
    if (--_state == 0)
    {
      _transition = transReleased;
    }
  }
}