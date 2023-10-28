#include "Header.h"
#include "gui.h"

SDL_Window* gui_window = NULL;

void InitGUIBoard(SHORT x, SHORT y)
{    
    SDL_Surface* screenSurface = NULL;
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    else
    {
        gui_window = SDL_CreateWindow( "ArSIMEDe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_SHOWN );
        if (gui_window == 0)
            return;
        screenSurface = SDL_GetWindowSurface( gui_window );
        SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
        SDL_Surface* arduino_board = SDL_LoadBMP("arduino.bmp");
        SDL_BlitSurface( arduino_board, NULL, screenSurface, NULL );
    }
}

void GUIIOHook(int pin, int value, bool digital)
{
    SDL_Event e; 

    if ( SDL_PollEvent( &e ) )
    { 
        if (e.type == SDL_QUIT)
            return;
    }

    SDL_UpdateWindowSurface( gui_window );

    /*static COORD pin_pos_digital[15] = {{49, 1}, {47, 1}, {45, 1}, {43, 1}, {41, 1}, {39, 1}, {37, 1}, {35, 1}, {31, 1}, {29, 1}, {27, 1}, {25, 1}, {23, 1}, {21, 1}, {19, 1}};
    static COORD pin_pos_analog[6] = { {49, 13}, {47, 13}, {45, 13}, {43, 13}, {41, 13}, {39, 13} };
    HANDLE std = GetStdHandle(STD_OUTPUT_HANDLE);

    if (digital)
        SetConsoleCursorPosition(std, { xx + pin_pos_digital[pin].X, yy + pin_pos_digital[pin].Y });
    else
        SetConsoleCursorPosition(std, { xx + pin_pos_analog[pin].X, yy + pin_pos_analog[pin].Y });

    if (value)
        SetConsoleTextAttribute(std, 0x0C);
    else
        SetConsoleTextAttribute(std, 0x07);

    printf("\xFE");

    SetConsoleTextAttribute(std, 0x07);*/
}

void FinishGUI()
{
    SDL_DestroyWindow( gui_window );
    SDL_Quit();    
}