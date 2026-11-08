#pragma once

struct AccelState
{
  bool active = false;
  unsigned long startTime = 0;
};

// Retorna o multiplicador de velocidade atual (rampa suave ao sair do centro)
float updateAcceleration(AccelState &state, bool moving);
