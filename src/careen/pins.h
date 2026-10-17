#pragma once
#include <Arduino.h>

// Single source of truth for wiring. Matches the KiCad schematic.
namespace pins {
  constexpr uint8_t IR_RX        = 2;   // IR receiver

  constexpr uint8_t SIGNAL_LEFT  = 3;   // 2 white LEDs
  constexpr uint8_t SIGNAL_RIGHT = 4;   // 2 white LEDs
  constexpr uint8_t BRAKE        = 13;  // 4 red LEDs (also the onboard LED pin!)

  // L293D channel 1 = STEER motor, channel 2 = DRIVE motor
  constexpr uint8_t EN_STEER     = 5;   // EN1 (PWM)
  constexpr uint8_t IN1          = 7;
  constexpr uint8_t IN2          = 8;
  constexpr uint8_t EN_DRIVE     = 6;   // EN2 (PWM)
  constexpr uint8_t IN3          = 9;
  constexpr uint8_t IN4          = 10;

  constexpr uint8_t BUZZER       = A1;  // passive buzzer (horn)

  constexpr uint8_t TRIG         = 11;  // HC-SR04
  constexpr uint8_t ECHO         = 12;
}