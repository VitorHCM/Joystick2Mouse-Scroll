#include "Config.h"
#include "DefaultProfile.h"

// =====================================================
// DEFINICAO DAS VARIAVEIS
// Os valores reais sao atribuidos em resetConfigToDefaults(),
// assim existe um unico lugar com os valores de fabrica.
// =====================================================

// Botoes
ButtonAction PIN_02, PIN_03, PIN_04, PIN_05, PIN_06, PIN_07;
ButtonAction PIN_08, PIN_09, PIN_10, PIN_11, PIN_12, PIN_13;

// Joysticks
int JOY1_X, JOY1_Y, JOY2_X, JOY2_Y;
int CENTER_X1, CENTER_Y1, CENTER_X2, CENTER_Y2;
int DEADZONE_X1, DEADZONE_Y1, DEADZONE_X2, DEADZONE_Y2;
int SCROLL_MODE_DEADZONE;
bool INVERT_X1, INVERT_Y1, INVERT_X2, INVERT_Y2;
int MAX_MOUSE_SPEED;

// Aceleracao
bool ENABLE_ACCELERATION;
float ACCEL_START_MULTIPLIER;
float ACCEL_END_MULTIPLIER;
unsigned long ACCEL_RAMP_TIME;

// Scroll
unsigned long SCROLL_INITIAL_DELAY;
unsigned long SCROLL_REPEAT_INTERVAL;
int MIN_SCROLL_INTERVAL;
int MAX_SCROLL_INTERVAL;

// =====================================================
// VALORES DE FABRICA
// =====================================================

void resetConfigToDefaults()
{
  // Botoes
  PIN_02 = C_PIN_02;
  PIN_03 = C_PIN_03;
  PIN_04 = C_PIN_04;
  PIN_05 = C_PIN_05;
  PIN_06 = C_PIN_06;
  PIN_07 = C_PIN_07;
  PIN_08 = C_PIN_08;
  PIN_09 = C_PIN_09;
  PIN_10 = C_PIN_10;
  PIN_11 = C_PIN_11;
  PIN_12 = C_PIN_12;
  PIN_13 = C_PIN_13;

  // Joysticks
  JOY1_X = C_JOY1_X;
  JOY1_Y = C_JOY1_Y;
  JOY2_X = C_JOY2_X;
  JOY2_Y = C_JOY2_Y;

  CENTER_X1 = C_CENTER_X1;
  CENTER_Y1 = C_CENTER_Y1;
  CENTER_X2 = C_CENTER_X2;
  CENTER_Y2 = C_CENTER_Y2;

  DEADZONE_X1 = C_DEADZONE_X1;
  DEADZONE_Y1 = C_DEADZONE_Y1;
  DEADZONE_X2 = C_DEADZONE_X2;
  DEADZONE_Y2 = C_DEADZONE_Y2;
  SCROLL_MODE_DEADZONE = C_SCROLL_MODE_DEADZONE;

  INVERT_X1 = C_INVERT_X1;
  INVERT_Y1 = C_INVERT_Y1;
  INVERT_X2 = C_INVERT_X2;
  INVERT_Y2 = C_INVERT_Y2;

  MAX_MOUSE_SPEED = C_MAX_MOUSE_SPEED;

  // Aceleracao
  ENABLE_ACCELERATION = C_ENABLE_ACCELERATION;
  ACCEL_START_MULTIPLIER = C_ACCEL_START_MULTIPLIER;
  ACCEL_END_MULTIPLIER = C_ACCEL_END_MULTIPLIER;
  ACCEL_RAMP_TIME = C_ACCEL_RAMP_TIME;

  // Scroll
  SCROLL_INITIAL_DELAY = C_SCROLL_INITIAL_DELAY;
  SCROLL_REPEAT_INTERVAL = C_SCROLL_REPEAT_INTERVAL;
  MIN_SCROLL_INTERVAL = C_MIN_SCROLL_INTERVAL;
  MAX_SCROLL_INTERVAL = C_MAX_SCROLL_INTERVAL;
}
