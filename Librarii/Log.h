#pragma once

#include "Arduino.h"

class Log {
private:
    Log();
    ~Log();

    static Log* instanta;
    wstring fname;
public:
    static Log* GetInstance();
    void DestroyInstance();
    void DebugMsg(const char* format, ...);
    void DebugMsgTime(const char* format, ...);
    void ChangeLogName(const wchar_t* log);
    void RemovePrevious();
};
