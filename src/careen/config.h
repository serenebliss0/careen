#pragma once
#include <Arduino.h>

// Every tunable lives here. No magic numbers in the .cpp files.
namespace cfg {
  // Startup
  constexpr bool START_IN_AUTO = false;      // false = wait for you in MANUAL

  // Safety layer (always on, both modes)
  constexpr int           STOP_CM     = 15;  // forward veto distance
  constexpr unsigned long WATCHDOG_MS = 500; // MANUAL: no command this long -> stop

  // Sensor
  constexpr unsigned long SENSE_EVERY_MS = 60;

  // Autopilot
  constexpr int           AVOID_CM   = 25;   // start avoiding at this distance
  constexpr unsigned long BACKUP_MS  = 600;  // careful: no rear sensor yet
  constexpr unsigned long TURN_MS    = 900;

  // Motors
  constexpr uint8_t DRIVE_SPEED  = 180;
  constexpr uint8_t STEER_SPEED  = 255;
  constexpr bool    DRIVE_INVERT = false;    // flip if forward drives backward
  constexpr bool    STEER_INVERT = false;    // flip if left steers right

  // Lights
  constexpr unsigned long BLINK_MS      = 500;
  constexpr unsigned long BRAKE_HOLD_MS = 1500;

  // Horn (passive buzzer)
  constexpr unsigned long HORN_MS = 200;     // each press / repeat frame honks this long
  constexpr unsigned int  HORN_HZ = 2400;    // loud on small buzzers; ~450 sounds more like a car but is quieter

  // IR remote codes (read from your remote; unmapped codes print to Serial)
  constexpr uint8_t IR_FWD   = 0x18;
  constexpr uint8_t IR_BACK  = 0x52;
  constexpr uint8_t IR_LEFT  = 0x08;
  constexpr uint8_t IR_RIGHT = 0x5A;
  constexpr uint8_t IR_STOP  = 0x1C;
  constexpr uint8_t IR_AUTO  = 0x45;         // placeholder: pick a free button
  constexpr uint8_t IR_HORN  = 0x46;         // placeholder: pick a free button
}