#pragma once 
#include "Header.h" 
#include "Player.h" 
#include "Matrix.h" 
#include "Game.h" 

class User: public Player
{

private:
    PlayerNumber playerNumber;
    int** matrix;
    int width;
    int height;

    vector<Coordinate*> DestinationÑoordinates; //Êîðäèíàòû êîòîðûå íóæíî çàïîëíèòü ôèøêàìè äëÿ âûéãðûøà
  

public:

    User(Matrix* matrix, PlayerNumber playerNumber);
 
   virtual int getLabel() { return (int)this->playerNumber; }

   void getData(Coordinate* c, int flag = false);

    virtual void move(vector<Coordinate*>& path);
    bool checkingMove(int label, vector<Coordinate*>& path, Coordinate* finish);
    bool findingAWay(vector<Coordinate*>& path, Coordinate* finish);
    bool findingAWay(vector<Coordinate*>& path, Coordinate* finish, Coordinate* newPlace);
    virtual bool isWin();
};
