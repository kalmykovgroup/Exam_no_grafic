#pragma once 
#include "Header.h"

class Player
{

public:

	virtual int getLabel() = 0;
	virtual void move(vector<Coordinate*>& path) = 0;
	virtual bool isWin() = 0; 
};