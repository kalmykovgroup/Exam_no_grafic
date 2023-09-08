#pragma once  

#include "Header.h" 
#include "Coordinate.h" 
#include "ConsoleColor.h"  

class Matrix {

private:

    int** field;  //Игровое поле

    int width;  //Размеры поля
    int height;

public:

    Matrix(int width, int height);

    int getWidth() { return this->width; }
    int getHeight() { return this->height; }
    int** getArray() { return this->field; }

     void show(Coordinate* begin = nullptr, Coordinate* end = nullptr, Coordinate* current = nullptr);

     void animate(vector<Coordinate*>& path);

     void setFigures(int countFigures);
};