#include "careen.h"
#include "config.h"

// wander -> see something -> back up -> turn away -> wander
enum class State : uint8_t { CRUISE, BACKING, TURNING };

static State state = State::CRUISE;
static unsigned long since = 0;
static Steer turnDir = Steer::LEFT;
static bool turnLeftNext = true;     // alternate so it doesn't get stuck in corners

static Steer opposite(Steer s) { return s == Steer::LEFT ? Steer::RIGHT : Steer::LEFT; }

void autopilotReset() { state = State::CRUISE; }

Motion autopilotStep(int cm, unsigned long now) {
  switch (state) {
    case State::CRUISE:
      if (cm >= 0 && cm <= cfg::AVOID_CM) {
        state = State::BACKING;
        since = now;
        turnDir = turnLeftNext ? Steer::LEFT : Steer::RIGHT;
        turnLeftNext = !turnLeftNext;
        return Motion();
      }
      return Motion(Drive::FORWARD, Steer::CENTER);

    case State::BACKING:
      if (now - since >= cfg::BACKUP_MS) { state = State::TURNING; since = now; }
      // reverse with opposite lock so the nose swings toward turnDir
      return Motion(Drive::REVERSE, opposite(turnDir));

    case State::TURNING:
      if (now - since >= cfg::TURN_MS) state = State::CRUISE;
      return Motion(Drive::FORWARD, turnDir);
  }
  return Motion();
}
