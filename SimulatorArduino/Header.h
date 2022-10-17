#pragma once

#include <windows.h>
#include <Shlwapi.h>
#include <stdio.h>
#include <map>
#include <string>
#include <vector>
#include <Arduino.h>
using namespace std;

BOOL InitArduinoBoard(char* arduino_board_config, bool cui, bool gui);
void ShowConsoleCursor(bool showFlag);
void UpdateTime(double time);

