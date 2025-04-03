#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include "raylib.h"
#include "raygui.h"
double windowWidth, windowHeight;
std::vector<char> grid(9, ' ');
char playerTurn = 'X';
Rectangle border;
char win()
{
    int counter;
    char slot;
    int won[8][3] = {{0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 4, 8}, {2, 4, 6}};
    for (counter = 0; counter < 8; counter++)
    {
        int combination[3] = {won[counter][0], won[counter][1], won[counter][2]};
        {
            if (grid[combination[0]] == grid[combination[1]] && grid[combination[1]] == grid[combination[2]] && grid[combination[0]] != ' ')
                {
                return grid[combination[0]];
                }
        }
        for (counter = 0; counter < 9; counter++)
        {
            slot = grid[counter];
            {
                if (slot == ' ')
                {
                    return false;
                }
            }
        }
    }
}
int main()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(windowWidth, windowHeight, "Tic Tac Toe");
	SetWindowMinSize(320, 180);
	SetWindowMaxSize(windowWidth, windowHeight);
	MaximizeWindow();
	while (!WindowShouldClose()) {
		BeginDrawing();
		if (IsWindowResized()) {
			windowWidth = GetScreenWidth();
			windowHeight = GetScreenHeight();
		}
		if (windowWidth > windowHeight) {
			border.width = windowHeight;
			border.height = windowHeight;
			border.x = (windowWidth - border.width) / 2;
			border.y = 0;
		}
		else {
			border.height = windowWidth;
			border.width = windowWidth;
			border.y = (windowHeight - border.height) / 2;
			border.x = 0;
		}
		DrawRectangleLinesEx(border, 2, WHITE);
		const float cellLength = border.height / 3;
		const float y1 = border.y + cellLength;
		const float y2 = border.y + cellLength * 2;
		const float x1 = border.x + cellLength;
		const float x2 = border.x + cellLength * 2;
		DrawLineEx({ border.x, y1 }, { border.x + border.width, y1 }, 2, WHITE);
		DrawLineEx({ border.x, y2 }, { border.x + border.width, y2 }, 2, WHITE);
		DrawLineEx({ x1, border.y }, { x1, border.y + border.height }, 2, WHITE);
		DrawLineEx({ x2, border.y }, { x2, border.y + border.height }, 2, WHITE);
		if (IsMouseButtonPressed(0)) {
			const int mouseX = GetMouseX();
			const int mouseY = GetMouseY();
			int boardIndexY = 0;
			int boardIndexX = 0;
			if (mouseX > y2) boardIndexY = 2;
			if (mouseX <= y2 && mouseX > y1) boardIndexY = 1;
			if (mouseY > x2) boardIndexX = 2;
			if (mouseY <= x2 && mouseY > x1) boardIndexX = 1;
		}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
