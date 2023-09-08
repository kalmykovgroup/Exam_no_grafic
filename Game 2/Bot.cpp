#include "Bot.h" 

//������� ���������� ����� ������������
double Bot::distancePoints(Coordinate a, Coordinate b)
{   
    return sqrt(pow(b.x  - a.x, 2) + pow(b.y - a.y, 2));
}


Bot::Bot(Matrix *matrix, PlayerNumber playerNumber) : matrix{ matrix->getArray() },
width{ matrix->getWidth() }, height{ matrix->getHeight() }, playerNumber{playerNumber}, idxProgress{0} {

    //���������� ���� �� ������� ����� ������ ���-�� ��������� ����
    int label = (int)PlayerNumber::Two;
    Coordinate tmp(this->width - 1, this->height - 1);

    if (this->playerNumber == PlayerNumber::Two) {
        label = (int)PlayerNumber::One;
        tmp.setData(0, 0);
    }
    
    
    //��������� �� � ������
    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++){
            if (this->matrix[y][x] == label) {
                Destination�oordinates.push_back(new Coordinate(x, y, distancePoints({x, y}, tmp)));
            }
        }
    }
 
    //��������� �� �������������� 
    for (int i = 0; i < Destination�oordinates.size(); i++) {
        for (int j = 0; j < Destination�oordinates.size() - 1; j++) {
            if (Destination�oordinates[j]->length > Destination�oordinates[j + 1]->length)
                swap(Destination�oordinates[j], Destination�oordinates[j + 1]);
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

    //����� ����������� �����, �������� ����, ������� ��� ���� ���-�� ������� ��� ������ ������ ����� ����� ����.
    //��������� ���� �������� ��� ����� �������, �������������, �������.
 
    int max = width > height ? width : height;
    int min = width < height ? width : height;

    //����� ��� ��������������, ���-�� �������� ������ ����������
    double persentMax = 100.0 / max;
    double persentMin = 100.0 / min;

    //����� ��� ������������ ��������� ���� ��� ��������� �����
    double currentPersent = 0;

    //��� ������� Y,  ���� width ������ ��� ����� heigth
    

    //����� ���-�� ������� ��������� ���������� 
    int middleMax = max / 2;
    int middleMin = min / 2;
 
    double p = 0.2;
    for (int l = middleMax, g = 0; l < max; l++, g++) { //����� ��������� ��������� ����������, �� ��������� �� 1
        currentPersent = 0; //�������� ����� ������ ���������
        int z = 1;

        for (int i = middleMax + g; i < max; i++, currentPersent += persentMax) { //���� �� ����� ������� ��������, ���� ��� �������������

            for (int k = 0; k < min; k++, z++) {//������� � ���������� ����������� ������� Y (���� width(x) ������ height(y))
                if (z * persentMin > currentPersent) break;
            }
   
            if (width >= height) { //���� ��� ������� ��� �������������, � �������� ������ ������
                this->pointMatrix[z - 1][i] = p;     
                this->pointMatrix[z - 1 + middleMin + g][i - middleMax - g] = p;

            }
            else {//����� ������ �������� � �����������, ���� ������ ������ ������.
                this->pointMatrix[i][z - 1] = p;
                this->pointMatrix[i - middleMax - g][z - 1 + middleMin + g] = p;
            }

        }
        p += 0.25;
    }
}
 

void Bot::move(vector<Coordinate*>& path)
{
    findingAWayForBot(path); //����� ������������ ���� ����������   

    //���� �� ������ �� ������ ����������,
    this->IncreaseProgress(path);
     
}

//���� ��� ���������� ������� ��� ����� �� ����� �����
bool Bot::CheckingForProhibition(Coordinate c) {
    for (int i = 0; i < this->idxProgress; i++) 
        if (*Destination�oordinates[i] == c)  return true;
        
    return false;
}


//�������� �� �������
bool Bot::isWin(){
    if (this->Destination�oordinates.size() == this->idxProgress)
        return true;
    return false;
}

//���� �� ��������� ������ ������, �� ����� ��������� ��� ����
void Bot::IncreaseProgress(vector<Coordinate*>& path) {
      
    if (*path[path.size() - 1] == *this->Destination�oordinates[this->idxProgress]) { 
        while (true) {
            this->idxProgress++;
            //���� �� ����� �� �����
            if (this->idxProgress == this->Destination�oordinates.size()) return;

            int y = this->Destination�oordinates[this->idxProgress]->y;
            int x = this->Destination�oordinates[this->idxProgress]->x;

            if (matrix[y][x] == this->getLabel() && *this->Destination�oordinates[this->idxProgress] != *path[0]) {
                continue;
            }
            break;
        }
        //��� ������ �� ��� ��������, �� ��������.
    }
}

void Bot::findingAWayForBot(vector<Coordinate*>& path) { //����� ������������ ����

    double mainUtilityFactor = 0; //����� ���-�� ������� ��� ���� ���������  (���-�� � �������� �������, ���������� ����)

    vector<Coordinate*> savedPath;  //���-�� ������� ������� �������
    vector<Coordinate*> currentPath; //���-�� ������� ������� � ������� ������

    //������� ��������� ����� ������
    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++) {
             
            if (matrix[y][x] == this->getLabel()) {

                if (CheckingForProhibition({ x, y })) continue;

                double savedUtilityFactor = 0; //������� ���������� ���-�� ������� ������� ��� ���� (���-�� � �������� �������, ���������� ����)

                currentPath.push_back(new Coordinate(x, y)); //���������� ��������� ������, �� ������� ����� ������ ��������� ������� ����

                findingAWayForBot(currentPath, savedPath, savedUtilityFactor);


                if (savedUtilityFactor > mainUtilityFactor) { //���� ����������� ���������� ������, �� �������������� ��� ����� ����������� ���
                    this->clearPath(path);

                    mainUtilityFactor = savedUtilityFactor;
                    for (int i = 0; i < savedPath.size(); i++)
                        path.push_back(new Coordinate(*savedPath[i]));

                }

                this->clearPath(currentPath); //������� ������� ����� ������� ������, ��� ���������
                this->clearPath(savedPath);
            }
        }
    }
 
 
    if (mainUtilityFactor <= 0 || path.size() == 2) { //���� ��� ���� ������ ��� ���

        if (path.size() == 2 && mainUtilityFactor > 0) { //���� ��� ���� ������, � ��� � ���� ������������� - ��������� �������� �������� ������, ������ �� ���� ������ ��� ������� ����� ����
            int randNum = rand() % 100; 
             if (randNum >= 50) return;
        }

        vector<Coordinate*> vector;

        for (int y = 0; y < this->height; y++) { //�������� ��� ���������� �� ������� �����
            for (int x = 0; x < this->width; x++) { //����� ������ �����, ���-�� ������ ����� ����� ����� �����
                if (matrix[y][x] == getLabel()) {
                    if (CheckingForProhibition({ x, y })) continue; //�������� ��, ������� ����� �� ������ �����
                    vector.push_back(new Coordinate(x, y));
                }
            }
        }

        //���������, ���-�� ����� ���������� ������� ����� ���������, ��� ���� ���-�� �� ������������ ����� �� �����.
        for (int i = 0; i < vector.size(); i++) {
            for (int j = 0; j < vector.size() - 1; j++) {
                if (distancePoints(*vector[j], this->getDirection()) < distancePoints(*vector[j + 1], this->getDirection())) {
                    swap(vector[j], vector[j + 1]);
                }
            }
        }
         
        //���� �� ����� ����� ����� ��������� �� ������� ����
        for (int i = 0; i < vector.size(); i++) {
            if (singleStroke(vector[i]->x, vector[i]->y, path, 0, true)) {                            
                return;
            }
        }

        //���� �� ����������, ������� ������� ������������� ���. (��� ������� ��� ���������)
        for (int i = 0; i < vector.size(); i++) {
            if (singleStroke(vector[i]->x, vector[i]->y, path, 0)) { //���� ��� ������������� ������                             
                return;
            }
        }   

        //���� �� ����������, ������� ������ ������������� ���
        for (int i = 0; i < vector.size(); i++) {
            if (singleStroke(vector[i]->x, vector[i]->y, path, -9999)) { //���� ��� ������������� ������                             
                return;
            }
        }   
              
     
    } 
 

}


