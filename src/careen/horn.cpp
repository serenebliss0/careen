#include "careen/careen.h"
#include "config.h"
#include "pins.h"

// Passive buzzer horn. tone() runs on Timer2, which IRremote also uses,
// so the receiver is paused for the (short) duration of each honk.

static bool hornOn = false;
static unsigned long hornUntil = 0;

void hornBegin() {
  pinMode(pins::BUZZER, OUTPUT);
  digitalWrite(pins::BUZZER, LOW);
}

void hornHonk(unsigned long now) {
  hornUntil = now + cfg::HORN_MS;
  if (!hornOn) {
    irPause();
    tone(pins::BUZZER, cfg::HORN_HZ);
    hornOn = true;
  }
}

void hornUpdate(unsigned long now) {
  if (hornOn && (long)(now - hornUntil) >= 0) {
    noTone(pins::BUZZER);
    irResume();
    hornOn = false;
  }
}