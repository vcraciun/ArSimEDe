#include "Log.h"

Log* Log::instanta = 0;

Log::Log()
{
    fname = L"";
}

Log::~Log()
{
    fname = L"";    
}

void Log::DestroyInstance()
{
    delete instanta;
    instanta = 0;
}

Log* Log::GetInstance()
{
    if (!instanta)
        instanta = new Log();
    return instanta;
}

void Log::DebugMsg(const char* format, ...)
{
    char data[1024];
    char formated[256];
    HANDLE f = 0;
    va_list argptr;

    va_start(argptr, format);
    vsprintf(formated, format, argptr);
    va_end(argptr);

    sprintf(data, "%s", formated);

    f = CreateFile(fname.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (f == INVALID_HANDLE_VALUE)
    {
        f = CreateFile(fname.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (f == INVALID_HANDLE_VALUE)
            return;
    }

    DWORD size = GetFileSize(f, 0);
    DWORD write;
    SetFilePointer(f, size, 0, FILE_BEGIN);
    if (!WriteFile(f, data, (DWORD)strlen(data), &write, NULL))
    {
        printf("FAILED to write to [%S]\n", fname.c_str());
        CloseHandle(f);
        return;
    }
    CloseHandle(f);    
}

void Log::DebugMsgTime(const char* format, ...)
{
    SYSTEMTIME time;
    char data[1024];
    char formated[256];
    HANDLE f = 0;
    va_list argptr;

    va_start(argptr, format);
    vsprintf(formated, format, argptr);
    va_end(argptr);

    GetLocalTime(&time);
    sprintf(data, "[%02d:%02d:%02d.%03d]    %s\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, formated);

    f = CreateFile(fname.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (f == INVALID_HANDLE_VALUE)
    {
        f = CreateFile(fname.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (f == INVALID_HANDLE_VALUE)
            return;
    }

    DWORD size = GetFileSize(f, 0);
    DWORD write;
    SetFilePointer(f, size, 0, FILE_BEGIN);
    if (!WriteFile(f, data, (DWORD)strlen(data), &write, NULL))
    {
        printf("FAILED to write to [%S]\n", fname.c_str());
        CloseHandle(f);
        return;
    }
    CloseHandle(f);
}

void Log::ChangeLogName(const wchar_t* log)
{
    wchar_t temp[MAX_PATH];
    GetFullPathName(log, MAX_PATH, temp, NULL);
    fname = temp;
}

void Log::RemovePrevious()
{
    if (!fname.empty())
        DeleteFile(fname.c_str());
}

