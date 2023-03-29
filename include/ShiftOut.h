#ifndef ShiftOut_h
#define ShiftOut_h
#include <Arduino.h>

/// @brief Class to encapsulate a DM13A LED driver IC
class ShiftOut
{
public:
  /// @brief Constructor, setup DM13A LED driver and set pins
  /// @param pin_DAI DAI pin of DM13A
  /// @param pin_DCK DCL pin of DM13A
  /// @param pin_LAT LAT pin of DM13A
  /// @param pins Number of LED pins for cascaded LED drivers (max 64)
  ShiftOut(uint8_t pin_DAI, uint8_t pin_DCK, uint8_t pin_LAT, uint8_t pins = 16);

  /// @brief Set one LED to a display mode
  /// @param pin DM13A pin of the LED (0-15)
  /// @param mode LED display mode
  void setPin(uint8_t pin, bool state);

  /// @brief Set display mode for all 16 LEDs
  /// @param mode LED display mode
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
