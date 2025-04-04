#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
enum Screen {
	MAIN_MENU,
	GAMEPLAY,
};
struct GameCell {
	std::string text;
	Rectangle box;
};
class TTTGame {
public:
	Rectangle area;
	std::array<GameCell, 9> cells;
	float cellLength, windowWidth, windowHeight;
	struct ButtonManager {
		Vector2 globalSize;
		int fontSize;
		Rectangle play;
		Rectangle exit;
		Rectangle restart;
	} button;
	Screen currentScreen = MAIN_MENU;
	std::string currentPlayer;
	std::string winner;
	TTTGame() {
		SetMenuSize();
	}
	void SetMenuSize() {
		windowWidth = GetScreenWidth();
		windowHeight = GetScreenHeight();
		button.globalSize = { windowWidth / 4, windowWidth / 30 };
		GuiSetStyle(DEFAULT, TEXT_SIZE, button.globalSize.y);
		button.play = { windowWidth / 2 - button.globalSize.x / 2, windowHeight / 2 - button.globalSize.y / 2, button.globalSize.x, button.globalSize.y };
	}
	void DrawMenu() {
		windowWidth = GetScreenWidth();
		const int textSize = MeasureText("TicTacToe", windowWidth / 10);
		DrawText("TicTacToe", windowWidth / 2 - textSize / 2, 0, windowWidth / 10, WHITE);
		if (GuiButton(button.play, "Play")) {
			currentScreen = Screen::GAMEPLAY;
			currentPlayer = std::rand() % 2 == 0 ? "X" : "O";
			SetGridSize();
		}
	}
	void SetGridSize() {
		windowWidth = GetScreenWidth();
		windowHeight = GetScreenHeight();
		button.globalSize = { windowWidth / 7, 40 };
		GuiSetStyle(DEFAULT, TEXT_SIZE, button.globalSize.y);
		area.height = windowHeight - 50;
		button.exit = { windowWidth / 2 - button.globalSize.x / 2, area.height + 5, button.globalSize.x, button.globalSize.y };
		if (windowWidth > windowHeight) {
			area.x = (windowWidth - area.height) / 2;
			area.y = 0;
			cellLength = area.height / 3;
		}
		else {
			area.y = (area.height - windowWidth) / 2;
			area.x = 0;
			cellLength = windowWidth / 3;
		}
		for (int i = 0; i < 9; i++) {
			int row = i / 3;
			int col = i % 3;
			cells[i].box.x = area.x + col * cellLength;
			cells[i].box.y = area.y + row * cellLength;
			cells[i].box.height = cellLength;
			cells[i].box.width = cellLength;
		}
	}
	void DrawGrid() {
		for (int i = 0; i < 9; i++) {
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
		if (!winner.empty()) {
			const std::string winnerText = winner + " has won the game!";
			const Vector2 textSize = MeasureTextEx(GetFontDefault(), "TicTacToe", windowWidth / 10, 1);
			DrawText(winnerText.c_str(), windowWidth / 2 - textSize.x / 2, 0, windowWidth / 10, WHITE);
			DrawRectangle(0, 0, windowWidth, windowHeight, { 255, 255, 255, 128 });
		}
		if (GuiButton(button.exit, "Exit")) {
			currentScreen = Screen::MAIN_MENU;
			SetMenuSize();
			for (int i = 0; i < 9; i++) cells[i].text = "";
		}
	}
	void GridMouseClick() {
		for (int i = 0; i < 9; i++) {
			const Vector2 mouse = { GetMouseX(), GetMouseY() };
			if (CheckCollisionPointRec(mouse, cells[i].box)) {
				if (!cells[i].text.empty()) break;
				cells[i].text = currentPlayer;
				GetWinner();
				std::cout << winner << std::endl;
				if (!winner.empty()) break;
				currentPlayer = currentPlayer == "X" ? "O" : "X";
				int count = 0;
				for (int i = 0; i < 0; i++) {
					if (!cells[i].text.empty()) count++;
				}
				if (count != 9) break;
				for (int i = 0; i < 9; i++) cells[i].text = "";
			}
		}
	}
	void GetWinner() {
		std::cout << "Calculating Winner\n(idk why this isnt working rn its late gonna look at it again tomorrow)" << std::endl;
		const std::vector<std::vector<int>> won = { { {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 4, 8}, {2, 4, 6} } };
		for (int i = 0; i < 8; i++) {
			const std::vector<int> combination = { { won[i][0], won[i][1], won[i][2] } };
			std::cout << combination[0] << combination[1] << combination[2] << std::endl;
			if (cells[combination[0]].text == cells[combination[1]].text && cells[combination[1]].text == cells[combination[2]].text && cells[combination[0]].text.empty()) {
				winner = cells[combination[0]].text;
				return;
			}
		}
	}
};
int main()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "TicTacToe");
	SetWindowMinSize(200, 200);
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
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
	TTTGame ticTacToe;
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		switch (ticTacToe.currentScreen) {
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