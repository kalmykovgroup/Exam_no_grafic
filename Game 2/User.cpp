#include "User.h" 

User::User(Matrix* matrix, PlayerNumber playerNumber) : matrix{ matrix->getArray() }, width{ matrix->getWidth() }, height{ matrix->getHeight() }, playerNumber{ playerNumber }
{
    //���������� ���� �� ������� ����� ������ ���-�� ��������� ����
    int label = this->playerNumber == PlayerNumber::Two ? (int)PlayerNumber::One : (int)PlayerNumber::Two;

    //��������� �� � ������
    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++) {
            if (this->matrix[y][x] == label) {
                Destination�oordinates.push_back(new Coordinate(x, y));
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
                    cout << "������, ��������� ����!" << endl;
                    continue;
                }    
            break;
        } 
    }
     
    catch (...) {
        throw Exception{__LINE__, __FILE__, ErrorCode::Fatal, "getData() - �� ��������� ������"};
    }
   
   
}

void User::move(vector<Coordinate*>& path)
{
    while (true) {
        Coordinate* beginPath = new Coordinate; //���������� ������ ��������
        Coordinate* endPath = new Coordinate;// ���������� ���� �������

        cout << "������� ���������� (��� ����������): ";
        this->getData(beginPath, true);

        cout << "������� ���������� (���� ����������): ";
        this->getData(endPath);

        path.push_back(beginPath); //���������� ������ ����

        if (!checkingMove(this->getLabel(), path, endPath)) { //��������� ��� ����� ��� �������� � ���� �������� ���������� � ������ �������� �� �������            
            Game::clearPath(path);
            cout << "�� ���������� ���" << endl;
            continue;

        } 
        break;
    }
}

bool User::checkingMove(int label, vector<Coordinate*>& path, Coordinate* finish)
{
    try {
        //���� �� ���� ����� ������ || ������� ��� �� ����� ������� || �� ������� ������

        if (finish == path[0] || matrix[finish->y][finish->x] != 0 || matrix[path[0]->y][path[0]->x] != label)
            return false;

        //���� ���� �� ��������� ������� �� ����� ����
        if (*path[0] < 0 || *finish < 0 || path[0]->x > this->width || path[0]->y > this->height || finish->x > this->width || finish->y > this->height)
            return false;


        //���� �������� �� ���� ��������, 
        if (finish->y == path[0]->y && (finish->x == path[0]->x - 1 || finish->x == path[0]->x + 1) ||  //�����, ������
            finish->x == path[0]->x && (finish->y == path[0]->y + 1 || finish->y == path[0]->y - 1)) { //�����, ����          
            path.push_back(finish);
            return true;
        }

        if (findingAWay(path, finish)) //���� ���� ������ 
            return true;
        else
            return false;
    }
    catch (...) {
        throw Exception{__LINE__, __FILE__, ErrorCode::Fatal, "checkingMove() - �� ��������� ������"};
    }
    

}

bool User::findingAWay(vector<Coordinate*>& path, Coordinate* finish)
{
    if (*path[path.size() - 1] == *finish) //���� ���� ������
        return true;

    Coordinate currentP(*path[path.size() - 1]);

    //���� �� ����� �� ������, ���� ����� ������� � ��������� ����� ����� ���
    if (currentP.y + 2 < this->height && matrix[currentP.y + 1][currentP.x] != 0 && matrix[currentP.y + 2][currentP.x] == 0) { //��� ����
        //����� ���� �� �� ������ �� �������� ����� �� ����� ����, �� ������� ���������, ����� ���� ������
        if (findingAWay(path, finish, new Coordinate{ currentP.x, currentP.y + 2 })) return true;
    }

    if (currentP.x - 2 >= 0 && matrix[currentP.y][currentP.x - 1] != 0 && matrix[currentP.y][currentP.x - 2] == 0) { //��� �����
        if (findingAWay(path, finish, new Coordinate{ currentP.x - 2, currentP.y })) return true;
    }


    if (currentP.y - 2 >= 0 && matrix[currentP.y - 1][currentP.x] != 0 && matrix[currentP.y - 2][currentP.x] == 0) { //��� �����
        if (findingAWay(path, finish, new Coordinate{ currentP.x, currentP.y - 2 })) return true;

    }

    if (currentP.x + 2 < this->width && matrix[currentP.y][currentP.x + 1] != 0 && matrix[currentP.y][currentP.x + 2] == 0) { //��� ������
        if (findingAWay(path, finish, new Coordinate{ currentP.x + 2, currentP.y })) return true;
    }

    return false;

}

bool User::findingAWay(vector<Coordinate*>& path, Coordinate* finish, Coordinate* newPlace)
{
    try{
        //newPlace ������ �� ������� �� ���������� �������
        bool flag = true;
        for (int i = 0; i < path.size(); i++) { //��������� ��� �� ���� �� �����������, ���-�� �� ������ �� �����
            if (*path[i] == *newPlace) { flag = false; break; }
        }

        if (flag) { //���� �� �� �������� ��� ������
            path.push_back(newPlace); //���������� ��� ������������ ��������� �������, �� ���� ��������� ������
            if (findingAWay(path, finish)) return true; //���� ����� �� �� ������� �������

            path.erase(path.end() - 1);//�� ������� ���� ����, ������������ � ������� ������
        }

        delete newPlace;
        return false;
    }
    catch (...) {
        throw Exception{__LINE__, __FILE__, ErrorCode::Fatal, "findingAWay() - �� ��������� ������"};
    }
   
}

bool User::isWin()
{
     for(int i = 0; i < this->Destination�oordinates.size(); i++)
         if(this->matrix[Destination�oordinates[i]->y][Destination�oordinates[i]->x] != this->getLabel())
        return false;
    return true;
}


