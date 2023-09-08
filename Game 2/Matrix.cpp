#include "Matrix.h"  


Matrix::Matrix(int width, int height) : field{ new int* [height] }, width{ width }, height{ height }{
    for (int i = 0; i < height; i++) //Создаем матрицу
        field[i] = new int[width];

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            this->field[i][j] = 0;     //Инициализитруем ее
}

void Matrix::show(Coordinate* begin, Coordinate* end, Coordinate* current)
{
   

        string navArr = "ABCDEFGHIKLMNOPQRSTVXYZ";

        char label;
        int label_1 = int(PlayerNumber::One);
        int label_2 = int(PlayerNumber::Two);

        ConsoleColor::SetTextColor::Cyan();
        cout << setw(3) << setfill(' ') << ' ';

        for (int k = 0; k < width; k++)
            cout << " " << navArr[k] << " ";
     
        ConsoleColor::SetTextColor::Yellow();
        cout << endl << "   " << setw(width * 3 - 1) << setfill('-') << "-" << endl;
        ConsoleColor::SetTextColor::White();

        for (int i = 0; i < height; i++) {

            for (int j = 0; j < width; j++) {

                if (j == 0) cout << i + 1 << " |";
            
                if (field[i][j] == label_1) {
                    ConsoleColor::SetTextColor::Blue();
                    label = label_1;
                }
                else if (field[i][j] == label_2) {
                    ConsoleColor::SetTextColor::Green();
                    label = label_2;
                }
                else label = '+';

                if (begin != nullptr && end != nullptr && current != nullptr) {
                
                        if (i == current->y && j == current->x) {
                            if (field[i][j] == label_1) {
                                ConsoleColor::SetTextColor::Cyan();
                            }
                            else ConsoleColor::SetTextColor::LightGreen();
                     }
                   
                }

                cout << " " << label << " ";
                ConsoleColor::SetTextColor::White();
            }
            cout << endl;

        }

}

void Matrix::animate(vector<Coordinate*>& path)
{ 
    try {
        int tmpNum = field[path[0]->y][path[0]->x];

        for (int i = 0; i < path.size() - 1; i++) {
            system("cls");
            field[path[i]->y][path[i]->x] = 0; //Убираем из текущей
            field[path[i + 1]->y][path[i + 1]->x] = tmpNum; //Ставим на следующую
            show(path[0], path[path.size() - 1], path[i + 1]); //Вывод поля с передачей доп. параметров
            Sleep(100);
        }

        system("cls");
        show();
    }
    catch (...) {
        cout << "error" << endl; 
    }
  
}

void Matrix::setFigures(int countFigures)
{
    int preWidth = 1, preHeight = 1;

    int width_ = 1, height_ = 1;

    int finalWidth, finalHeight;

    while (true) { //Находим оптимальное рассположение фигур
        if (width_ * height_ == countFigures || width_ * height_ > countFigures)
            break;
        else {
            preWidth = width_;
            preHeight = height_;
        }

        if (width_ == height_)
            width_++;
        else
            height_++;

    }

    int res1 = countFigures - preWidth * preHeight;
    int res2 = width_ * height_ - countFigures;
    if (res1 > res2) {
        finalWidth = width_;
        finalHeight = height_;
    }
    else {
        finalWidth = preWidth;
        finalHeight = preHeight;
    }
    int count = 0;
    int flag = false;
    for (int i = 0; i < height; i++) {
        if (flag) break;
        for (int j = 0; j < finalWidth; j++) {
            this->field[i][j] = PlayerNumber::One;
            if (++count == countFigures) { flag = true; break; }
        }
    }

    count = 0;
    flag = false;
    for (int i = height - 1; i != 0; i--) {
        if (flag) break;
        for (int j = 0; j < finalWidth; j++) {
            this->field[i][width - 1 - j] = PlayerNumber::Two;
            if (++count == countFigures) { flag = true; break; }
        }
    }


}


 