#pragma once

extern const char board[][64];

void InitCUIBoard(SHORT x, SHORT y);
void CUIIOHook(int pin, int value, bool digital);

