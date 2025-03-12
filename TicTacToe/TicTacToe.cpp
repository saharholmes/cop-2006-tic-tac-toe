#include <iostream>
#include "raylib.h"

int main()
{
    InitWindow(600, 400, "Tic Tac Toe");
    while (!WindowShouldClose()) {
        BeginDrawing();
        Color brown = GetColor(0x7b4321e8);
        ClearBackground(brown);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
