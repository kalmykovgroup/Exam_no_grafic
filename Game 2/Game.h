#pragma once 
#include "Header.h"  
#include "Matrix.h"  
#include "Player.h"  
#include "User.h"  
#include "Bot.h"  

class Game
{
	Matrix* matrix = nullptr;

	Player* player_1 = nullptr; //—сылка на игрока
	Player* player_2 = nullptr;

	void menu_1(); //ћеню выбора размера пол€
	void menu_2(); //ћеню выбора режима игры 
	void enteringMatrixParameters(int &width, int &height, int& countFigures);
	void enteringGameModeParameters();
public:
	void run();
	void start();

	static void clearPath(vector<Coordinate*>& path);

	 
};

