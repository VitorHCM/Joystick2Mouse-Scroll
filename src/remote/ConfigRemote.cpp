#include <Arduino.h>
#include <Mouse.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>

#include "ConfigRemote.h"
#include "config/Config.h"
#include "buttons/Buttons.h"

// =====================================================
// PROTOCOLO (texto, uma linha por comando, terminada em \n)
//
//   PING            -> OK
//   GET             -> NOME=VALOR (uma linha por variavel) + OK
//   SET NOME VALOR  -> OK  ou  ERR mensagem
//   SAVE            -> grava os valores atuais na EEPROM -> OK
//   DEFAULTS        -> volta aos valores de fabrica (nao apaga a EEPROM) -> OK
//
// Valores: numeros; bool = 0/1; acao = numero do enum ButtonAction.
// =====================================================

// -----------------------------------------------------
// TABELA DE VARIAVEIS EXPOSTAS
//
// PARA EXPOR UMA NOVA VARIAVEL:
//   1) declare-a em Config.h / Config.cpp / resetConfigToDefaults()
//   2) adicione uma linha em PARAM_LIST abaixo:
//        X(NOME, TIPO, MINIMO, MAXIMO)
//   3) aumente PARAMS_VERSION (invalida o que ficou salvo na EEPROM)
//   4) adicione o mesmo NOME no programa do PC (tools/joystick_config.py)
//
// MINIMO e MAXIMO sao inteiros e valem para qualquer tipo (protecao contra
// valores absurdos).
// -----------------------------------------------------

enum ParamType : uint8_t
{
  T_INT,    // int
  T_BOOL,   // bool
  T_FLOAT,  // float
  T_ULONG,  // unsigned long
  T_ACTION  // ButtonAction (enum)
};

#define PARAMS_VERSION 1

#define PARAM_LIST(X)                          \
  X(PIN_02, T_ACTION, 0, 7)                    \
  X(PIN_03, T_ACTION, 0, 7)                    \
  X(PIN_04, T_ACTION, 0, 7)                    \
  X(PIN_05, T_ACTION, 0, 7)                    \
  X(PIN_06, T_ACTION, 0, 7)                    \
  X(PIN_07, T_ACTION, 0, 7)                    \
  X(PIN_08, T_ACTION, 0, 7)                    \
  X(PIN_09, T_ACTION, 0, 7)                    \
  X(PIN_10, T_ACTION, 0, 7)                    \
  X(PIN_11, T_ACTION, 0, 7)                    \
  X(PIN_12, T_ACTION, 0, 7)                    \
  X(PIN_13, T_ACTION, 0, 7)                    \
  X(JOY1_X, T_INT, 0, 255)                     \
  X(JOY1_Y, T_INT, 0, 255)                     \
  X(JOY2_X, T_INT, 0, 255)                     \
  X(JOY2_Y, T_INT, 0, 255)                     \
  X(CENTER_X1, T_INT, 0, 1023)                 \
  X(CENTER_Y1, T_INT, 0, 1023)                 \
  X(CENTER_X2, T_INT, 0, 1023)                 \
  X(CENTER_Y2, T_INT, 0, 1023)                 \
  X(DEADZONE_X1, T_INT, 0, 500)                \
  X(DEADZONE_Y1, T_INT, 0, 500)                \
  X(DEADZONE_X2, T_INT, 0, 500)                \
  X(DEADZONE_Y2, T_INT, 0, 500)                \
  X(SCROLL_MODE_DEADZONE, T_INT, 0, 500)       \
  X(INVERT_X1, T_BOOL, 0, 1)                   \
  X(INVERT_Y1, T_BOOL, 0, 1)                   \
  X(INVERT_X2, T_BOOL, 0, 1)                   \
  X(INVERT_Y2, T_BOOL, 0, 1)                   \
  X(MAX_MOUSE_SPEED, T_INT, 1, 127)            \
  X(ENABLE_ACCELERATION, T_BOOL, 0, 1)         \
  X(ACCEL_START_MULTIPLIER, T_FLOAT, 0, 1)     \
  X(ACCEL_END_MULTIPLIER, T_FLOAT, 0, 10)      \
  X(ACCEL_RAMP_TIME, T_ULONG, 1, 30000)        \
  X(SCROLL_INITIAL_DELAY, T_ULONG, 0, 5000)    \
  X(SCROLL_REPEAT_INTERVAL, T_ULONG, 10, 5000) \
  X(MIN_SCROLL_INTERVAL, T_INT, 5, 1000)       \
  X(MAX_SCROLL_INTERVAL, T_INT, 5, 2000)

