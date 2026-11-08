#pragma once
#include <Arduino.h>
#include "ButtonAction.h"

// =====================================================
// PROFILE CONFIGURAVEL
// Estas sao as variaveis usadas pelo codigo. Comecam com os
// valores de DefaultProfile.h e podem ser alteradas em runtime.
// (extern = declaradas aqui, definidas em Config.cpp)
// =====================================================

// Botoes
extern ButtonAction PIN_02, PIN_03, PIN_04, PIN_05, PIN_06, PIN_07;
extern ButtonAction PIN_08, PIN_09, PIN_10, PIN_11;
extern ButtonAction PIN_12; // botao do joystick 2
extern ButtonAction PIN_13; // botao do joystick 1

// Joysticks
extern int JOY1_X, JOY1_Y, JOY2_X, JOY2_Y;
extern int CENTER_X1, CENTER_Y1, CENTER_X2, CENTER_Y2;
extern int DEADZONE_X1, DEADZONE_Y1, DEADZONE_X2, DEADZONE_Y2;
extern int SCROLL_MODE_DEADZONE;
extern bool INVERT_X1, INVERT_Y1, INVERT_X2, INVERT_Y2;
extern int MAX_MOUSE_SPEED;

// Aceleracao
extern bool ENABLE_ACCELERATION;
extern float ACCEL_START_MULTIPLIER;
extern float ACCEL_END_MULTIPLIER;
extern unsigned long ACCEL_RAMP_TIME;

// Scroll
extern unsigned long SCROLL_INITIAL_DELAY;
extern unsigned long SCROLL_REPEAT_INTERVAL;
extern int MIN_SCROLL_INTERVAL;
extern int MAX_SCROLL_INTERVAL;

// Preenche todas as variaveis acima com os valores de DefaultProfile.h.
// Deve ser chamada uma vez no inicio do setup().
void resetConfigToDefaults();
