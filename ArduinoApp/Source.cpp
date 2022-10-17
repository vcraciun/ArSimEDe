#include "Header.h"

int T = 6000;
typedef void (*LED_N)();

void Pulse(int pin, int percent, int T)
{
    float _1_ref = T * (percent / 100.0);
    float _passed = (micros() % T) * 1.0;
    if (_passed < _1_ref)
        digitalWrite(pin, HIGH);
    else
        digitalWrite(pin, LOW);
}

void led_8()
{
    if ((millis() / 500) % 2 == 0)
        Pulse(8, (millis() / 5) % 100, T);
    else
        Pulse(8, 100 - (millis() / 5) % 100, T);
}

void led_12()
{
    if ((millis() / 1000) % 2 == 0)
        Pulse(12, (millis() / 10) % 100, T);
    else
        Pulse(12, 100 - (millis() / 10) % 100, T);
}

void led_13()
{
    if ((millis() / 2000) % 2 == 0)
        Pulse(13, (millis() / 20) % 100, T);
    else
        Pulse(13, 100 - (millis() / 20) % 100, T);
}

void setup()
{
    pinMode(8, TIP_OUTPUT);
    pinMode(12, TIP_OUTPUT);
    pinMode(13, TIP_OUTPUT);
    digitalWrite(8, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
}

void loop()
{
    int i;
    LED_N f_leds[3] = { led_13, led_12, led_8 };
    for (i = 0; i < 3; i++)
        f_leds[i]();
}

BOOL WINAPI DllMain(HINSTANCE module_handle, DWORD reason_for_call, LPVOID reserved) {
    if (reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module_handle);
    }
    return TRUE;
}

