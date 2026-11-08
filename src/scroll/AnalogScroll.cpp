#include <Arduino.h>
#include "AnalogScroll.h"
#include "config/Config.h"

int updateAnalogScroll(
    int pin,
    int center,
    int deadzone,
    bool invert,
    unsigned long &lastScrollTime)
{
  int value = analogRead(pin);
  int difference = value - center;
  int magnitude = abs(difference);

  if (magnitude <= deadzone)
  {
    return 0;
  }

  int interval = map(
      magnitude,
      deadzone,
      512,
      MAX_SCROLL_INTERVAL,
      MIN_SCROLL_INTERVAL);

  interval = constrain(interval, MIN_SCROLL_INTERVAL, MAX_SCROLL_INTERVAL);

  unsigned long currentTime = millis();

  if (currentTime - lastScrollTime >= (unsigned long)interval)
  {
    // Joystick para cima = Scroll Up
    int direction = (difference < 0) ? 1 : -1;

    if (invert)
    {
      direction *= -1;
    }

    lastScrollTime = currentTime;
    return direction;
  }

  return 0;
}
