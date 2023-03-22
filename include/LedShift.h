#ifndef LedShift_h
#define LedShift_h
#include <Arduino.h>

/// @brief LED display modes to show
enum led_t
{
  /// @brief LED permanent off
  ledOff,
  /// @brief LED slow flashing (1200 ms cycle)
  ledSlow,
  /// @brief LED medium fast flashing (600 ms cycle)
  ledMedium,
  /// @brief LED fast flashing (300 ms cycle)
  ledFast,
  /// @brief LED permanent on
  ledOn
};

/// @brief Class to encapsulate a DM13A LED driver IC
class LedShift
{
public:
  /// @brief Constructor, setup DM13A LED driver and set pins
  /// @param pin_DAI DAI pin of DM13A
  /// @param pin_DCK DCL pin of DM13A
  /// @param pin_LAT LAT pin of DM13A
  LedShift(uint8_t pin_DAI, uint8_t pin_DCK, uint8_t pin_LAT);

  /// @brief Set one LED to a display mode
  /// @param pin DM13A pin of the LED (0-15)
  /// @param mode LED display mode
  void set(uint8_t pin, led_t mode);

  /// @brief Set display mode for all 16 LEDs
  /// @param mode LED display mode
  void set_all(led_t mode);

  /// @brief Real time handling, call cyclic in loop()
  void handle();

private:
  void _send();
  void _update(uint8_t pin);
  uint8_t _pin_DAI;
  uint8_t _pin_DCK;
  uint8_t _pin_LAT;
  uint16_t _state;
  led_t _mode[16];
  uint8_t _count;
  unsigned long _timer;
};

#endif
