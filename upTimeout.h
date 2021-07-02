#pragma once

#include <stdint.h>
#include <uPlugin.h>

#define ERROR_STATE_PLUGIN_TIMEOUT 0b1011101110100011100011101110111L  // PTO (.--. - ---)
#define ERROR_STATE_PLUGIN_TIMEOUT2 0b1011101110100011100011101110110L // PTO (.--. - ---)

typedef uint32_t time_t;

typedef void (*timeoutfunc_t)(void);
typedef uint32_t (*timefunc_t)(void);

class upTimeout : public uPlugin {
public:
  Timeout() {}

  void setup();
  void loop();

  // After delay ms run timeout function f
  uint8_t after(time_t delay, timefunc_t tf, timeoutfunc_t tof);
};
