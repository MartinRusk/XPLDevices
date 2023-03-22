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
  /// @param type unipolar (0..1) or bipolar (-1..1) range
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

  /// @brief Perform calibration for bipolar input, current position gets center and +/- ranges are adapted to cover +/-1
  void calibrate();

private:
  float _filterConst;
  float _value;
  float _scale;
  float _scalePos;
  float _scaleNeg;
  int _offset;
  uint8_t _pin;
};

#endif
