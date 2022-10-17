#pragma once

#include <Arduino.h>

typedef void (*LED_N)();

void Pulse(int pin, int percent, int T);
void led_8();
void led_12();
void led_13();

extern "C" {
    DLL_EXPORT void setup();
    DLL_EXPORT void loop();
};

