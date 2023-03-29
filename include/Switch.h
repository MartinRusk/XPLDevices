#ifndef Switch_h
#define Switch_h
#include <Arduino.h>
#include <DigitalIn.h>

/// @brief Class for a simple on/off switch with debouncing and XPLDirect command handling.
class Switch
{
public:
  /// @brief Constructor. Connect the switch to a pin on a mux.
  /// @param mux mux number (from DigitalIn initialization order)
  /// @param pin pin on the mux (0-15)
  Switch(uint8_t mux, uint8_t pin);

  /// @brief Constructor, set digital input without mux 
  /// @param pin Arduino pin number
  Switch(uint8_t pin) : Switch (NOT_USED, pin) {};
  
  /// @brief Handle realtime. Read input and evaluate any transitions.
  void handle();

  /// @brief Handle realtime and process XPLDirect commands
  void handleXP() { handle(); processCommand(); };

  /// @brief Check whether Switch set to on
  /// @return true: Switch is on
  bool on()       { return _state == switchOn; };
  
  /// @brief Check whether Switch set to off
  /// @return true: Switch is off
  bool off()      { return _state == switchOff; };

  /// @brief Set XPLDirect commands for Switch events (command only for on position)
  /// @param cmdOn Command handle for Switch moved to on as returned by XP.registerCommand()
  void setCommand(int cmdOn);

  /// @brief Set XPLDirect commands for Switch events (command only for on position)
  /// @param cmdNameOn Command for Switch moved to on
  void setCommand(XPString_t *cmdNameOn);

  /// @brief Set XPLDirect commands for Switch events
  /// @param cmdOn Command handle for Switch moved to on as returned by XP.registerCommand()
  /// @param cmdOff Command handle for Switch moved to off as returned by XP.registerCommand()
  void setCommand(int cmdOn, int cmdOff);
  
  /// @brief Set XPLDirect commands for Switch events
  /// @param cmdNameOn Command for Switch moved to on
  /// @param cmdNameOff Command for Switch moved to off
  void setCommand(XPString_t *cmdNameOn, XPString_t *cmdNameOff);

  /// @brief Get XPLDirect command for last transition of Switch
  /// @return Handle of the last command
  int getCommand();
  
  /// @brief Process all transitions to XPLDirect
  void processCommand();
  
  /// @brief Check Status of Switch and translate to float value
  /// @param onValue Value to return when Switch is set to on
  /// @param offValue Value to return when Switch is set to off
  /// @return Returned value
  float value(float onValue, float offValue) { return on() ? onValue : offValue; };

private:
  enum SwState_t
  {
    switchOff,
    switchOn
  };
  uint8_t _mux;
  uint8_t _pin;
  uint8_t _debounce;
  uint8_t _state;
  bool _transition;
  int _cmdOff;
  int _cmdOn;
};

/// @brief Class for an on/off/on switch with debouncing and XPLDirect command handling.
class Switch2
{
public:
  /// @brief Constructor. Connect the switch to pins on a mux.
  /// @param mux mux number (from DigitalIn initialization order)
  /// @param pin1 on1 pin on the mux (0-15)
  /// @param pin2 on2 pin on the mux (0-15)
  Switch2(uint8_t mux, uint8_t pin1, uint8_t pin2);

  /// @brief Constructor, set digital input pins without mux 
  /// @param pin1 on1 Arduino pin number 
  /// @param pin2 on2 Arduino pin number 
  Switch2(uint8_t pin1, uint8_t pin2) : Switch2(NOT_USED, pin1, pin2) {}

  /// @brief Handle realtime. Read inputs and evaluate any transitions.
  void handle();

  /// @brief Handle realtime and process XPLDirect commands
  void handleXP() { handle(); processCommand(); };

  /// @brief Check whether Switch set to off
  /// @return true: Switch is off
  bool off()      { return _state == switchOff; };

  /// @brief Check whether Switch set to on1
  /// @return true: Switch is on1
  bool on1()      { return _state == switchOn1; };

  /// @brief Check whether Switch set to on2
  /// @return true: Switch is on2
  bool on2()      { return _state == switchOn2; };

  /// @brief Set XPLDirect commands for Switch events in cases only up/down commands are to be used
  /// @param cmdUp Command handle for Switch moved from on1 to off or from off to on2 as returned by XP.registerCommand()
  /// @param cmdDown Command handle for Switch moved from on2 to off or from off to on1 as returned by XP.registerCommand()
  void setCommand(int cmdUp, int cmdDown);
  
  /// @brief Set XPLDirect commands for Switch events in cases only up/down commands are to be used
  /// @param cmdNameUp Command for Switch moved from on1 to off or from off to on2 on
  /// @param cmdNameDown Command for Switch moved from on2 to off or from off to on1
  void setCommand(XPString_t *cmdNameUp, XPString_t *cmdNameDown);

  /// @brief Set XPLDirect commands for Switch events in cases separate events for on1/off/on2 are to be used
  /// @param cmdOn1 Command handle for Switch moved to on1 position as returned by XP.registerCommand()
  /// @param cmdOff Command handle for Switch moved to off position as returned by XP.registerCommand()
  /// @param cmdOn2 Command handle for Switch moved to on2 position as returned by XP.registerCommand()
  void setCommand(int cmdOn1, int cmdOff, int cmdOn2);

  /// @brief Set XPLDirect commands for Switch events in cases separate events for on1/off/on2 are to be used
  /// @param cmdNameOn1 Command for Switch moved to on1 position
  /// @param cmdNameOff Command for Switch moved to off position
  /// @param cmdNameOn2 Command for Switch moved to on2 position
  void setCommand(XPString_t *cmdNameOn1, XPString_t *cmdNameOff, XPString_t *cmdNameOn2);

  /// @brief Get XPLDirect command for last transition of Switch
  /// @return Handle of the last command
  int getCommand();

  /// @brief Process all transitions to XPLDirect
  void processCommand();

  /// @brief Check Status of Switch and translate to float value
  /// @param on1Value Value to return when Switch is set to on1
  /// @param offValue Value to return when Switch is set to off
  /// @param on2Value Value to return when Switch is set to on2
  /// @return Returned value
  float value(float on1Value, float offValue, float on2Value) { return (on1() ? on1Value : on2() ? on2Value : offValue); };

private:
  enum SwState_t
  {
    switchOff,
    switchOn1,
    switchOn2
  };
  uint8_t _mux;
  uint8_t _pin1;
  uint8_t _pin2;
  uint8_t _lastState;
  uint8_t _debounce;
  uint8_t _state;
  bool _transition;
  int _cmdOff;
  int _cmdOn1;
  int _cmdOn2;
};

#endif
