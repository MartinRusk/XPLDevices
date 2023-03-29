#ifndef LedShift_h
#define LedShift_h
#include <Arduino.h>

/// @brief LED display modes to show
enum led_t
{
  /// @brief LED permanent off
  ledOff = 0x00,
  /// @brief LED fast flashing (300 ms cycle)
  ledFast = 0x01,
  /// @brief LED medium fast flashing (600 ms cycle)
  ledMedium = 0x02,
  /// @brief LED slow flashing (1200 ms cycle)
  ledSlow = 0x04,
  /// @brief LED permanent on
  ledOn = 0x08
};

/// @brief Class to encapsulate a DM13A LED driver IC
class LedShift
{
public:
  /// @brief Constructor, setup DM13A LED driver and set pins
  /// @param pin_DAI DAI pin of DM13A
  /// @param pin_DCK DCL pin of DM13A
  /// @param pin_LAT LAT pin of DM13A
  /// @param pins Number of LED pins for cascaded LED drivers (max 64)
  LedShift(uint8_t pin_DAI, uint8_t pin_DCK, uint8_t pin_LAT, uint8_t pins = 16);

  /// @brief Set one LED to a display mode
  /// @param pin DM13A pin of the LED (0-64)
  /// @param mode LED display mode (ledOff, ledFast, ledMedium, ledSlow, ledOn)
  void setPin(uint8_t pin, led_t mode);
  void set(uint8_t pin, led_t mode) { setPin(pin, mode); }; // obsolete

  /// @brief Set display mode for all LEDs
  /// @param mode LED display mode (ledOff, ledFast, ledMedium, ledSlow, ledOn)
  void setAll(led_t mode);
  void set_all(led_t mode) { setAll(mode); }; // obsolete

  /// @brief Real time handling, call cyclic in loop()
  void handle();

private:
  void _send();
  uint8_t _pin_DAI;
  uint8_t _pin_DCK;
  uint8_t _pin_LAT;
  uint8_t _pins;
  led_t _mode[64];
  uint8_t _count;
  unsigned long _timer;
  bool _update;
};

#endif
