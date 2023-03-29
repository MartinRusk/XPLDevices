#include <Arduino.h>
#include "DigitalIn.h"

#define MCP_PIN 254

// constructor
DigitalIn_::DigitalIn_()
{
  _numPins = 0;
  for (uint8_t expander = 0; expander < MUX_MAX_NUMBER; expander++)
  {
    _pin[expander] = NOT_USED;
  }
  _s0 = NOT_USED;
  _s1 = NOT_USED;
  _s2 = NOT_USED;
  _s3 = NOT_USED;
}

// configure 74HC4067 adress pins S0-S3
void DigitalIn_::setMux(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3)
{
  _s0 = s0;
  _s1 = s1;
  _s2 = s2;
  _s3 = s3;    
  pinMode(_s0, OUTPUT);
  pinMode(_s1, OUTPUT);
  pinMode(_s2, OUTPUT);
  pinMode(_s3, OUTPUT);
  #ifdef ARDUINO_ARCH_AVR
  _s0port = digitalPinToPort(_s0);
  _s1port = digitalPinToPort(_s1);
  _s2port = digitalPinToPort(_s2);
  _s3port = digitalPinToPort(_s3);
  _s0mask = digitalPinToBitMask(_s0);
  _s1mask = digitalPinToBitMask(_s1);
  _s2mask = digitalPinToBitMask(_s2);
  _s3mask = digitalPinToBitMask(_s3);
  #endif
}

// Add a 74HC4067
bool DigitalIn_::addMux(uint8_t pin)
{
  if (_numPins >= MUX_MAX_NUMBER)
  {
    return false;
  }
  _pin[_numPins++] = pin;
  pinMode(pin, INPUT);
  return true;
}

#if MCP_MAX_NUMBER > 0
// Add a MCP23017
bool DigitalIn_::addMCP(uint8_t adress)
{
  if (_numMCP >= MCP_MAX_NUMBER)
  {
    return false;
  }
  if (!_mcp[_numMCP].begin_I2C(adress, &Wire))
  {
    return false;
  }
  for (int i = 0; i < 16; i++)
  {
    // TODO: register write iodir = 0xffff, ipol = 0xffff, gppu = 0xffff
    _mcp[_numMCP].pinMode(i, INPUT_PULLUP);
  }
  _numMCP++;
  _pin[_numPins++] = MCP_PIN;
  return true;
}
#endif

// Gets specific channel from expander, number according to initialization order 
bool DigitalIn_::getBit(uint8_t expander, uint8_t channel)
{
  if (expander == NOT_USED)
  {	
  #ifdef ARDUINO_ARCH_AVR
  	return (*portInputRegister(digitalPinToPort(channel)) & digitalPinToBitMask(channel)) ? false : true;
  #else
    return !digitalRead(channel);
  #endif
  } 
  return bitRead(_data[expander], channel);
}

// read all inputs together -> base for board specific optimization by using byte read
void DigitalIn_::handle()
{
  // only if Mux Pins present
#if MCP_MAX_NUMBER > 0  
  if (_numPins > _numMCP)
#else
  if (_numPins > 0)
#endif
  {
    for (uint8_t channel = 0; channel < 16; channel++)
    {
#ifdef ARDUINO_ARCH_AVR
      uint8_t oldSREG = SREG;
      noInterrupts();
      bitRead(channel, 0) ? *portOutputRegister(_s0port) |= _s0mask : *portOutputRegister(_s0port) &= ~_s0mask;
      bitRead(channel, 1) ? *portOutputRegister(_s1port) |= _s1mask : *portOutputRegister(_s1port) &= ~_s1mask;
      bitRead(channel, 2) ? *portOutputRegister(_s2port) |= _s2mask : *portOutputRegister(_s2port) &= ~_s2mask;
      bitRead(channel, 3) ? *portOutputRegister(_s3port) |= _s3mask : *portOutputRegister(_s3port) &= ~_s3mask;
      SREG = oldSREG;
      delayMicroseconds(1);
#else
      digitalWrite(_s0, bitRead(channel, 0));
      digitalWrite(_s1, bitRead(channel, 1));
      digitalWrite(_s2, bitRead(channel, 2));
      digitalWrite(_s3, bitRead(channel, 3));
#endif
      for (uint8_t expander = 0; expander < _numPins; expander++)
      {
        if (_pin[expander] != MCP_PIN)
        {
#ifdef ARDUINO_ARCH_AVR
          bitWrite(_data[expander], channel, (*portInputRegister(digitalPinToPort(_pin[expander])) & digitalPinToBitMask(_pin[expander])) ? false : true);
#else
          bitWrite(_data[expander], channel, !digitalRead(_pin[expander]));
#endif
        }
      }
    }
  }
#if MCP_MAX_NUMBER > 0
  int mcp = 0;
  for (uint8_t expander = 0; expander < _numPins; expander++)
  {
    if (_pin[expander] == MCP_PIN)
    {
      _data[expander] = ~_mcp[mcp++].readGPIOAB();
    }
  }
#endif
}

DigitalIn_ DigitalIn;
