#include "careen.h"
#include "config.h"
#include "pins.h"

static int lastCm = -1;
static unsigned long lastPing = 0;

void sensorBegin() {
  pinMode(pins::TRIG, OUTPUT);
  pinMode(pins::ECHO, INPUT);
  digitalWrite(pins::TRIG, LOW);
}

void sensorUpdate(unsigned long now) {
  if (now - lastPing < cfg::SENSE_EVERY_MS) return;
  lastPing = now;

  digitalWrite(pins::TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(pins::TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(pins::TRIG, LOW);

  // 25 ms timeout is roughly 4 m. This blocks only when nothing is in range.
  const unsigned long us = pulseIn(pins::ECHO, HIGH, 25000UL);
  lastCm = (us == 0) ? -1 : (int)(us * 0.0343f / 2.0f);
}

int sensorCm() { return lastCm; }
