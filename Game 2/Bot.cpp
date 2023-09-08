#include "Bot.h" 

//Рассчет расстояние между координатоми
double Bot::distancePoints(Coordinate a, Coordinate b)
{   
    return sqrt(pow(b.x  - a.x, 2) + pow(b.y - a.y, 2));
}


Bot::Bot(Matrix *matrix, PlayerNumber playerNumber) : matrix{ matrix->getArray() },
width{ matrix->getWidth() }, height{ matrix->getHeight() }, playerNumber{playerNumber}, idxProgress{0} {

    //Определяем поля на которые нужно встать что-бы завершить игру
    int label = (int)PlayerNumber::Two;
    Coordinate tmp(this->width - 1, this->height - 1);

    if (this->playerNumber == PlayerNumber::Two) {
        label = (int)PlayerNumber::One;
        tmp.setData(0, 0);
    }
    
    
    //Добавляем их в массив
    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++){
            if (this->matrix[y][x] == label) {
                DestinationСoordinates.push_back(new Coordinate(x, y, distancePoints({x, y}, tmp)));
            }
        }
    }
 
    //Сортируем по приотитетности 
    for (int i = 0; i < DestinationСoordinates.size(); i++) {
        for (int j = 0; j < DestinationСoordinates.size() - 1; j++) {
            if (DestinationСoordinates[j]->length > DestinationСoordinates[j + 1]->length)
                swap(DestinationСoordinates[j], DestinationСoordinates[j + 1]);
        }
    }
  

    iniPointMatrix();

    this->showPointMatrix();
    cout << "test" << endl;
};


void Bot::iniPointMatrix() {
    this->pointMatrix = new double* [this->height];

    for (int i = 0; i < this->height; i++)
        this->pointMatrix[i] = new double[width] {0};

    //Сдесь начинаеться магия, создание поля, нужного для того что-бы двигать все фигуры кучкой через центр поля.
    //Генерация поля заточена под любые размеры, прямоугольник, квадрат.
 
    int max = width > height ? width : height;
    int min = width < height ? width : height;

    //Нужно для прямоугольника, что-бы находить нужные координаты
    double persentMax = 100.0 / max;
    double persentMin = 100.0 / min;

    //Нужно для отслеживания положения поля при генирации чисел
    double currentPersent = 0;

    //Для позиции Y,  если width больше или равно heigth
    

    //Нужно что-бы смещать диагональ проведения 
    int middleMax = max / 2;
    int middleMin = min / 2;
 
    double p = 0.2;
    for (int l = middleMax, g = 0; l < max; l++, g++) { //Чтобы проводить несколько диагоналей, со смещением на 1
        currentPersent = 0; //Обнуляем после каждой диагонали
        int z = 1;

        for (int i = middleMax + g; i < max; i++, currentPersent += persentMax) { //Идем по самой большой велечине, если это прямоугольник

            for (int k = 0; k < min; k++, z++) {//Находим в процентном соотношении позицию Y (если width(x) больше height(y))
                if (z * persentMin > currentPersent) break;
            }
   
            if (width >= height) { //Если Это квадрат или прямоугольник, у которого ширина больше
                this->pointMatrix[z - 1][i] = p;     
                this->pointMatrix[z - 1 + middleMin + g][i - middleMax - g] = p;

            }
            else {//Иначе делаем инверсию в координатах, если высота больше ширины.
                this->pointMatrix[i][z - 1] = p;
                this->pointMatrix[i - middleMax - g][z - 1 + middleMin + g] = p;
            }

        }
        p += 0.25;
    }
}
 

void Bot::move(vector<Coordinate*>& path)
{
    findingAWayForBot(path); //Поиск оптимального хода компьютера   

    //Если мы встаем на клетку назначения,
    this->IncreaseProgress(path);
     
}

//Если это координата которая уже стоит на своем месте
bool Bot::CheckingForProhibition(Coordinate c) {
    for (int i = 0; i < this->idxProgress; i++) 
        if (*DestinationСoordinates[i] == c)  return true;
        
    return false;
}


//Проверка на выйгрыш
bool Bot::isWin(){
    if (this->DestinationСoordinates.size() == this->idxProgress)
        return true;
    return false;
}

//Если мы заполнили нужную клетку, то берем следующую как цель
void Bot::IncreaseProgress(vector<Coordinate*>& path) {
      
    if (*path[path.size() - 1] == *this->DestinationСoordinates[this->idxProgress]) { 
        while (true) {
            this->idxProgress++;
            //Если мы дошли до конца
            if (this->idxProgress == this->DestinationСoordinates.size()) return;

            int y = this->DestinationСoordinates[this->idxProgress]->y;
            int x = this->DestinationСoordinates[this->idxProgress]->x;

            if (matrix[y][x] == this->getLabel() && *this->DestinationСoordinates[this->idxProgress] != *path[0]) {
                continue;
            }
            break;
        }
        //Как только мы все заполним, то выйграли.
    }
}

