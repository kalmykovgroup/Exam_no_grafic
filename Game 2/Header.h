#pragma once
#include <iostream>
#include <windows.h>
#include <iomanip>
#include <vector>
#include <conio.h>
using namespace std;

class Coordinate;
class Matrix;
class Player;
class User;
class Bot;

enum PlayerNumber { One = 1, Two = 2 };

enum ErrorCode {Warning, Fatal };

struct Exception {
	int line;
	const char* file;
	ErrorCode code;
	const char* text;
	
};

