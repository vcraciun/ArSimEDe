#pragma once

#include "SDL.h"
#undef main

void InitGUIBoard(SHORT x, SHORT y);
void GUIIOHook(int pin, int value, bool digital);
void FinishGUI();

