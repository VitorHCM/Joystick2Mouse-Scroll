#pragma once
#include "config/ButtonAction.h"

// Monta a tabela de botoes a partir do profile e configura os pinos
void initButtons();

// Le os botoes, envia cliques e retorna o scroll gerado
int updateButtons();

// true se QUALQUER botao configurado com essa funcao estiver pressionado
bool isActionPressed(ButtonAction action);
