#include "Arduino.h"
#include "Log.h"

ARDUINO_BOARD selected_board;
ARDUINO_APP loaded_app;
vector<F_IOHOOK> io_hooks;

void pinMode(int pin, int tip_pin)
{
    //pinul exista pe placa
	selected_board.pio.digital_mode[pin] = tip_pin;
}

void digitalWrite(int pin, int semnal_iesire)
{
    //pinul are modul corect
    for (auto it : io_hooks)
        it(pin, semnal_iesire, true);
    Log::GetInstance()->DebugMsg(" %02d --> %d", pin, semnal_iesire);
}

void analogWrite(int pin, int semnal_iesire)
{
    for (auto it : io_hooks)
        it(pin, semnal_iesire, false);
    Log::GetInstance()->DebugMsg(" %02d --> %d", pin, semnal_iesire);
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
        milis += (unsigned long long)timp.wDay * 24 * 3600 * 1000;
        diff = 0;
    }
    else
    {
        diff = (unsigned long long)timp.wMilliseconds;
        diff += (unsigned long long)timp.wSecond * 1000;
        diff += (unsigned long long)timp.wMinute * 60 * 1000;
        diff += (unsigned long long)timp.wHour * 3600 * 1000;
        diff += (unsigned long long)timp.wDay * 24 * 3600 * 1000;
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

    Log::GetInstance()->DebugMsg("Using board: [%S]", selected_board.board_name.c_str());
    Log::GetInstance()->DebugMsg("Simulation time: [%d] seconds", selected_board.simulation_seconds);

    loaded_app.f_setup = app->f_setup;
    loaded_app.f_loop = app->f_loop;
}

void library_setup()
{
    Log::GetInstance()->DebugMsg("setup()");
    loaded_app.f_setup();
}

void library_loop()
{
    Log::GetInstance()->DebugMsg("loop()");
    loaded_app.f_loop();
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
        Log::GetInstance()->DebugMsg("Start Simulation");
        DisableThreadLibraryCalls(module_handle);
    }
    if (reason_for_call == DLL_PROCESS_DETACH)
    {
        Log::GetInstance()->DebugMsg("Stop Simulation");
        Log::GetInstance()->DestroyInstance();
    }
    return TRUE;
}

