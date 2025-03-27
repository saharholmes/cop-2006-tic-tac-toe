#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include "raylib.h"

double width, height, horizontalLine1, horizontalLine2, verticalLine1, verticalLine2;

void drawGameBoard() {
	horizontalLine1 = height / 3;
	horizontalLine2 = height * 2 / 3;
	DrawLine(0, horizontalLine1, width, horizontalLine1, WHITE);
	DrawLine(0, horizontalLine2, width, horizontalLine2, WHITE);
	verticalLine1 = width / 3;
	verticalLine2 = width * 2 / 3;
	DrawLine(verticalLine1, 0, verticalLine1, height, WHITE);
	DrawLine(verticalLine2, 0, verticalLine2, height, WHITE);
}

int getButtonPressed() {
	const int x = GetMouseX();
	const int y = GetMouseY();
	std::cout << "\nMouse Click: (" << x << ", " << y << ")" << std::endl;
	int button;
	if (x > verticalLine2) button = 2;
	if (x < verticalLine2) button = 1;
	if (x < verticalLine1) button = 0;
	if (y > horizontalLine2) button += 6;
	if (y < horizontalLine2 && y > horizontalLine1) button += 3;
	return button;
}

int main()
{
	width = GetScreenWidth();
	height = GetScreenHeight();
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(width, height, "Tic Tac Toe");
	SetWindowMinSize(320, 180);
	SetWindowMaxSize(width, height);
	MaximizeWindow();
	while (!WindowShouldClose()) {
		BeginDrawing();
		if (IsWindowResized()) {
			width = GetScreenWidth();
			height = GetScreenHeight();
			std::cout << "width: " << width << "\nheight: " << height << std::endl;
		}
		drawGameBoard();
		if (IsMouseButtonPressed(0)) {
			std::cout << "Button Pressed: " << getButtonPressed() << std::endl;
		}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