struct Param
{
  const char *name; // texto guardado em flash (PROGMEM)
  void *ptr;        // endereco da variavel
  uint8_t type;
  int16_t lo;
  int16_t hi;
};

// Nomes em flash (economiza RAM)
#define X_NAME(id, type, lo, hi) static const char name_##id[] PROGMEM = #id;
PARAM_LIST(X_NAME)

// Tabela em flash
#define X_ROW(id, type, lo, hi) {name_##id, (void *)&id, type, lo, hi},
static const Param params[] PROGMEM = {PARAM_LIST(X_ROW)};

static const uint8_t PARAM_COUNT = sizeof(params) / sizeof(params[0]);

static void getParam(uint8_t index, Param &p)
{
  memcpy_P(&p, &params[index], sizeof(Param));
}

static int findParam(const char *name, Param &p)
{
  for (uint8_t i = 0; i < PARAM_COUNT; i++)
  {
    getParam(i, p);
    if (strcmp_P(name, p.name) == 0)
    {
      return i;
    }
  }
  return -1;
}

// -----------------------------------------------------
// LEITURA / ESCRITA DE VALORES
// -----------------------------------------------------

static void printValue(const Param &p)
{
  switch (p.type)
  {
  case T_INT:
    Serial.print(*(int *)p.ptr);
    break;
  case T_BOOL:
    Serial.print(*(bool *)p.ptr ? 1 : 0);
    break;
  case T_FLOAT:
    Serial.print(*(float *)p.ptr, 4);
    break;
  case T_ULONG:
    Serial.print(*(unsigned long *)p.ptr);
    break;
  case T_ACTION:
    Serial.print((int)*(ButtonAction *)p.ptr);
    break;
  }
}

// Converte o texto, valida a faixa e grava na variavel.
static bool setValue(const Param &p, const char *text)
{
  if (text == NULL || *text == '\0')
  {
    return false;
  }

  switch (p.type)
  {
  case T_BOOL:
    *(bool *)p.ptr = (atol(text) != 0);
    return true;

  case T_FLOAT:
  {
    float v = atof(text);
    if (v < p.lo || v > p.hi)
      return false;
    *(float *)p.ptr = v;
    return true;
  }

  default: // T_INT, T_ULONG, T_ACTION
  {
    long v = atol(text);
    if (v < p.lo || v > p.hi)
      return false;

    if (p.type == T_INT)
      *(int *)p.ptr = (int)v;
    else if (p.type == T_ULONG)
      *(unsigned long *)p.ptr = (unsigned long)v;
    else
      *(ButtonAction *)p.ptr = (ButtonAction)v;
    return true;
  }
  }
}

// Depois de mudar botoes, refaz a tabela e solta cliques que ficaram "presos"
static void applyButtonChanges()
{
  initButtons();
  Mouse.release(MOUSE_ALL);
}

// -----------------------------------------------------
// EEPROM
// Formato: [MAGIC][VERSION][COUNT] + valores em sequencia
// -----------------------------------------------------

static const uint8_t EEPROM_MAGIC = 0xA7;
static const int EEPROM_HEADER_SIZE = 3;

static void saveConfigToEEPROM()
{
  int addr = 0;
  EEPROM.put(addr++, EEPROM_MAGIC);
  EEPROM.put(addr++, (uint8_t)PARAMS_VERSION);
  EEPROM.put(addr++, PARAM_COUNT);

  for (uint8_t i = 0; i < PARAM_COUNT; i++)
  {
    Param p;
    getParam(i, p);

    switch (p.type)
    {
    case T_INT:
      EEPROM.put(addr, *(int *)p.ptr);
      addr += sizeof(int);
      break;
    case T_BOOL:
      EEPROM.put(addr++, (uint8_t)(*(bool *)p.ptr ? 1 : 0));
      break;
    case T_FLOAT:
      EEPROM.put(addr, *(float *)p.ptr);
      addr += sizeof(float);
      break;
    case T_ULONG:
      EEPROM.put(addr, *(unsigned long *)p.ptr);
      addr += sizeof(unsigned long);
      break;
    case T_ACTION:
      EEPROM.put(addr++, (uint8_t)*(ButtonAction *)p.ptr);
      break;
    }
  }
}

