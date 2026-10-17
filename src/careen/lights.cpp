#include "careen.h"
#include "config.h"
#include "pins.h"

// Lights only REPORT what the car is doing. They never control anything.

static unsigned long lastMoveAt = 0;
static bool hasMoved = false;

void lightsBegin() {
  pinMode(pins::SIGNAL_LEFT, OUTPUT);
  pinMode(pins::SIGNAL_RIGHT, OUTPUT);
  pinMode(pins::BRAKE, OUTPUT);   // fixes the dim-red-LED bug (was never set to OUTPUT)
  digitalWrite(pins::SIGNAL_LEFT, LOW);
  digitalWrite(pins::SIGNAL_RIGHT, LOW);
  digitalWrite(pins::BRAKE, LOW);
}

void lightsUpdate(const Motion& m, unsigned long now) {
  if (m.drive != Drive::STOP) { lastMoveAt = now; hasMoved = true; }

  // Brake lights: just stopped, or reversing (no dedicated reverse lights yet)
  const bool braking = (m.drive == Drive::REVERSE) ||
                       (hasMoved && m.drive == Drive::STOP &&
                        now - lastMoveAt < cfg::BRAKE_HOLD_MS);

  const bool blinkOn = ((now / cfg::BLINK_MS) % 2) == 0;

  digitalWrite(pins::BRAKE, braking ? HIGH : LOW);
  digitalWrite(pins::SIGNAL_LEFT,  (m.steer == Steer::LEFT  && blinkOn) ? HIGH : LOW);
  digitalWrite(pins::SIGNAL_RIGHT, (m.steer == Steer::RIGHT && blinkOn) ? HIGH : LOW);
}
