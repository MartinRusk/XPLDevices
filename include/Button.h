#ifndef Button_h
#define Button_h
#include <Arduino.h>
#include <DigitalIn.h>

/// @brief Class for a simple pushbutton with debouncing and XPLDirect command handling.
/// Supports start and end of commands so XPlane can show the current Button status.
class Button
{
private:
  void _handle(bool input);

public:
  /// @brief Constructor, set mux and pin number
  /// @param mux mux number (from DigitalIn initialization order)
  /// @param muxpin pin on the mux (0-15)
  Button(uint8_t mux, uint8_t muxpin);
  
  /// @brief Constructor, set digital input without mux 
  /// @param pin Arduino pin number
  Button(uint8_t pin) : Button(NOT_USED, pin){};

  /// @brief Handle realtime. Read input and evaluate any transitions.
  void handle()                 { _handle(true); };

  /// @brief Handle realtime. Read input and evaluate any transitions.
  /// @param input Additional mask bit. AND connected with physical input.
  void handle(bool input)       { _handle(input); };

  /// @brief Handle realtime and process XPLDirect commands
  void handleXP()               { _handle(true); processCommand(); };

  /// @brief Handle realtime and process XPLDirect commands
  /// @param input Additional mask bit. AND tied with physical input.
  void handleXP(bool input)     { _handle(input); processCommand(); };

  /// @brief Evaluate and reset transition if button pressed down
  /// @return true: Button was pressed. Transition detected.
  bool pressed()                { return _transition == transPressed  ? (_transition = transNone, true) : false; };

  /// @brief Evaluate and reset transition if button released
  /// @return true: Button was released. Transition detected.
  bool released()               { return _transition == transReleased ? (_transition = transNone, true) : false; };

  /// @brief Evaluate status of Button
  /// @return true: Button is currently held down
  bool engaged()                { return _state > 0; };

  /// @brief Set XPLDirect command for Button events
  /// @param cmdPush Command handle as returned by XP.registerCommand()
  void setCommand(int cmdPush)  { _cmdPush = cmdPush; };

  /// @brief Get XPLDirect command associated with Button
  /// @return Handle of the command
  int getCommand()              { return _cmdPush; };

  /// @brief Process all transitions and active transitions to XPLDirect   
  void processCommand();

protected:
  enum
  {
    transNone,
    transPressed,
    transReleased
  };
  uint8_t _mux;
  uint8_t _pin;
  uint8_t _state;
  uint8_t _transition;
  int _cmdPush;
};

/// @brief Class for a simple pushbutton with debouncing and XPLDirect command handling,
/// supports start and end of commands so XPlane can show the current Button status.
/// When button is held down cyclic new pressed events are generated for auto repeat function.
class RepeatButton : public Button
{
private:
  void _handle(bool input);

public:
  /// @brief Constructor, set mux and pin number
  /// @param mux mux number (from initialization order)
  /// @param muxpin pin on the mux (0-15)
  /// @param delay Cyclic delay for repeat function
  RepeatButton(uint8_t mux, uint8_t muxpin, uint32_t delay);

  /// @brief Constructor, set digital input without mux 
  /// @param pin Arduino pin number
  /// @param delay Cyclic delay for repeat function
  RepeatButton(uint8_t pin, uint32_t delay) : RepeatButton(NOT_USED, pin, delay){};

  /// @brief Handle realtime. Read input and evaluate any transitions.
  void handle()                 { _handle(true); };

  /// @brief Handle realtime. Read input and evaluate any transitions.
  /// @param input Additional mask bit. AND connected with physical input.
  void handle(bool input)       { _handle(input); };

  /// @brief Handle realtime and process XPLDirect commands
  void handleXP()               { _handle(true); processCommand(); };
  
  /// @brief Handle realtime and process XPLDirect commands
  /// @param input Additional mask bit. AND tied with physical input.
  void handleXP(bool input)     { _handle(input); processCommand(); };

protected:
  uint32_t _delay;
  uint32_t _timer;
};

#endif
