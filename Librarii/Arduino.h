#pragma once
#include <stdio.h>
#include <iostream>
#include <map>
#include <windows.h>
#include <string>
#include <vector>
using namespace std;

#pragma warning (disable:4996)

#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)

#ifdef IS_ARDUINO_LIBRARY
#define DLL_LINK DLL_EXPORT
#else
#define DLL_LINK DLL_IMPORT
#endif

typedef void (*F_SETUP)();
typedef void (*F_LOOP)();
typedef void (*F_IOHOOK)(int, int, bool);

enum tipuri_pin {
	TIP_INPUT,
	TIP_OUTPUT,
    TIP_UNDEFINED
};

enum semnale_iesire {
	LOW,
	HIGH
};

enum placi_dezvoltare {
	ARDUINO_UNO,
	MEGA2560,
	LEONARDO,
	NANO
};

struct IO_HOOK_PARAM
{
    map<int, int> digital_mode;
    map<int, int> analog_mode;
    map<int, int> valori_pini_digital;
    map<int, int> valori_pini_analog;
};

struct ARDUINO_BOARD
{
    wstring board_name;
    IO_HOOK_PARAM pio;
    int digital_pins;
    int analog_pins;
    DWORD simulation_seconds;
};

struct ARDUINO_APP
{
    F_SETUP f_setup;
    F_LOOP f_loop;
};

void DLL_LINK pinMode(int pin, int tip_pin);
void DLL_LINK digitalWrite(int pin, int semnal_iesire);
void DLL_LINK analogWrite(int pin, int semnal_iesire);
long long DLL_LINK millis();
long long DLL_LINK micros();
void DLL_LINK UseBoard(ARDUINO_BOARD* board, ARDUINO_APP* app);
void DLL_LINK library_setup();
void DLL_LINK library_loop();
void DLL_LINK library_finish();
void DLL_LINK RegisterGraphicIOHook(F_IOHOOK f_ptr);
