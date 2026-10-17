// Arduino.h must come BEFORE IRremote (an .ino gets it for free, a .cpp does not).
#include <Arduino.h>
// IRremote's .hpp contains the implementation, so include it in ONE .cpp only.
#include <IRremote.hpp>
#include "careen.h"
#include "config.h"
#include "pins.h"

void irBegin() {
  // LED feedback would blink pin 13, which is the brake light pin.
  IrReceiver.begin(pins::IR_RX, DISABLE_LED_FEEDBACK);
}

Cmd irPoll() {
  static Cmd last = Cmd::NONE;
  if (!IrReceiver.decode()) return Cmd::NONE;

  const auto& d = IrReceiver.decodedIRData;
  Cmd out = Cmd::NONE;

  if (d.protocol == UNKNOWN) {
    // noise, ignore
  } else if (d.flags & IRDATA_FLAGS_IS_REPEAT) {
    // button held: keep the last movement command alive (never repeat the mode toggle)
    out = (last == Cmd::TOGGLE_AUTO) ? Cmd::NONE : last;
  } else {
    switch (d.command) {
      case cfg::IR_FWD:   out = Cmd::FORWARD;     break;
      case cfg::IR_BACK:  out = Cmd::BACK;        break;
      case cfg::IR_LEFT:  out = Cmd::LEFT;        break;
      case cfg::IR_RIGHT: out = Cmd::RIGHT;       break;
      case cfg::IR_STOP:  out = Cmd::STOP;        break;
      case cfg::IR_AUTO:  out = Cmd::TOGGLE_AUTO; break;
      case cfg::IR_HORN:  out = Cmd::HORN;        break;
      default:
        Serial.print(F("IR unmapped: 0x"));
        Serial.println(d.command, HEX);
        break;
    }
    last = out;
  }

  IrReceiver.resume();
  return out;
}

// tone() and IRremote both want Timer2 on the Uno. The horn pauses the
// receiver while it honks, then restarts it (start() reconfigures the timer).
void irPause()  { IrReceiver.stop(); }
void irResume() { IrReceiver.start(); }