#include "Header.h"
#include "CUI.h"
#include "GUI.h"

DWORD simulate_time;

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; 
    SetConsoleCursorInfo(out, &cursorInfo);
}

void UpdateTime(double time)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(out, {0, 0});
    printf("Elapsed Time: %.3lf seconds", time);
}

BOOL InitArduinoBoard(char* arduino_board_config, bool cui, bool gui)
{
    ARDUINO_BOARD data;
    ARDUINO_APP app;
    HMODULE arduino_main;

    system("cls");

    data.board_name = L"Arduino Uno";
    data.simulation_seconds = 5;
    data.digital_pins = 14;
    data.analog_pins = 6;
    for (int i = 0; i < data.digital_pins; i++)
    {
        data.pio.digital_mode[i] = TIP_UNDEFINED;
        data.pio.valori_pini_digital[i] = LOW;
    }
    for (int i = 0; i < data.analog_pins; i++)
    {
        data.pio.analog_mode[i] = TIP_UNDEFINED;
        data.pio.valori_pini_analog[i] = LOW;
    }
    simulate_time = data.simulation_seconds;

    arduino_main = LoadLibrary(L"ArduinoApp.dll");
    if (!arduino_main)
        printf("Unable to find [ArduinoApp.dll] module\n");
    app.f_loop = (F_LOOP)GetProcAddress(arduino_main, "loop");
    app.f_setup = (F_SETUP)GetProcAddress(arduino_main, "setup");

    UseBoard(&data, &app);

    if (cui)
    {
        InitCUIBoard(30, 7);
        RegisterGraphicIOHook(CUIIOHook);
    }
    else
        if (gui)
        {
            InitGUIBoard(30, 7);
            RegisterGraphicIOHook(GUIIOHook);
        }

    return TRUE;
}

int main(int argc, char* argv[])
{    
    char* arduino_board = 0;
    LARGE_INTEGER start, stop, freq;
    double time;
    int cui = 0;
    int gui = 0;

    ShowConsoleCursor(false);
    
    QueryPerformanceFrequency(&freq);

    if (argc < 2)
    {
        printf("You must provide the path to the Arduino Board configuration json!!!\n");
        printf("EXAMPLE CMDLINE: \n");
        printf("%s board.json\n", PathFindFileNameA(argv[0]));
        return 0;
    }
    else
    {
        arduino_board = argv[1];

        if (argc == 3)
        {
            if (!strncmp(argv[2], "-cui=", 5))
                cui = atoi(argv[2] + 5);
            else
                if (!strncmp(argv[2], "-gui=", 5))
                    gui = atoi(argv[2] + 5);
        }
    }

    if (!InitArduinoBoard(arduino_board, cui, gui))
    {
        printf("Unable to initialize the Arduino Board!!!\n");
        return 0;
    }

    QueryPerformanceCounter(&start);
    library_setup();
    while (true)
    {
        QueryPerformanceCounter(&stop);
        time = (double)(stop.QuadPart - start.QuadPart) / freq.QuadPart;
        if (time > simulate_time)
            break;
        UpdateTime(time);
        library_loop();
    }

    if (gui)
        FinishGUI();

    return 0;
}

