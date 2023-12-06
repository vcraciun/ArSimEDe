#include "Arduino.h"
#include "Log.h"

ARDUINO_BOARD selected_board;
ARDUINO_APP loaded_app;
vector<F_IOHOOK> io_hooks;
map<DWORD, DWORD> pin_trace;
map<BYTE, BYTE> pin_state;

void pinMode(int pin, int tip_pin)
{
    //pinul exista pe placa
	selected_board.pio.digital_mode[pin] = tip_pin;
    pin_state[pin] = 0;
}

void digitalWrite(int pin, int semnal_iesire)
{    
    SYSTEMTIME time;
    GetLocalTime(&time);

    DWORD ctime = time.wHour * 3600 * 1000 + time.wMinute * 60000 + time.wSecond * 1000 + time.wMilliseconds;
    if (pin_state.find(pin) == pin_state.end())
    {
        Log::GetInstance()->DebugMsgTime(" PIN [%d] was not initialized as OUTPUT", pin);
        return;
    }

    pin_state[pin] = semnal_iesire;
    DWORD data = 0;
    for (auto it : pin_state)
        data = data | (it.second << it.first);
    pin_trace[ctime] = data;

    //pinul are modul corect
    for (auto it : io_hooks)
        it(pin, semnal_iesire, true);
    //Log::GetInstance()->DebugMsg(" %02d --> %d", pin, semnal_iesire);
}

void analogWrite(int pin, int semnal_iesire)
{
    SYSTEMTIME time;
    GetLocalTime(&time);

    for (auto it : io_hooks)
        it(pin, semnal_iesire, false);
    //Log::GetInstance()->DebugMsg(" %02d --> %d", pin, semnal_iesire);
}

long long millis()
{
	static unsigned long long milis = 0;
    unsigned long long diff;
	SYSTEMTIME timp;
	GetLocalTime(&timp);

    if (!milis)
    {
        milis = (unsigned long long)timp.wMilliseconds;
        milis += (unsigned long long)timp.wSecond * 1000;
        milis += (unsigned long long)timp.wMinute * 60 * 1000;
        milis += (unsigned long long)timp.wHour * 3600 * 1000;
        diff = 0;
    }
    else
    {
        diff = (unsigned long long)timp.wMilliseconds;
        diff += (unsigned long long)timp.wSecond * 1000;
        diff += (unsigned long long)timp.wMinute * 60 * 1000;
        diff += (unsigned long long)timp.wHour * 3600 * 1000;
        diff -= milis;
    }
	return diff;
}

long long micros()
{
	FILETIME ft;
	long long micros;

	GetSystemTimePreciseAsFileTime(&ft);
	micros = ft.dwHighDateTime;
	micros <<= 32;
	micros |= ft.dwLowDateTime;
	return micros;
}

void UseBoard(ARDUINO_BOARD* board, ARDUINO_APP* app)
{
    selected_board.analog_pins = board->analog_pins;
    selected_board.board_name = board->board_name;
    selected_board.digital_pins = board->digital_pins;
    selected_board.simulation_seconds = board->simulation_seconds;
    selected_board.pio = board->pio;

    Log::GetInstance()->DebugMsgTime("Using board: [%S]", selected_board.board_name.c_str());
    Log::GetInstance()->DebugMsgTime("Simulation time: [%d] seconds", selected_board.simulation_seconds);

    loaded_app.f_setup = app->f_setup;
    loaded_app.f_loop = app->f_loop;
}

void library_setup()
{
    Log::GetInstance()->DebugMsgTime("setup()");
    loaded_app.f_setup();
}

void library_loop()
{
    Log::GetInstance()->DebugMsgTime("loop()");
    loaded_app.f_loop();
}

void library_finish()
{    
    DWORD write;
    char* buffer = (char*)malloc(0x500000);
    memset(buffer, 0, 0x500000);
    HANDLE f = CreateFileA("main.json", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    sprintf(buffer + strlen(buffer), "[\n");
    int i = 0;
    for (auto it = pin_trace.begin(); it != pin_trace.end(); it++)
    {
        int hh = it->first / (1000 * 3600);
        int mm = (it->first % (1000 * 3600)) / (60 * 1000);
        int ss = (it->first % (1000 * 3600)) % 60;
        int ms = it->first % 1000;

        if (it == pin_trace.begin())
            sprintf(buffer + strlen(buffer), "[");        
        else
            sprintf(buffer + strlen(buffer), ",\n[");

        for (auto it2 = pin_state.begin(); it2 != pin_state.end(); it2++)
        {
            if (it2 == pin_state.begin())
                sprintf(buffer + strlen(buffer), "[%d,%d]", it2->first, (it->second >> it2->first) & 1);
            else
                sprintf(buffer + strlen(buffer), ",[%d,%d]", it2->first, (it->second >> it2->first) & 1);
        }

        sprintf(buffer + strlen(buffer), "]");
    }
    sprintf(buffer + strlen(buffer), "\n]");

    WriteFile(f, buffer, strlen(buffer), &write, 0);
    CloseHandle(f);
}

void RegisterGraphicIOHook(F_IOHOOK f_ptr)
{
    io_hooks.push_back(f_ptr);
}

//------------------------------------------------------------------------------------------

BOOL WINAPI DllMain(HINSTANCE module_handle, DWORD reason_for_call, LPVOID reserved) {
    if (reason_for_call == DLL_PROCESS_ATTACH) 
    {
        Log::GetInstance()->ChangeLogName(L"Arduino.log");        
        Log::GetInstance()->RemovePrevious();
        Log::GetInstance()->DebugMsgTime("Start Simulation");
        DisableThreadLibraryCalls(module_handle);
    }
    if (reason_for_call == DLL_PROCESS_DETACH)
    {
        Log::GetInstance()->DebugMsgTime("Stop Simulation");
        Log::GetInstance()->DestroyInstance();
    }
    return TRUE;
}

