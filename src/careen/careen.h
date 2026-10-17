#pragma once
#include <Arduino.h>

// What the car is told to do, independent of who is telling it.
enum class Drive : uint8_t { STOP, FORWARD, REVERSE };
enum class Steer : uint8_t { CENTER, LEFT, RIGHT };

struct Motion {
  Drive drive;
  Steer steer;
  constexpr Motion(Drive d = Drive::STOP, Steer s = Steer::CENTER) : drive(d), steer(s) {}
};

// Inputs (IR now, Bluetooth / second board later) all produce these.
enum class Cmd : uint8_t { NONE, FORWARD, BACK, LEFT, RIGHT, STOP, TOGGLE_AUTO, HORN };
enum class Mode : uint8_t { MANUAL, AUTO };

// sensor.cpp
void sensorBegin();
void sensorUpdate(unsigned long now);
int  sensorCm();                 // -1 = no echo (treated as "clear")

// motors.cpp
void motorsBegin();
void motorsApply(const Motion& m);

// lights.cpp
void lightsBegin();
void lightsUpdate(const Motion& m, unsigned long now);

// input_ir.cpp
void irBegin();
Cmd  irPoll();
void irPause();                  // frees Timer2 so tone() can use it
void irResume();

// horn.cpp
void hornBegin();
void hornHonk(unsigned long now);
void hornUpdate(unsigned long now);

// autopilot.cpp
void   autopilotReset();
Motion autopilotStep(int cm, unsigned long now);