void Bot::findingAWayForBot(vector<Coordinate*>& currentPath, vector<Coordinate*>& savedPath, double& savedUtilityFactor)
{

    //���������� ����������� ���������� ������ ����
    double utilityFactor = calculateUtilityFactor(*currentPath[0], *currentPath[currentPath.size() - 1], this->getDirection());

   
    if (utilityFactor > savedUtilityFactor) { //���� ����������� ���������� ������ ��� � �����������, �� ��������� ���� ���� ��� ����� �����������
        savedUtilityFactor = utilityFactor;
        this->clearPath(savedPath); //�������� ���������� ����
        for (int i = 0; i < currentPath.size(); i++) //�������� ������� ��� ����� �����������
            savedPath.push_back(new Coordinate(*currentPath[i]));
    }


    int x = currentPath[currentPath.size() - 1]->x;
    int y = currentPath[currentPath.size() - 1]->y;

    //���� �� �� ����� �� ������� ���� � ����� �������
    if (y + 2 < this->height && matrix[y + 1][x] != 0 && matrix[y + 2][x] == 0) { //��� ����

        if (Looping�heck(currentPath, x, y + 2)) { //���� �� ����������� �� ��� ����������
            currentPath.push_back(new Coordinate(x, y + 2)); //�������� � ����� ����������

            findingAWayForBot(currentPath, savedPath, savedUtilityFactor); //������� ������ ��������� ���

            //����� ���������, ������������ �����
            delete currentPath[currentPath.size() - 1]; //��������� ������
            currentPath.erase(currentPath.end() - 1);//������� �� ������� ��������� �� ������� ���������
        }

    }

    
    if (x - 2 >= 0 && matrix[y][x - 1] != 0 && matrix[y][x - 2] == 0) { //��� �����
        if (Looping�heck(currentPath, x - 2, y)) {
            currentPath.push_back(new Coordinate(x - 2, y));


            findingAWayForBot(currentPath, savedPath, savedUtilityFactor);

            delete currentPath[currentPath.size() - 1];
            currentPath.erase(currentPath.end() - 1);
        }
    }


    if (y - 2 >= 0 && matrix[y - 1][x] != 0 && matrix[y - 2][x] == 0) { //��� �����
        if (Looping�heck(currentPath, x, y - 2)) {
            currentPath.push_back(new Coordinate(x, y - 2));

            findingAWayForBot(currentPath, savedPath, savedUtilityFactor);

            delete currentPath[currentPath.size() - 1]; 
            currentPath.erase(currentPath.end() - 1);
        }
    }

    if (x + 2 < this->width && matrix[y][x + 1] != 0 && matrix[y][x + 2] == 0) { //��� ������
        if (Looping�heck(currentPath, x + 2, y)) {
            currentPath.push_back(new Coordinate(x + 2, y));

            findingAWayForBot(currentPath, savedPath, savedUtilityFactor);

            delete currentPath[currentPath.size() - 1]; 
            currentPath.erase(currentPath.end() - 1); 
        }
    }
    return;

}



