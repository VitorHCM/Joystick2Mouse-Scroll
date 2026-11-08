#include <Arduino.h>
#include <Mouse.h>
#include "Joystick.h"
#include "Acceleration.h"
#include "buttons/Buttons.h"
#include "scroll/AnalogScroll.h"
#include "config/Config.h"

static unsigned long lastAnalogScroll1 = 0;
static unsigned long lastAnalogScroll2 = 0;

static AccelState accel1;
static AccelState accel2;

// Converte a leitura do joystick em velocidade do mouse
static int calculateMouseSpeed(int rawValue, int center, int deadzone, bool invert)
{
  int difference = rawValue - center;
  int magnitude = abs(difference);

  if (magnitude <= deadzone)
  {
    return 0;
  }

  // Remove a deadzone e normaliza a faixa restante
  magnitude = constrain(magnitude, deadzone, 512);

  int speed = map(magnitude, deadzone, 512, 0, MAX_MOUSE_SPEED);
  speed = constrain(speed, 0, MAX_MOUSE_SPEED);

  int direction = (difference > 0) ? 1 : -1;

  if (invert)
  {
    direction *= -1;
  }

  return speed * direction;
}

void updateJoysticks()
{
  float mouseX = 0;
  float mouseY = 0;
  int scroll = 0;

  // Restos fracionarios (permitem velocidades menores que 1 pixel por ciclo)
  static float remainderX = 0;
  static float remainderY = 0;

  // Modo alternativo definido pelas variaveis PIN_XX
  bool scrollMode1 = isActionPressed(ALT_LEFT_ACTION);
  bool scrollMode2 = isActionPressed(ALT_RIGHT_ACTION);

  // JOYSTICK 1
  if (scrollMode1)
  {
    accel1.active = false;

    scroll += updateAnalogScroll(
        JOY1_Y, CENTER_Y1, SCROLL_MODE_DEADZONE, INVERT_Y1, lastAnalogScroll1);
  }
  else
  {
    int speedX1 = calculateMouseSpeed(
        analogRead(JOY1_X), CENTER_X1, DEADZONE_X1, INVERT_X1);

    int speedY1 = calculateMouseSpeed(
        analogRead(JOY1_Y), CENTER_Y1, DEADZONE_Y1, INVERT_Y1);

    float mult1 = updateAcceleration(accel1, (speedX1 != 0 || speedY1 != 0));

    mouseX += speedX1 * mult1;
    mouseY += speedY1 * mult1;
  }

  // JOYSTICK 2
  if (scrollMode2)
  {
    accel2.active = false;

    scroll += updateAnalogScroll(
        JOY2_Y, CENTER_Y2, SCROLL_MODE_DEADZONE, INVERT_Y2, lastAnalogScroll2);
  }
  else
  {
    int speedX2 = calculateMouseSpeed(
        analogRead(JOY2_X), CENTER_X2, DEADZONE_X2, INVERT_X2);

    int speedY2 = calculateMouseSpeed(
        analogRead(JOY2_Y), CENTER_Y2, DEADZONE_Y2, INVERT_Y2);

    float mult2 = updateAcceleration(accel2, (speedX2 != 0 || speedY2 != 0));

    mouseX += speedX2 * mult2;
    mouseY += speedY2 * mult2;
  }

  // Limita a velocidade combinada dos dois joysticks
  mouseX = constrain(mouseX, -MAX_MOUSE_SPEED, MAX_MOUSE_SPEED);
  mouseY = constrain(mouseY, -MAX_MOUSE_SPEED, MAX_MOUSE_SPEED);

  // Se nao ha movimento, descarta os restos para evitar deriva
  if (mouseX == 0 && mouseY == 0)
  {
    remainderX = 0;
    remainderY = 0;
  }

  // Soma o resto anterior e envia apenas a parte inteira
  remainderX += mouseX;
  remainderY += mouseY;

  int moveX = (int)remainderX;
  int moveY = (int)remainderY;

  remainderX -= moveX;
  remainderY -= moveY;

  Mouse.move(moveX, moveY, scroll);
}
