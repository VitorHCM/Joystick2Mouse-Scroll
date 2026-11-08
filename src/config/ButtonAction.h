#pragma once

// Funcoes possiveis para os botoes
enum ButtonAction
{
  NONE_ACTION,          // botao desativado

  MOUSE_LEFT_ACTION,    // clique esquerdo
  MOUSE_RIGHT_ACTION,   // clique direito
  MOUSE_MIDDLE_ACTION,  // clique do meio

  SCROLL_UP_ACTION,     // scroll para cima (botao)
  SCROLL_DOWN_ACTION,   // scroll para baixo (botao)

  ALT_LEFT_ACTION,      // modo alternativo: joystick 1 vira scroll
  ALT_RIGHT_ACTION      // modo alternativo: joystick 2 vira scroll
};
