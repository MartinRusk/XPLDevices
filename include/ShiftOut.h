#ifndef ShiftOut_h
#define ShiftOut_h
#include <Arduino.h>

/// @brief Class to encapsulate a DM13A LED driver IC
class ShiftOut
{
public:
  /// @brief Constructor, setup shift register and set pins
  /// @param pin_DAI DAI pin (data)
  /// @param pin_DCK DCL pin (clock)
  /// @param pin_LAT LAT pin (latch)
  /// @param pins Number of pins for cascaded shift registers (max 64)
  ShiftOut(uint8_t pin_DAI, uint8_t pin_DCK, uint8_t pin_LAT, uint8_t pins = 16);

  /// @brief Set one outpot  to a display mode
  /// @param pin Pin to set (0-64)
  /// @param state State to set (HIGH/LOW)
  void setPin(uint8_t pin, bool state);

  /// @brief Set state for all outputs
  /// @param state State to set (HIGH/LOW)
  void setAll(bool state);

  /// @brief Real time handling, call cyclic in loop()
  void handle();

private:
  void _send();
  uint8_t _pin_DAI;
  uint8_t _pin_DCK;
  uint8_t _pin_LAT;
  uint8_t _pins;
  uint8_t _state[8];
  bool _update;
};

#endif
