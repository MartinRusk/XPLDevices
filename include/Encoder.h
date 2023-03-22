#ifndef Encoder_h
#define Encoder_h
#include <Arduino.h>
#include <DigitalIn.h>

enum EncCmd_t
{
  encCmdUp,
  encCmdDown,
  encCmdPush
};

enum EncPulse_t
{
  enc1Pulse = 1,
  enc2Pulse = 2,
  enc4Pulse = 4
};

/// @brief Class for rotary encoders with optional push functionality. The number of counts per mechanical notch can be 
/// configured for the triggering of up/down events.
class Encoder
{
public:
  /// @brief Constructor. Sets connected pins and number of counts per notch.
  /// @param mux mux number (from DigitalIn initialization order)
  /// @param pin1 pin for Encoder A track
  /// @param pin2 pin for Encoder B track
  /// @param pin3 pin for encoder push function (NOT_USED if not connected)
  /// @param pulses Number of counts per mechanical notch
  Encoder(uint8_t mux, uint8_t pin1, uint8_t pin2, uint8_t pin3, EncPulse_t pulses);

  /// @brief Constructor. Sets connected pins and number of counts per notch.
  /// @param pin1 pin for Encoder A track
  /// @param pin2 pin for Encoder B track
  /// @param pin3 pin for encoder push function (NOT_USED if not connected)
  /// @param pulses Number of counts per mechanical notch
  Encoder(uint8_t pin1, uint8_t pin2, uint8_t pin3, EncPulse_t pulses) : Encoder(NOT_USED, pin1, pin2, pin3, pulses) {}

  /// @brief Handle realtime. Read input and evaluate any transitions.
  void handle();

  /// @brief Handle realtime and process XPLDirect commands.
  void handleXP()   { handle(); processCommand(); };

  /// @brief Read current Encoder count.
  /// @return Remaining Encoder count.
  int16_t pos()     { return _count; };

  /// @brief Evaluate Encoder up one notch (positive turn) and consume event
  /// @return true: up event available and transition reset.
  bool up()         { return _count >= _pulses ? (_count -= _pulses, true) : false; };
  
  /// @brief Evaluate Encoder up down notch (negative turn) and consume event
  /// @return true: up event available and transition reset.
  bool down()       { return _count <= -_pulses ? (_count += _pulses, true) : false; };
  
  /// @brief Evaluate and reset transition if Encoder pressed down
  /// @return true: Button was pressed. Transition detected and reset.
  bool pressed()    { return _transition == transPressed  ? (_transition = transNone, true) : false; };

  /// @brief Evaluate and reset transition if Encoder released
  /// @return true: Button was released. Transition detected and reset.
  bool released()   { return _transition == transReleased ? (_transition = transNone, true) : false; };

  /// @brief Evaluate status of Encoder push function
  /// @return true: Button is currently held down  
  bool engaged()    { return _state > 0; };

  /// @brief Set XPLDirect commands for Encoder events
  /// @param cmdUp Command handle for positive turn as returned by XP.registerCommand()
  /// @param cmdDown Command handle for negative turn as returned by XP.registerCommand()
  /// @param cmdPush Command handle for push as returned by XP.registerCommand()
  void setCommand(int cmdUp, int cmdDown, int cmdPush);

  /// @brief Set XPLDirect commands for Encoder events without push function
  /// @param cmdUp Command handle for positive turn as returned by XP.registerCommand()
  /// @param cmdDown Command handle for negative turn as returned by XP.registerCommand()
  void setCommand(int cmdUp, int cmdDown) { setCommand(cmdUp, cmdDown, -1); };

  /// @brief Get XPLDirect command assiciated with the selected event
  /// @param cmd Event to read out (encCmdUp, encCmdDown, encCmdPush)
  /// @return Handle of the command, -1 = no command
  int getCommand(EncCmd_t cmd);

  /// @brief Check for Encoder events and process XPLDirect commands as appropriate
  void processCommand();
private:
  enum
  {
    transNone,
    transPressed,
    transReleased
  };
  uint8_t _mux;
  uint8_t _pin1, _pin2, _pin3;
  int8_t _count;
  uint8_t _pulses;
  uint8_t _state;
  uint8_t _debounce;
  uint8_t _transition;
  int _cmdUp;
  int _cmdDown;
  int _cmdPush;
};

#endif
