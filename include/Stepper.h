#ifndef Stepper_h
#define Stepper_h

class Stepper
{
public:
  Stepper(uint8_t pin_1, uint8_t pin_2, uint8_t pin_3, uint8_t pin_4);
  Stepper(uint8_t pin_1, uint8_t pin_2, uint8_t pin_3, uint8_t pin_4, uint16_t steps);
  void handle();
  void setIncrements(int32_t pos);
  void setIncrementsRelative(int32_t steps);
  int32_t getIncrements();
  void setPosition(float pos);
  void setPositionRelative(float pos);
  float getPosition();
  void moveTarget();
  bool inTarget();
  void reset();
  void adjust(int32_t steps);
  void setBacklash(int32_t steps);
  void setFeedConst(float feed);
  void setModulo(uint16_t steps);
  void setUnlimited();
  void setPositionLimit(float lower, float upper);
  void setFrequency(uint16_t freq);
  void dirReverse(bool neg);
  void setPowersaveTime(uint16_t seconds);

private:
  bool _step_up();
  bool _step_dn();
  void _step();
  int32_t _trim_modulo(int32_t pos);
  int32_t _diff_modulo(int32_t diff);
  void _add_backlash();
  void _power_off();
  uint16_t _steps_turn;
  int32_t _step_act;
  int32_t _step_target;
  int32_t _backlash;
  int32_t _backlash_act;
  int32_t _step_motor;
  bool _is_modulo;
  bool _is_limited;
  int32_t _steps_modulo;
  int32_t _upper_limit;
  int32_t _lower_limit;
  float _feed_const;
  float _gear_ratio;
  bool _neg_dir;

  // motor pin numbers
  uint8_t _pin_1;
  uint8_t _pin_2;
  uint8_t _pin_3;
  uint8_t _pin_4;
  
  //timing
  unsigned long _delay_powersave;
  unsigned long _delay_step;
  unsigned long _time_last_step;
};

#endif