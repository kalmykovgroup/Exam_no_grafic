#include "User.h" 

User::User(Matrix* matrix, PlayerNumber playerNumber) : matrix{ matrix->getArray() }, width{ matrix->getWidth() }, height{ matrix->getHeight() }, playerNumber{ playerNumber }
{
    //Определяем поля на которые нужно встать что-бы завершить игру
    int label = this->playerNumber == PlayerNumber::Two ? (int)PlayerNumber::One : (int)PlayerNumber::Two;

    //Добавляем их в массив
    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++) {
            if (this->matrix[y][x] == label) {
                DestinationСoordinates.push_back(new Coordinate(x, y));
            }
        }
    }
};

void User::getData(Coordinate* c, int flag)
{
    try {
        string navArr1 = "ABCDEFGHIKLMNOPQRSTVXYZ";
        string navArr2 = "abcdefghiklmnopqrstvxyz";
        char value[3];

        while (true) {

                cin >> value;

                for (int i = 0; i < 24; i++) {
                    if (value[0] == navArr1[i] || value[0] == navArr2[i]) {
                        c->x = i;
                        break;
                    }
                }
                char a[2] = { value[1] };
                c->y = atoi(a);
                c->y--;
                if (c->y == -1 || c->x == -1 || flag && this->matrix[c->y][c->x] != this->getLabel()) {
                    cout << "Ошибка, повторите ввод!" << endl;
                    continue;
                }    
            break;
        } 
    }
     
    catch (...) {
        throw Exception{__LINE__, __FILE__, ErrorCode::Fatal, "getData() - Не известная ошибка"};
    }
   
   
}

void User::move(vector<Coordinate*>& path)
{
    while (true) {
        Coordinate* beginPath = new Coordinate; //Координата откуда начинать
        Coordinate* endPath = new Coordinate;// координата куда шагнуть

        cout << "Введите координату (что перемещать): ";
        this->getData(beginPath, true);

        cout << "Введите координату (куда перемещать): ";
        this->getData(endPath);

        path.push_back(beginPath); //Записываем начало пути

        if (!checkingMove(this->getLabel(), path, endPath)) { //Проверяем что такой ход возможен и если возможен записываем в массив движение по клеткам            
            Game::clearPath(path);
            cout << "Не допустимый ход" << endl;
            continue;

        } 
        break;
    }
}

bool User::checkingMove(int label, vector<Coordinate*>& path, Coordinate* finish)
{
    try {
        //Если на туже самую клетку || сделать ход не своей фигурой || на занятую клетку

        if (finish == path[0] || matrix[finish->y][finish->x] != 0 || matrix[path[0]->y][path[0]->x] != label)
            return false;

        //Если одна из координат выходит за рамки поля
        if (*path[0] < 0 || *finish < 0 || path[0]->x > this->width || path[0]->y > this->height || finish->x > this->width || finish->y > this->height)
            return false;


        //Если движение на одну клеточку, 
        if (finish->y == path[0]->y && (finish->x == path[0]->x - 1 || finish->x == path[0]->x + 1) ||  //Влево, вправо
            finish->x == path[0]->x && (finish->y == path[0]->y + 1 || finish->y == path[0]->y - 1)) { //Вверх, вниз          
            path.push_back(finish);
            return true;
        }

        if (findingAWay(path, finish)) //Если путь найден 
            return true;
        else
            return false;
    }
    catch (...) {
        throw Exception{__LINE__, __FILE__, ErrorCode::Fatal, "checkingMove() - Не известная ошибка"};
    }
    

}

bool User::findingAWay(vector<Coordinate*>& path, Coordinate* finish)
{
    if (*path[path.size() - 1] == *finish) //Если путь найден
        return true;

    Coordinate currentP(*path[path.size() - 1]);

    //если не выщли за предел, есть шашка впереди и свободное место после нее
    if (currentP.y + 2 < this->height && matrix[currentP.y + 1][currentP.x] != 0 && matrix[currentP.y + 2][currentP.x] == 0) { //Шаг вниз
        //Далее если мы не дайдем да конечной точки по этому пути, то пробуем следующий, иначе путь найден
        if (findingAWay(path, finish, new Coordinate{ currentP.x, currentP.y + 2 })) return true;
    }

    if (currentP.x - 2 >= 0 && matrix[currentP.y][currentP.x - 1] != 0 && matrix[currentP.y][currentP.x - 2] == 0) { //Шаг влево
        if (findingAWay(path, finish, new Coordinate{ currentP.x - 2, currentP.y })) return true;
    }


    if (currentP.y - 2 >= 0 && matrix[currentP.y - 1][currentP.x] != 0 && matrix[currentP.y - 2][currentP.x] == 0) { //Шаг вверх
        if (findingAWay(path, finish, new Coordinate{ currentP.x, currentP.y - 2 })) return true;

    }

    if (currentP.x + 2 < this->width && matrix[currentP.y][currentP.x + 1] != 0 && matrix[currentP.y][currentP.x + 2] == 0) { //Шаг вправо
        if (findingAWay(path, finish, new Coordinate{ currentP.x + 2, currentP.y })) return true;
    }

    return false;

}

bool User::findingAWay(vector<Coordinate*>& path, Coordinate* finish, Coordinate* newPlace)
{
    try{
        //newPlace клетка на которую мы собираемся шагнуть
        bool flag = true;
        for (int i = 0; i < path.size(); i++) { //Проверяем что мы туда не становились, что-бы не ходить по кругу
            if (*path[i] == *newPlace) { flag = false; break; }
        }

        if (flag) { //Если мы не занимали эту клетку
            path.push_back(newPlace); //записываем как потенциально возможный маршрут, От него двигаемся дальше
            if (findingAWay(path, finish)) return true; //Если далее мы не находим маршрут

            path.erase(path.end() - 1);//То удаляем этот путь, возвращаемся и пробуем другие
        }

        delete newPlace;
        return false;
    }
    catch (...) {
        throw Exception{__LINE__, __FILE__, ErrorCode::Fatal, "findingAWay() - Не известная ошибка"};
    }
   
}

bool User::isWin()
{
     for(int i = 0; i < this->DestinationСoordinates.size(); i++)
         if(this->matrix[DestinationСoordinates[i]->y][DestinationСoordinates[i]->x] != this->getLabel())
        return false;
    return true;
}