// Le um valor da EEPROM e valida. Retorna false se estiver fora da faixa.
static bool loadParamFromEEPROM(const Param &p, int &addr)
{
  switch (p.type)
  {
  case T_INT:
  {
    int v;
    EEPROM.get(addr, v);
    addr += sizeof(int);
    if (v < p.lo || v > p.hi)
      return false;
    *(int *)p.ptr = v;
    return true;
  }
  case T_BOOL:
  {
    uint8_t v;
    EEPROM.get(addr++, v);
    if (v > 1)
      return false;
    *(bool *)p.ptr = (v == 1);
    return true;
  }
  case T_FLOAT:
  {
    float v;
    EEPROM.get(addr, v);
    addr += sizeof(float);
    if (!(v >= p.lo && v <= p.hi)) // tambem rejeita NaN
      return false;
    *(float *)p.ptr = v;
    return true;
  }
  case T_ULONG:
  {
    unsigned long v;
    EEPROM.get(addr, v);
    addr += sizeof(unsigned long);
    if ((long)v < p.lo || v > (unsigned long)p.hi)
      return false;
    *(unsigned long *)p.ptr = v;
    return true;
  }
  case T_ACTION:
  {
    uint8_t v;
    EEPROM.get(addr++, v);
    if (v < p.lo || v > p.hi)
      return false;
    *(ButtonAction *)p.ptr = (ButtonAction)v;
    return true;
  }
  }
  return false;
}

bool loadConfigFromEEPROM()
{
  uint8_t magic, version, count;
  EEPROM.get(0, magic);
  EEPROM.get(1, version);
  EEPROM.get(2, count);

  if (magic != EEPROM_MAGIC || version != PARAMS_VERSION || count != PARAM_COUNT)
  {
    return false; // nada salvo (ou salvo por outra versao do firmware)
  }

  int addr = EEPROM_HEADER_SIZE;
  for (uint8_t i = 0; i < PARAM_COUNT; i++)
  {
    Param p;
    getParam(i, p);

    if (!loadParamFromEEPROM(p, addr))
    {
      // Dado corrompido: descarta tudo e volta ao padrao
      resetConfigToDefaults();
      return false;
    }
  }
  return true;
}

// -----------------------------------------------------
// COMANDOS
// -----------------------------------------------------

static void handleCommand(char *line)
{
  if (strcmp_P(line, PSTR("PING")) == 0)
  {
    Serial.println(F("OK"));
  }
  else if (strcmp_P(line, PSTR("GET")) == 0)
  {
    for (uint8_t i = 0; i < PARAM_COUNT; i++)
    {
      Param p;
      getParam(i, p);
      Serial.print((const __FlashStringHelper *)p.name);
      Serial.print('=');
      printValue(p);
      Serial.println();
    }
    Serial.println(F("OK"));
  }
  else if (strncmp_P(line, PSTR("SET "), 4) == 0)
  {
    char *name = strtok(line + 4, " ");
    char *value = strtok(NULL, " ");

    Param p;
    if (name == NULL || findParam(name, p) < 0)
    {
      Serial.println(F("ERR variavel desconhecida"));
    }
    else if (!setValue(p, value))
    {
      Serial.println(F("ERR valor invalido"));
    }
    else
    {
      if (strncmp_P(name, PSTR("PIN_"), 4) == 0)
      {
        applyButtonChanges();
      }
      Serial.println(F("OK"));
    }
  }
  else if (strcmp_P(line, PSTR("SAVE")) == 0)
  {
    saveConfigToEEPROM();
    Serial.println(F("OK"));
  }
  else if (strcmp_P(line, PSTR("DEFAULTS")) == 0)
  {
    resetConfigToDefaults();
    applyButtonChanges();
    Serial.println(F("OK"));
  }
  else
  {
    Serial.println(F("ERR comando desconhecido"));
  }
}

// -----------------------------------------------------
// RECEPCAO SERIAL (sem bloquear o loop)
// -----------------------------------------------------

static char lineBuffer[48];
static uint8_t lineLength = 0;
static bool lineOverflow = false;

void initConfigRemote()
{
  Serial.begin(115200); // na Leonardo/Micro a velocidade e ignorada (USB nativo)
}

void updateConfigRemote()
{
  while (Serial.available() > 0)
  {
    char c = (char)Serial.read();

    if (c == '\n' || c == '\r')
    {
      if (lineLength > 0 && !lineOverflow)
      {
        lineBuffer[lineLength] = '\0';
        handleCommand(lineBuffer);
      }
      else if (lineOverflow)
      {
        Serial.println(F("ERR linha muito longa"));
      }
      lineLength = 0;
      lineOverflow = false;
    }
    else if (lineLength < sizeof(lineBuffer) - 1)
    {
      lineBuffer[lineLength++] = c;
    }
    else
    {
      lineOverflow = true;
    }
  }
}
