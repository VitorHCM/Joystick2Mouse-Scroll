#include <Arduino.h>
#include <Mouse.h>
#include "Buttons.h"
#include "config/Config.h"

struct ButtonConfig
{
  int pin;
  ButtonAction action;
};

struct ButtonState
{
  int previousState = HIGH;
  unsigned long pressedAt = 0;
  unsigned long lastRepeat = 0;
};

static const int BUTTON_COUNT = 12;
static ButtonConfig buttons[BUTTON_COUNT];
static ButtonState buttonStates[BUTTON_COUNT];

static uint8_t getMouseButton(ButtonAction action)
{
  switch (action)
  {
  case MOUSE_LEFT_ACTION:
    return MOUSE_LEFT;
  case MOUSE_RIGHT_ACTION:
    return MOUSE_RIGHT;
  case MOUSE_MIDDLE_ACTION:
    return MOUSE_MIDDLE;
  default:
    return 0;
  }
}

void initButtons()
{
  // Passa a funcao configurada para cada um dos pinos
  const ButtonConfig table[BUTTON_COUNT] = {
      {2, PIN_02}, {3, PIN_03}, {4, PIN_04}, {5, PIN_05},
      {6, PIN_06}, {7, PIN_07}, {8, PIN_08}, {9, PIN_09},
      {10, PIN_10}, {11, PIN_11}, {12, PIN_12}, {13, PIN_13}};

  for (int i = 0; i < BUTTON_COUNT; i++)
  {
    buttons[i] = table[i];

    // Configura somente os pinos que tem funcao atribuida
    if (buttons[i].action != NONE_ACTION)
    {
      pinMode(buttons[i].pin, INPUT_PULLUP);
    }
  }
}

bool isActionPressed(ButtonAction action)
{
  for (int i = 0; i < BUTTON_COUNT; i++)
  {
    if (buttons[i].action == action && digitalRead(buttons[i].pin) == LOW)
    {
      return true;
    }
  }
  return false;
}

int updateButtons()
{
  unsigned long currentTime = millis();
  int scroll = 0;

  for (int i = 0; i < BUTTON_COUNT; i++)
  {
    ButtonAction action = buttons[i].action;

    // Botoes desativados e de modo alternativo sao tratados em outro lugar
    if (action == NONE_ACTION ||
        action == ALT_LEFT_ACTION ||
        action == ALT_RIGHT_ACTION)
    {
      continue;
    }

    int currentState = digitalRead(buttons[i].pin);
    ButtonState &state = buttonStates[i];

    // BOTOES DE CLIQUE
    if (action == MOUSE_LEFT_ACTION ||
        action == MOUSE_RIGHT_ACTION ||
        action == MOUSE_MIDDLE_ACTION)
    {
      uint8_t mouseButton = getMouseButton(action);

      if (currentState == LOW && state.previousState == HIGH)
      {
        Mouse.press(mouseButton);
      }
      else if (currentState == HIGH && state.previousState == LOW)
      {
        Mouse.release(mouseButton);
      }
    }
    // BOTOES DE SCROLL
    else
    {
      int direction = (action == SCROLL_UP_ACTION) ? 1 : -1;

      // Primeiro pressionamento
      if (currentState == LOW && state.previousState == HIGH)
      {
        scroll += direction;
        state.pressedAt = currentTime;
        state.lastRepeat = currentTime;
      }
      // Repeticao automatica
      else if (currentState == LOW)
      {
        if (currentTime - state.pressedAt >= SCROLL_INITIAL_DELAY &&
            currentTime - state.lastRepeat >= SCROLL_REPEAT_INTERVAL)
        {
          scroll += direction;
          state.lastRepeat = currentTime;
        }
      }
    }

    state.previousState = currentState;
  }

  return scroll;
}