double Bot::calculateUtilityFactor(Coordinate start, Coordinate current, Coordinate finish) {
  
    //start - ���������� ������ �������� ���
    //current - ���������� ���� �������
    //finish - ���������� ���� ���������

    //�������� ������� ����� �� ������ �� ���������
    double res1 = distancePoints(start, finish);

    // ��������� ���������� �� ����������� ����� �� ���������
    double res2 = distancePoints(current, finish);
    
    if (res2 > res1) return 0;

    //������� ���� �������, � ����� ���������� ���������� � ��������� 
    return (res1 - res2) - this->pointMatrix[current.y][current.x];;
}


void Bot::clearPath(vector<Coordinate*>& path)
{
    for (int i = 0; i < path.size(); i++)
        delete path[i];
    path.clear();
}

bool Bot::singleStroke(int x, int y, vector<Coordinate*>& path, int endMove, bool dc) {


    if (dc && x - 1 >= 0 && matrix[y][x - 1] == 0 && *this->Destination�oordinates[this->idxProgress] == Coordinate{x - 1, y} ||
        x - 1 >= 0 && matrix[y][x - 1] == 0 && calculateUtilityFactor({ x, y }, { x - 1, y }, this->getDirection()) > endMove) {//left
   
            clearPath(path);//������� �������  
            path.push_back(new Coordinate(x, y)); //���������� ������ ����                      
            path.push_back(new Coordinate(x - 1, y));//���������� ����� ����  
       
        return true;
    }

    if (dc && x + 1 < this->width && matrix[y][x + 1] == 0 && *this->Destination�oordinates[this->idxProgress] == Coordinate{x + 1, y} ||
         x + 1 < this->width && matrix[y][x + 1] == 0 && calculateUtilityFactor({ x, y }, { x + 1, y }, this->getDirection()) > endMove) {//right
   
            clearPath(path);
            path.push_back(new Coordinate(x, y));                      
            path.push_back(new Coordinate(x + 1, y));
       
        return true;
    }


    if (dc && y - 1 >= 0 && matrix[y - 1][x] == 0 && *this->Destination�oordinates[this->idxProgress] == Coordinate{x, y - 1} ||
        y - 1 >= 0 && matrix[y - 1][x] == 0 && calculateUtilityFactor({ x, y }, { x, y - 1 }, this->getDirection()) > endMove) {//top            
            clearPath(path);
            path.push_back(new Coordinate(x, y));                     
            path.push_back(new Coordinate(x, y - 1));
            return true;     
    }


    if (dc && y + 1 < this->height && matrix[y + 1][x] == 0 && *this->Destination�oordinates[this->idxProgress] == Coordinate{x, y + 1} ||
        y + 1 < this->height && matrix[y + 1][x] == 0 && calculateUtilityFactor({ x, y }, { x, y + 1 }, this->getDirection()) > endMove) {//bottom
        
            clearPath(path);
            path.push_back(new Coordinate(x, y));                   
            path.push_back(new Coordinate(x, y + 1)); 
            return true;
    }
    return false;

}


//��������� ��� �� ���� �� �����������, ���-�� �� ������ �� �����
bool Bot::Looping�heck(vector<Coordinate*>& path, int x, int y)
{
    //x,y ������ �� ������� �� ���������� �������
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