void Bot::findingAWayForBot(vector<Coordinate*>& path) { //Поиск оптимального хода

    double mainUtilityFactor = 0; //Нужно что-бы хранить вес хода итогового  (кол-во и качество прыжков, пройденный путь)

    vector<Coordinate*> savedPath;  //Что-бы хранить историю прыжков
    vector<Coordinate*> currentPath; //Что-бы хранить историю в текущий момент

    //Пробуем двигаться через клетки
    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++) {
             
            if (matrix[y][x] == this->getLabel()) {

                if (CheckingForProhibition({ x, y })) continue;

                double savedUtilityFactor = 0; //Создаем переменную что-бы хранить текущий вес хода (кол-во и качество прыжков, пройденный путь)

                currentPath.push_back(new Coordinate(x, y)); //Записываем стартовую позици, от которой будем искать найлучшый вариант хода

                findingAWayForBot(currentPath, savedPath, savedUtilityFactor);


                if (savedUtilityFactor > mainUtilityFactor) { //если коэффициент полезности больше, то перезаписываем как самый опримальный ход
                    this->clearPath(path);

                    mainUtilityFactor = savedUtilityFactor;
                    for (int i = 0; i < savedPath.size(); i++)
                        path.push_back(new Coordinate(*savedPath[i]));

                }

                this->clearPath(currentPath); //Очищаем историю ходов текущей шашкой, для следующей
                this->clearPath(savedPath);
            }
        }
    }
 
 
    if (mainUtilityFactor <= 0 || path.size() == 2) { //Если вес хода меньше или рав

        if (path.size() == 2 && mainUtilityFactor > 0) { //Если это один прыжок, и вес у него положительный - компьютер рандомно выбирает ходить, ходить на одну клетку или прыгать через одну
            int randNum = rand() % 100; 
             if (randNum >= 50) return;
        }

        vector<Coordinate*> vector;

        for (int y = 0; y < this->height; y++) { //Добавили все координыты на которых стоим
            for (int x = 0; x < this->width; x++) { //Обход каждой фишки, что-бы понять какой лучше всего пойти
                if (matrix[y][x] == getLabel()) {
                    if (CheckingForProhibition({ x, y })) continue; //Исключем те, которые стоят на нужном месте
                    vector.push_back(new Coordinate(x, y));
                }
            }
        }

        //Сортируем, что-бы найти координату которая стоит последняя, для того что-бы не расползались шашки по доске.
        for (int i = 0; i < vector.size(); i++) {
            for (int j = 0; j < vector.size() - 1; j++) {
                if (distancePoints(*vector[j], this->getDirection()) < distancePoints(*vector[j + 1], this->getDirection())) {
                    swap(vector[j], vector[j + 1]);
                }
            }
        }
         
        //Если мы можем одним ходом поставить на позицию цели
        for (int i = 0; i < vector.size(); i++) {
            if (singleStroke(vector[i]->x, vector[i]->y, path, 0, true)) {                            
                return;
            }
        }

        //Если не получилось, пробуем сделать положительный ход. (ход который нас преблизит)
        for (int i = 0; i < vector.size(); i++) {
            if (singleStroke(vector[i]->x, vector[i]->y, path, 0)) { //Если ход положительный найден                             
                return;
            }
        }   

        //Если не получилось, пробуем делать отрицательный ход
        for (int i = 0; i < vector.size(); i++) {
            if (singleStroke(vector[i]->x, vector[i]->y, path, -9999)) { //Если ход положительный найден                             
                return;
            }
        }   
              
     
    } 
 

}


void Bot::findingAWayForBot(vector<Coordinate*>& currentPath, vector<Coordinate*>& savedPath, double& savedUtilityFactor)
{

    //Выщитываем коэффициент полезности такого хода
    double utilityFactor = calculateUtilityFactor(*currentPath[0], *currentPath[currentPath.size() - 1], this->getDirection());

   
    if (utilityFactor > savedUtilityFactor) { //Если коэффициент полезности больше чем у предыдущего, то сохраняем этот путь как самый оптимальный
        savedUtilityFactor = utilityFactor;
        this->clearPath(savedPath); //Очистили предыдущий путь
        for (int i = 0; i < currentPath.size(); i++) //Записали текущий как самый оптимальный
            savedPath.push_back(new Coordinate(*currentPath[i]));
    }


    int x = currentPath[currentPath.size() - 1]->x;
    int y = currentPath[currentPath.size() - 1]->y;

    //Если мы не вышли за пределы поля и можем шагнуть
    if (y + 2 < this->height && matrix[y + 1][x] != 0 && matrix[y + 2][x] == 0) { //Шаг вниз

        if (LoopingСheck(currentPath, x, y + 2)) { //Если не становились на эту координату
            currentPath.push_back(new Coordinate(x, y + 2)); //Добавили в конец координату

            findingAWayForBot(currentPath, savedPath, savedUtilityFactor); //Пробуем делать следующий шаг

            //После выполения, откатываемся назад
            delete currentPath[currentPath.size() - 1]; //Вычистили память
            currentPath.erase(currentPath.end() - 1);//Удалили из массива последний по индексу указатель
        }

    }

    
    if (x - 2 >= 0 && matrix[y][x - 1] != 0 && matrix[y][x - 2] == 0) { //Шаг влево
        if (LoopingСheck(currentPath, x - 2, y)) {
            currentPath.push_back(new Coordinate(x - 2, y));


            findingAWayForBot(currentPath, savedPath, savedUtilityFactor);

            delete currentPath[currentPath.size() - 1];
            currentPath.erase(currentPath.end() - 1);
        }
    }


    if (y - 2 >= 0 && matrix[y - 1][x] != 0 && matrix[y - 2][x] == 0) { //Шаг вверх
        if (LoopingСheck(currentPath, x, y - 2)) {
            currentPath.push_back(new Coordinate(x, y - 2));

            findingAWayForBot(currentPath, savedPath, savedUtilityFactor);

            delete currentPath[currentPath.size() - 1]; 
            currentPath.erase(currentPath.end() - 1);
        }
    }

    if (x + 2 < this->width && matrix[y][x + 1] != 0 && matrix[y][x + 2] == 0) { //Шаг вправо
        if (LoopingСheck(currentPath, x + 2, y)) {
            currentPath.push_back(new Coordinate(x + 2, y));

            findingAWayForBot(currentPath, savedPath, savedUtilityFactor);

            delete currentPath[currentPath.size() - 1]; 
            currentPath.erase(currentPath.end() - 1); 
        }
    }
    return;

}



