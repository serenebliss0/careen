#include "careen.h"
#include "config.h"
#include "pins.h"

// dir: +1, -1 or 0 (coast)
static void channel(uint8_t en, uint8_t inA, uint8_t inB, int dir, uint8_t speed) {
  digitalWrite(inA, dir > 0 ? HIGH : LOW);
  digitalWrite(inB, dir < 0 ? HIGH : LOW);
  analogWrite(en, dir == 0 ? 0 : speed);
}

void motorsApply(const Motion& m) {
  int drive = (m.drive == Drive::FORWARD) ? 1 : (m.drive == Drive::REVERSE) ? -1 : 0;
  int steer = (m.steer == Steer::LEFT)    ? 1 : (m.steer == Steer::RIGHT)   ? -1 : 0;
  if (cfg::DRIVE_INVERT) drive = -drive;
  if (cfg::STEER_INVERT) steer = -steer;

  channel(pins::EN_DRIVE, pins::IN3, pins::IN4, drive, cfg::DRIVE_SPEED);
  channel(pins::EN_STEER, pins::IN1, pins::IN2, steer, cfg::STEER_SPEED);
}

void motorsBegin() {
  const uint8_t out[] = {pins::EN_STEER, pins::IN1, pins::IN2,
                         pins::EN_DRIVE, pins::IN3, pins::IN4};
  for (uint8_t p : out) pinMode(p, OUTPUT);
  motorsApply(Motion());
}
