// _CRT_SECURE_NO_WARNINGS included for raygui as instructed in error messages.
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <string>
#include <array>
#include <algorithm>
#include <vector>
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "tictactoe.h"
// resetSecondsDelay is used in the case of a tie so the user has time to process a tie has occured before the board is reset.
TicTacToe::TicTacToe()
{
	SetMenuSize();
	resetSecondsDelay = -1;
}
// Minimum play button size is 400pxx50px
void TicTacToe::SetMenuSize()
{
	windowWidth = GetScreenWidth();
	windowHeight = GetScreenHeight();
	button.textSize = { std::max((windowWidth / 4), 400.0f), std::max((windowWidth / 30), 50.0f) };
	GuiSetStyle(DEFAULT, TEXT_SIZE, button.textSize.y);
	button.play = { windowWidth / 2 - button.textSize.x / 2, windowHeight / 2 - button.textSize.y / 2, button.textSize.x, button.textSize.y };
}
void TicTacToe::DrawMenu()
{
	const int textSize = MeasureText("TicTacToe", windowWidth / 10);
	DrawText("TicTacToe", windowWidth / 2 - textSize / 2, 0, windowWidth / 10, WHITE);
	if (GuiButton(button.play, "Play"))
	{
		currentScreen = Screen::GAMEPLAY;
		currentPlayer = std::rand() % 2 == 0 ? "X" : "O";
		SetGridSize();
	}
}
// Sets positions and sizes of ui elements on the gameplay screen based on window size.
void TicTacToe::SetGridSize()
{
	windowWidth = GetScreenWidth();
	windowHeight = GetScreenHeight();
	area.height = windowHeight - 50;
	if (windowWidth > windowHeight)
	{
		area.x = (windowWidth - area.height) / 2;
		area.y = 0;
		cellLength = area.height / 3;
	}
	else
	{
		area.y = (area.height - windowWidth) / 2;
		area.x = 0;
		cellLength = windowWidth / 3;
	}
	for (int i = 0; i < 9; i++)
	{
		int row = i / 3;
		int col = i % 3;
		cells[i].box.x = area.x + col * cellLength;
		cells[i].box.y = area.y + row * cellLength;
		cells[i].box.height = cellLength;
		cells[i].box.width = cellLength;
	}
	if (winner.empty())
	{
		button.textSize = { std::max((windowWidth / 7), 200.0f), 45 };
		button.back = { area.x + cellLength * 2.5f - button.textSize.x / 2, area.height + 5, button.textSize.x, button.textSize.y };
	}
	else
	{
		button.textSize = { std::max((windowWidth / 4), 400.0f), std::max((windowWidth / 30), 50.0f) };
		button.playAgain = { windowWidth / 2 - button.textSize.x / 2, windowHeight * 0.6f - button.textSize.y * 2, button.textSize.x, button.textSize.y };
		button.exit = { button.playAgain.x, windowHeight * 0.6f - button.textSize.y / 2, button.textSize.x, button.textSize.y };
		button.back = { area.x + cellLength * 2.5f - std::max((windowWidth / 7), 200.0f) / 2, area.height + 5, std::max((windowWidth / 7), 200.0f), 45 };
	}
	GuiSetStyle(DEFAULT, TEXT_SIZE, button.textSize.y);
}
// Draws gameplay screen ui elements.
void TicTacToe::DrawGrid()
{
	for (int i = 0; i < 9; i++)
	{
		DrawRectangleLinesEx(cells[i].box, 2, WHITE);
		const Vector2 textSize = MeasureTextEx(GetFontDefault(), cells[i].text.c_str(), area.height / 3, 1);
		DrawText(
			cells[i].text.c_str(),
			cells[i].box.x + (cells[i].box.width - textSize.x) / 2,
			cells[i].box.y + (cells[i].box.height - textSize.y) / 2,
			area.height / 3,
			WHITE
		);
	}
	DrawText(
		(currentPlayer + " player turn").c_str(),
		std::max(0.0f, area.x + cellLength - MeasureText((currentPlayer + " player turn").c_str(), std::min(45.0f, windowWidth / 15))),
		area.height + 5,
		std::min(45.0f, windowWidth / 15),
		WHITE
	);
	if (winner.empty())
	{
		if (resetSecondsDelay != -1 && GetTime() > resetSecondsDelay + 2)
		{
			for (int i = 0; i < 9; i++) cells[i].text = "";
			resetSecondsDelay = -1;
		}
		if (GuiButton(button.back, "Back"))
		{
			currentScreen = Screen::MAIN_MENU;
			SetMenuSize();
			for (int i = 0; i < 9; i++) cells[i].text = "";
		}
	}
	else
	{
		DrawRectangleLinesEx(button.back, 2, WHITE);
		DrawRectangle(0, 0, windowWidth, windowHeight, { 255, 255, 255, 192 });
		const Vector2 textSize = MeasureTextEx(GetFontDefault(), (winner + " has won the game!").c_str(), windowWidth / 15, windowWidth / 75);
		DrawTextEx(GetFontDefault(),
			(winner + " has won the game!").c_str(),
			{ windowWidth / 2 - textSize.x / 2, windowHeight / 2 / 3 - textSize.y / 2, },
			windowWidth / 15,
			windowWidth / 75,
			BLACK
		);
		if (GuiButton(button.playAgain, "Play Again"))
		{
			for (int i = 0; i < 9; i++) cells[i].text = "";
			winner = "";
			currentPlayer = std::rand() % 2 == 0 ? "X" : "O";
		}
		if (GuiButton(button.exit, "Exit")) {
			for (int i = 0; i < 9; i++) cells[i].text = "";
			winner = "";
			SetMenuSize();
			currentScreen = Screen::MAIN_MENU;
		}
	}
}
// Determines whether mouse click is valid for a player's turn.
void TicTacToe::GridMouseClick() {
	if (!winner.empty()) return;
	const Vector2 mouse = { GetMouseX(), GetMouseY() };
	for (int i = 0; i < 9; i++)
	{
		if (CheckCollisionPointRec(mouse, cells[i].box))
		{
			if (!cells[i].text.empty()) return;
			cells[i].text = currentPlayer;
			break;
		}
		if (i == 8) return;
	}
	GetWinner();
	if (!winner.empty()) return;
	currentPlayer = currentPlayer == "X" ? "O" : "X";
	int count = 0;
	for (int i = 0; i < 9; i++)
	{
		if (!cells[i].text.empty()) count++;
	}
	if (count != 9) return;
	resetSecondsDelay = GetTime();
}
// Checks both player's text character in array for winning combination.
void TicTacToe::GetWinner()
{
	const std::vector<std::vector<int>> won = { { {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 4, 8}, {2, 4, 6} } };
	for (int i = 0; i < 8; i++)
	{
		const std::vector<int> combination = { { won[i][0], won[i][1], won[i][2] } };
		if (
			cells[combination[0]].text == cells[combination[1]].text &&
			cells[combination[1]].text == cells[combination[2]].text &&
			!cells[combination[0]].text.empty()
			) {
			winner = cells[combination[0]].text;
			SetGridSize();
			return;
		}
	}
}
// Used WinMain for /MTd and /MT code generation. Switching the code generation setting is part of packaging the required dlls with the exe file.
int WinMain()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "TicTacToe");
	SetWindowMinSize(500, 350);
	SetTargetFPS(60);
	MaximizeWindow();
	GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0x000000FF);
	GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, 0x111111FF);
	GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, 0x222222FF);
	GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xFFFFFFFF);
	GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, 0xFFFFFFFF);
	GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, 0xFFFFFFFF);
	GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, 0xFFFFFFFF);
	GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, 0xEEEEEEFF);
	GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, 0xFFFFFFFF);
	TicTacToe ticTacToe;
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		switch (ticTacToe.currentScreen)
		{
		case Screen::MAIN_MENU:
			ticTacToe.DrawMenu();
			if (IsWindowResized()) ticTacToe.SetMenuSize();
			break;
		case Screen::GAMEPLAY:
			ticTacToe.DrawGrid();
			if (IsWindowResized()) ticTacToe.SetGridSize();
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) ticTacToe.GridMouseClick();
			break;
		}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}