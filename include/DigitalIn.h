#ifndef DigitalIn_h
#define DigitalIn_h
#include <Arduino.h>

/// @brief Maximum number of 74HC4067 multiplexers
#ifndef MUX_MAX_NUMBER
#define MUX_MAX_NUMBER 6
#endif

/// @brief Maximum number of MCP23017 multiplexers
#ifndef MCP_MAX_NUMBER
#define MCP_MAX_NUMBER 0
#endif

// Include i2c lib only when needed
#if MCP_MAX_NUMBER > 0
#include <Adafruit_MCP23X17.h>
#endif

#define NOT_USED 255

/// @brief Class to encapsulate digital inputs from 74HC4067 and MCP23017 input multiplexers,
/// used by all digital input devices. Scans all mux inputs into internal process data image.
class DigitalIn_
{
public:
  /// @brief Class constructor
  DigitalIn_();

  /// @brief Set adress pins for 74HC4067 multiplexers. All mux share the same adress pins.
  /// @param s0 Adress pin s0
  /// @param s1 Adress pin s1
  /// @param s2 Adress pin s2
  /// @param s3 Adress pin s3
  void setMux(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3);
  
  /// @brief Add one 74HC4067 multiplexer
  /// @param pin Data pin the mux is connected to
  /// @return true when successful, false when all mux have been used up (increase MUX_MAX_NUMBER)
  bool addMux(uint8_t pin);

#if MCP_MAX_NUMBER > 0
  /// @brief Add one MCP23017 i2c multiplexer
  /// @param adress i2c adress of the mux (valid: 0x20-0x28)
  /// @return true when successful, false when all mux have been used up (increase MCP_MAX_NUMBER)
  bool addMCP(uint8_t adress);
#endif
  
  /// @brief Get one bit from the mux or a digital input
  /// @param mux mux to read from. Use NOT_USED to access ardunio digital input without mux
  /// @param muxpin pin (0-15) on the mux or Arduino pin when mux = NOT_USED
  /// @return Status of the input (inverted, true = GND, false = +5V)
  bool getBit(uint8_t mux, uint8_t muxpin);
  
  /// @brief Read all mux inputs into process data input image
  void handle();
private:
  uint8_t _s0, _s1, _s2, _s3;
  uint8_t _numPins;
  uint8_t _pin[MUX_MAX_NUMBER + MCP_MAX_NUMBER];
  int16_t _data[MUX_MAX_NUMBER + MCP_MAX_NUMBER];
#if MCP_MAX_NUMBER > 0
  uint8_t _numMCP;
  Adafruit_MCP23X17 _mcp[MCP_MAX_NUMBER];
#endif
};

/// @brief Instance of the class for system wide use
extern DigitalIn_ DigitalIn;

#endif
