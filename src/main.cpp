#include <Arduino.h>
#include "careen/careen.h"
#include "careen/config.h"

// Input -> Arbiter -> Safety -> Motors + Lights
// Whoever is in charge only proposes a Motion. Safety always gets the last word.

static Mode mode = cfg::START_IN_AUTO ? Mode::AUTO : Mode::MANUAL;
static Motion manualMotion;
static unsigned long lastCmdAt = 0;

static Motion motionFor(Cmd c) {
  switch (c) {
    case Cmd::FORWARD: return Motion(Drive::FORWARD, Steer::CENTER);
    case Cmd::BACK:    return Motion(Drive::REVERSE, Steer::CENTER);
    case Cmd::LEFT:    return Motion(Drive::FORWARD, Steer::LEFT);
    case Cmd::RIGHT:   return Motion(Drive::FORWARD, Steer::RIGHT);
    default:           return Motion();
  }
}

static void setMode(Mode m) {
  mode = m;
  autopilotReset();
  manualMotion = Motion();
  Serial.println(m == Mode::AUTO ? F("mode: AUTO") : F("mode: MANUAL"));
}

// Safety layer: runs in every mode, nothing can bypass it.
static Motion applySafety(Motion m, int cm) {
  if (m.drive == Drive::FORWARD && cm >= 0 && cm <= cfg::STOP_CM) m.drive = Drive::STOP;
  if (m.drive == Drive::STOP) m.steer = Steer::CENTER;   // don't stall the steering motor
  return m;
}

void setup() {
  Serial.begin(9600);
  motorsBegin();
  lightsBegin();
  sensorBegin();
  hornBegin();
  irBegin();
  Serial.println(F("careen ready"));
}

void loop() {
  const unsigned long now = millis();

  sensorUpdate(now);
  const int cm = sensorCm();

  // Arbiter: you always win
  const Cmd c = irPoll();
  if (c == Cmd::HORN) {
    hornHonk(now);                       // horn never changes mode or motion
  } else if (c == Cmd::TOGGLE_AUTO) {
    setMode(mode == Mode::AUTO ? Mode::MANUAL : Mode::AUTO);
    lastCmdAt = now;
  } else if (c != Cmd::NONE) {
    if (mode == Mode::AUTO) setMode(Mode::MANUAL);
    manualMotion = motionFor(c);
    lastCmdAt = now;
  }

  Motion want;
  if (mode == Mode::AUTO) {
    want = autopilotStep(cm, now);
  } else {
    if (now - lastCmdAt > cfg::WATCHDOG_MS) manualMotion = Motion();   // watchdog
    want = manualMotion;
  }

  const Motion safe = applySafety(want, cm);
  motorsApply(safe);
  lightsUpdate(safe, now);
  hornUpdate(now);
}