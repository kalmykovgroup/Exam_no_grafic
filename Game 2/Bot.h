#pragma once   
#include "Header.h"   
#include "Player.h"   
#include "Matrix.h"   

class Bot : public Player
{
private:


    int** matrix; //Указатель на игровое поле
    double** pointMatrix; //Числовое поле для бота
    int width;
    int height;
    
    vector<Coordinate*> DestinationСoordinates; //Кординаты которые нужно заполнить фишками для выйгрыша
    int idxProgress; //Ползунок определяющий уровень заполнения

    PlayerNumber playerNumber;

public:

    Bot(Matrix* matrix1, PlayerNumber playerNumber);

    void iniPointMatrix();

   virtual int getLabel() {
        return int(playerNumber);
    }
 

    virtual void move(vector<Coordinate*>& path);  

    void clearPath(vector<Coordinate*>& path);

    bool singleStroke(int x, int y, vector<Coordinate*>& path, int endMove = 0, bool dc = false);

    void findingAWayForBot(vector<Coordinate*>& path);

    inline Coordinate getDirection(){ return *this->DestinationСoordinates[this->idxProgress]; }

    double distancePoints(Coordinate a, Coordinate b);

    bool LoopingСheck(vector<Coordinate*>& path, int x, int y);
  
    double calculateUtilityFactor(Coordinate start, Coordinate current, Coordinate finish);

    void findingAWayForBot(vector<Coordinate*>& currentPath, vector<Coordinate*>& savedPath, double&savedUtilityFactor);
 
    void showPointMatrix();

    //Если это координата которая уже стоит на своем месте
    bool CheckingForProhibition(Coordinate c);

    virtual bool isWin();
    void IncreaseProgress(vector<Coordinate*>& path);
  
};

