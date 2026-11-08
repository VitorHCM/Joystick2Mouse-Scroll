#pragma once

// Converte a posicao do joystick em passos de scroll (-1, 0 ou +1)
int updateAnalogScroll(
    int pin,
    int center,
    int deadzone,
    bool invert,
    unsigned long &lastScrollTime);
