#include <Arduino.h>
#include "Acceleration.h"
#include "config/Config.h"

float updateAcceleration(AccelState &state, bool moving)
{
  if (!ENABLE_ACCELERATION)
  {
    return 1.0;
  }

  // Joystick no centro: reinicia a aceleracao
  if (!moving)
  {
    state.active = false;
    return 1.0;
  }

  unsigned long currentTime = millis();

  // Acabou de sair do centro
  if (!state.active)
  {
    state.active = true;
    state.startTime = currentTime;
  }

  float progress = (float)(currentTime - state.startTime) / ACCEL_RAMP_TIME;

  if (progress > 1.0)
  {
    progress = 1.0;
  }

  return ACCEL_START_MULTIPLIER +
         (ACCEL_END_MULTIPLIER - ACCEL_START_MULTIPLIER) * progress;
}
