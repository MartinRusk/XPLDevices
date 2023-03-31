#include <Arduino.h>
#include "AnalogIn.h"

#define FULL_SCALE ((1 << AD_RES) - 1)
#define HALF_SCALE (1 << (AD_RES - 1))

AnalogIn::AnalogIn(uint8_t pin, Analog_t type)
{
  _pin = pin;
  _filterConst = 1.0;
  _scale = 1.0;
  _min = 0;
  _max = FULL_SCALE;
  _type = type;
  pinMode(_pin, INPUT);
  if (_type == bipolar)
  {
    _offset = HALF_SCALE;
  }
  else
  {
    _offset = 0;
  }
  _calcScales();
}

AnalogIn::AnalogIn(uint8_t pin, Analog_t type, float timeConst) : AnalogIn(pin, type)
{
  if (timeConst > 0)
  {
    _filterConst = 1.0 / timeConst;
  }
}

void AnalogIn::handle()
{
  int _raw = raw();
  _value = (_filterConst * _raw * (_raw >= 0 ? _scalePos : _scaleNeg)) + (1.0 - _filterConst) * _value;
}

int AnalogIn::raw()
{
  return constrain(analogRead(_pin), (int16_t)_min, (int16_t)_max) - _offset;
}

void AnalogIn::calibrate()
{
  if (_type == unipolar)
  {
    return;
  }
  long sum = 0;
  for (int i = 0; i < 64; i++)
  {
    sum += analogRead(_pin);
  }
  _offset = (int)(sum / 64);
  _calcScales();
}

void AnalogIn::setRange(uint16_t min, uint16_t max)
{
  _min = min(min, max);
  _max = max(min, max);
  if (min == max)
  {
    _min = 0;
    _max = FULL_SCALE;
  } 
  if (_type == unipolar)
  {
    _offset = _min;
  }
  else
  {
    _offset = (_max + _min) / 2;
  }
  _calcScales();
}

void AnalogIn::setScale(float scale)
{
  _scale = scale;
  _calcScales();
}

void AnalogIn::_calcScales()
{
  if (_type == unipolar)
  {
    _scalePos = _scale / (float)(_max - _min);
    _scaleNeg = 0;
  }
  else
  {
    _scalePos = (_offset == _max) ? 0 : _scale / (float)(_max - _offset);
    _scaleNeg = (_offset == _min) ? 0 : _scale / (float)(_offset - _min);
  }
}
