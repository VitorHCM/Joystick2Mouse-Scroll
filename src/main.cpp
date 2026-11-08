#include <Arduino.h>
#include <Mouse.h>
#include "config/Config.h"
#include "buttons/Buttons.h"
#include "joystick/Joystick.h"
#include "remote/ConfigRemote.h"

void setup()
{
  resetConfigToDefaults();  // 1) valores de fabrica (DefaultProfile.h)
  loadConfigFromEEPROM();   // 2) sobrescreve com o que foi salvo pelo programa do PC
  initButtons();
  Mouse.begin();
  initConfigRemote();
}

void loop()
{
  // Atualiza os botoes e captura os inputs de scroll
  int buttonScroll = updateButtons();

  // Atualiza os joysticks
  updateJoysticks();

  // Aplica scroll dos botoes
  if (buttonScroll != 0)
  {
    Mouse.move(0, 0, buttonScroll);
  }

  // Processa comandos vindos do programa de configuracao
  updateConfigRemote();

  delay(5);
}
