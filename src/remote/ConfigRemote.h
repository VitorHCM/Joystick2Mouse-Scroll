#pragma once

// Abre a porta serial (USB) usada pelo programa de configuracao do PC
void initConfigRemote();

// Chamar a cada volta do loop(): le e executa comandos recebidos
void updateConfigRemote();

// Carrega da EEPROM os valores salvos pelo comando SAVE.
// Se nao houver nada valido salvo, mantem os valores atuais.
// Retorna true se carregou.
bool loadConfigFromEEPROM();
