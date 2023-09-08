#pragma once
#include "Header.h"

class Coordinate {
public:
   
    int x, y;
    double length;
    Coordinate() : x{ -1 }, y{ -1 }, length{ -99 } {};
    Coordinate(int x, int y) : x{ x }, y{ y }, length{ 0 } {};
    Coordinate(int x, int y, double length) : Coordinate(x, y) {
        this->length = length;
    };

    bool operator ==(const Coordinate& ob2) {
        if (this->x == ob2.x && this->y == ob2.y)
            return true;
        return false;
    }

    bool operator ==(const Coordinate&& dying) {
        if (this->x == dying.x && this->y == dying.y)
            return true;
        return false;
    }

    inline void setData(int x, int y) {
        this->x = x; this->y = y;
    }

    Coordinate(const Coordinate& original) {
        this->x = original.x;
        this->y = original.y;
        this->length = original.length;
    }

    Coordinate& operator=(const Coordinate& original) {
        this->x = original.x;
        this->y = original.y;
        return *this;
    }

    bool operator!=(const Coordinate& that) {

        if (this->x != that.x || this->y != that.y)
            return true;
        return false;
    }




    bool operator <(int value) {
        if (this->x < value || this->y < value) return true;
        return false;
    }

    void clear() {
        this->x = -1;
        this->y = -1;
        this->length = -99;
    }



};

