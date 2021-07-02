#include <Arduino.h>

#include "upTimeout.h"
#include <error-state.h>
#include <every.h>

class TimeoutEntry {
public:
  time_t delay;
  time_t timestamp;
  timeoutfunc_t callback;

  TimeoutEntry() {}
  TimeoutEntry(time_t timestamp, time_t delay, timeoutfunc_t callback)
      : timestamp(timestamp), delay(delay), callback(callback) {}
};

#ifndef NUM_TIMEOUT_SLOTS
#define NUM_TIMEOUT_SLOTS 12
#endif

static uint8_t num_active_entries = 0;
static TimeoutEntry entries[NUM_TIMEOUT_SLOTS];

void upTimeout::setup() {
  //
}

void upTimeout::loop() {
  EVERY(10 MILLISECONDS) {
    if (num_active_entries > 0) {
      time_t t_now = millis();

      for (uint8_t i = 0; i < NUM_TIMEOUT_SLOTS; i++) {
        if (entries[i].timestamp > 0 && (t_now >= (entries[i].timestamp + entries[i].delay))) {
          entries[i].callback();

          entries[i].timestamp = 0;
          entries[i].delay = 0;

          num_active_entries--;
        }

        if (num_active_entries == 0) {
          break;
        }
      }
    }
  }
}

uint8_t upTimeout::after(time_t delay, timefunc_t timefunc, timeoutfunc_t f) {
  for (uint8_t i = 0; i < NUM_TIMEOUT_SLOTS; i++) {
    if (!entries[i].timestamp) {
      num_active_entries++;

      time_t t_now = timefunc();

      // timestamp == 0 means inactive, so must be at least 1
      entries[i].timestamp = t_now == 0 ? 1 : t_now;
      entries[i].delay = delay;
      entries[i].callback = f;

      return i;
    }
  }

  // If we get here, there were no empty timeout slots available, go into error state.
  error_state(ERROR_STATE_PLUGIN_TIMEOUT);
}