double Bot::calculateUtilityFactor(Coordinate start, Coordinate current, Coordinate finish) {
  
    //start - Координата откуда начинали ход
    //current - Координата куда шагнули
    //finish - Координата куда стремимся

    //Получить главную длину от начала до финальной
    double res1 = distancePoints(start, finish);

    // Вычислить расстояние от проверяемой точки до финальной
    double res2 = distancePoints(current, finish);
    
    if (res2 > res1) return 0;

    //Вычесть этот процент, и найти пройденное расстояние в процентах 
    return (res1 - res2) - this->pointMatrix[current.y][current.x];;
}


void Bot::clearPath(vector<Coordinate*>& path)
{
    for (int i = 0; i < path.size(); i++)
        delete path[i];
    path.clear();
}

bool Bot::singleStroke(int x, int y, vector<Coordinate*>& path, int endMove, bool dc) {


    if (dc && x - 1 >= 0 && matrix[y][x - 1] == 0 && *this->DestinationСoordinates[this->idxProgress] == Coordinate{x - 1, y} ||
        x - 1 >= 0 && matrix[y][x - 1] == 0 && calculateUtilityFactor({ x, y }, { x - 1, y }, this->getDirection()) > endMove) {//left
   
            clearPath(path);//Очищаем историю  
            path.push_back(new Coordinate(x, y)); //Записываем начало пути                      
            path.push_back(new Coordinate(x - 1, y));//Записываем конец пути  
       
        return true;
    }

    if (dc && x + 1 < this->width && matrix[y][x + 1] == 0 && *this->DestinationСoordinates[this->idxProgress] == Coordinate{x + 1, y} ||
         x + 1 < this->width && matrix[y][x + 1] == 0 && calculateUtilityFactor({ x, y }, { x + 1, y }, this->getDirection()) > endMove) {//right
   
            clearPath(path);
            path.push_back(new Coordinate(x, y));                      
            path.push_back(new Coordinate(x + 1, y));
       
        return true;
    }


    if (dc && y - 1 >= 0 && matrix[y - 1][x] == 0 && *this->DestinationСoordinates[this->idxProgress] == Coordinate{x, y - 1} ||
        y - 1 >= 0 && matrix[y - 1][x] == 0 && calculateUtilityFactor({ x, y }, { x, y - 1 }, this->getDirection()) > endMove) {//top            
            clearPath(path);
            path.push_back(new Coordinate(x, y));                     
            path.push_back(new Coordinate(x, y - 1));
            return true;     
    }


    if (dc && y + 1 < this->height && matrix[y + 1][x] == 0 && *this->DestinationСoordinates[this->idxProgress] == Coordinate{x, y + 1} ||
        y + 1 < this->height && matrix[y + 1][x] == 0 && calculateUtilityFactor({ x, y }, { x, y + 1 }, this->getDirection()) > endMove) {//bottom
        
            clearPath(path);
            path.push_back(new Coordinate(x, y));                   
            path.push_back(new Coordinate(x, y + 1)); 
            return true;
    }
    return false;

}


//Проверяем что мы туда не становились, что-бы не ходить по кругу
bool Bot::LoopingСheck(vector<Coordinate*>& path, int x, int y)
{
    //x,y клетка на которую мы собираемся шагнуть
    for (int i = 0; i < path.size(); i++) {
        if (path[i]->x == x && path[i]->y == y)  return false;
    }
    return true;
}

 


void Bot::showPointMatrix()
{
    cout << endl << endl;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cout << setw(4) << setfill(' ') << this->pointMatrix[i][j];
        }
        cout << endl;
    }

}


