#pragma once
#include <Arduino.h>
#include "ButtonAction.h"

// =====================================================
// PROFILE PADRAO (valores de fabrica, somente leitura)
// =====================================================

// LAYOUT PADRAO DOS BOTOES
/*
(02) (03) (04) (05) (06)  (07) (08) (09) (10) (11)
(13)                                          (12)
*/

// ---------------- Botoes (pinos 2 a 13) ----------------
const ButtonAction C_PIN_02 = SCROLL_DOWN_ACTION;
const ButtonAction C_PIN_03 = SCROLL_UP_ACTION;
const ButtonAction C_PIN_04 = MOUSE_RIGHT_ACTION;
const ButtonAction C_PIN_05 = MOUSE_LEFT_ACTION;
const ButtonAction C_PIN_06 = ALT_LEFT_ACTION;
const ButtonAction C_PIN_07 = ALT_RIGHT_ACTION;
const ButtonAction C_PIN_08 = MOUSE_LEFT_ACTION;
const ButtonAction C_PIN_09 = MOUSE_RIGHT_ACTION;
const ButtonAction C_PIN_10 = SCROLL_DOWN_ACTION;
const ButtonAction C_PIN_11 = SCROLL_UP_ACTION;
const ButtonAction C_PIN_12 = MOUSE_MIDDLE_ACTION; // botao do joystick 2
const ButtonAction C_PIN_13 = MOUSE_MIDDLE_ACTION; // botao do joystick 1

// ---------------- Joysticks ----------------
// Joystick 1 - LEFT
const int C_JOY1_X = A0;
const int C_JOY1_Y = A1;

// Joystick 2 - RIGHT
const int C_JOY2_X = A2;
const int C_JOY2_Y = A3;

// Centros dos eixos
const int C_CENTER_X1 = 512;
const int C_CENTER_Y1 = 512;
const int C_CENTER_X2 = 512;
const int C_CENTER_Y2 = 512;

// Deadzones independentes
const int C_DEADZONE_X1 = 50;
const int C_DEADZONE_Y1 = 50;
const int C_DEADZONE_X2 = 50;
const int C_DEADZONE_Y2 = 50;

// Deadzone usada quando o joystick esta em modo scroll
const int C_SCROLL_MODE_DEADZONE = 120;

// Inversao independente dos eixos (true = invertido)
const bool C_INVERT_X1 = false;
const bool C_INVERT_Y1 = false;
const bool C_INVERT_X2 = false;
const bool C_INVERT_Y2 = false;

// Velocidade maxima do cursor
const int C_MAX_MOUSE_SPEED = 12;

// ---------------- Aceleracao ----------------
// true = aceleracao ligada | false = comportamento linear original
const bool C_ENABLE_ACCELERATION = true;

// Multiplicador de velocidade quando o joystick sai do centro (0.0 a 1.0)
const float C_ACCEL_START_MULTIPLIER = 0.05;

// Multiplicador final (1.0 = velocidade normal do joystick)
const float C_ACCEL_END_MULTIPLIER = 1.2;

// Tempo (ms) para ir do multiplicador inicial ao final
const unsigned long C_ACCEL_RAMP_TIME = 1000;

// ---------------- Scroll ----------------
// Scroll por botoes
const unsigned long C_SCROLL_INITIAL_DELAY = 300;
const unsigned long C_SCROLL_REPEAT_INTERVAL = 60;

// Scroll analogico
const int C_MIN_SCROLL_INTERVAL = 35;
const int C_MAX_SCROLL_INTERVAL = 250;
