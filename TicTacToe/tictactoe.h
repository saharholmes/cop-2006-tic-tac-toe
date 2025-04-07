#ifndef TICTACTOE_H
#define TICTACTOE_H
#include <string>
#include <array>
#include "raylib.h"
enum Screen
{
	MAIN_MENU,
	GAMEPLAY,
};
struct GameCell
{
	std::string text;
	Rectangle box;
};
class TicTacToe
{
public:
	Rectangle area;
	std::array<GameCell, 9> cells;
	float cellLength, windowWidth, windowHeight;
	struct ButtonManager
	{
		Vector2 textSize;
		int fontSize;
		Rectangle play;
		Rectangle back;
		Rectangle playAgain;
		Rectangle exit;
	} button;
	Screen currentScreen = MAIN_MENU;
	std::string currentPlayer;
	std::string winner;
	double resetSecondsDelay;
	TicTacToe();
	void SetMenuSize();
	void DrawMenu();
	void SetGridSize();
	void DrawGrid();
	void GridMouseClick();
	void GetWinner();
};
#endif // TICTACTOE_H