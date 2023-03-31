#ifndef AnalogIn_h
#define AnalogIn_h
#include <Arduino.h>

#define AD_RES 10

enum Analog_t
{
  unipolar,
  bipolar
};

/// @brief Class to encapsulate analog inputs
class AnalogIn
{
public:
  /// @brief Setup analog input
  /// @param pin Arduino pin number to use
  /// @param type unipolar (0..scale) or bipolar (-scale..scale) range. 
  AnalogIn(uint8_t pin, Analog_t type);

  /// @brief Setup analog input with low pass filter
  /// @param pin Arduino pin number to use
  /// @param type unipolar (0..1) or bipolar (-1..1)
  /// @param timeConst Filter time constant (t_filter/t_sample)
  AnalogIn(uint8_t pin, Analog_t type, float timeConst);

  /// @brief Read analog input, scale value and perform filtering, call once per sample loop
  void handle();

  /// @brief Return actual value
  /// @return Actual, filtered value as captured with handle()
  float value() { return _value; };

  /// @brief Return raw value
  /// @return Read raw analog input and compensate bipolta offset
  int raw();

  /// @brief Perform calibration for bipolar input, current position gets center and min/max ranges 
  /// are adapted to cover +/- scale. Usage is only sensible for small deviations like for joysticks.
  void calibrate();

  /// @brief Set subrange for mechanically limited potentiometers and limit output value to this range.
  /// for bipolar applications the offset is set to the center value of this range.
  /// @param min Minimum value in raw digits (maps to Zero)
  /// @param max Maximum value in raw digits (maps to Scale)
  void setRange(uint16_t min, uint16_t max);

  /// @brief Set output scale for max input range. Default scale is 1.0
  /// @param scale Scale of output value for maximum range
  void setScale(float scale);

private:
  void _calcScales();
  float _value;
  float _filterConst;
  float _scale;
  float _scalePos;
  float _scaleNeg;
  uint16_t _offset;
  uint16_t _min;
  uint16_t _max;
  uint8_t _pin;
  Analog_t _type;  
};

#endif
