#include <Arduino.h>
#include "Timer.h"

Timer::Timer(float cycle)
{
  setCycle(cycle);
  _lastUpdateTime = micros();
}

void Timer::setCycle(float cycle)
{  
  _cycleTime = (unsigned long)(cycle * 1000.0);
}

bool Timer::elapsed()
{
  _count++;
  unsigned long now = micros();
  if (now > _lastUpdateTime + _cycleTime)
  {
   _lastUpdateTime = now;
   return true; 
  }
  return false;
}

float Timer::getTime()
{
  unsigned long now = micros();
  unsigned long cycle = now - _lastUpdateTime;
  _lastUpdateTime = now;
  return (float)cycle * 0.001;
}

long Timer::count()
{
  long ret = _count;
  _count = 0;
  return ret;
}