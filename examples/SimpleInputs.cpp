#include <Arduino.h>
#include <XPLDevices.h>

// Encoder on Pin 2+3, push function on Pin 4, 4 Pulses ber indent
Encoder encHeading(2, 3, 4, eEnc4Pulse);
// simple Pushbutton
Button btnComFF(3);
// simple Switch
Switch swStrobe(4);

// LEDs
LedShift leds(16, 14, 15);
#define LED_1 0
#define LED_2 1

// Setup
void setup()
{
  XPsetup("G1000 MFD");
  encHeading.setCommand(
      XP.registerCommand(F("sim/GPS/g1000n1_hdg_up")),
      XP.registerCommand(F("sim/GPS/g1000n1_hdg_down")),
      XP.registerCommand(F("sim/GPS/g1000n1_hdg_sync")));
  btnComFF.setCommand(
      XP.registerCommand(F("sim/GPS/g1000n3_com_ff")));
  swStrobe.setCommand(
      XP.registerCommand(F("sim/lights/strobe_lights_on")),
      XP.registerCommand(F("sim/lights/strobe_lights_off")));
}

// Main loop
void loop()
{
  XP.xloop();
  handleCommand(&encHeading);
  handleCommand(&btnComFF);
  handleCommand(&swStrobe);
}