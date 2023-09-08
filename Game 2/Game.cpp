#include "Game.h" 

void Game::menu_1()
{
	cout << "Выберите игровое поле:" << endl;

	cout << "1) 9 X 9 (по 9 шашек)" << endl;
	cout << "2) 12 X 12 (по 25 шашек)" << endl;
	cout << "3) 15 X 15 (по 36 шашек)" << endl; 
	cout << "0) Введу размеры сам." << endl;
}

void Game::menu_2()
{
	cout << "Выберите режим игры:" << endl;

	cout << "1) Игрок - Игрок" << endl;
	cout << "2) Игрок - Компьютер" << endl;
	cout << "3) Компьютер - Компьютер" << endl;
}

void Game::enteringMatrixParameters(int& width, int& height, int& countFigures)
{
	while (true) {
		int data = _getch(); //Отслеживаем нажатие кнопки

		//Сверяем по номеру в таблице Ascii
		if (data == 49) {  //9 X 9
			width = 9, height = 9, countFigures = 9; break;
		}

		else if (data == 50) {  //12 X 12
			width = 12, height = 12, countFigures = 25;  break;
		}

		else if (data == 51) {  //15 X 15
			width = 15, height = 15, countFigures = 36;  break;
		} 

		else if (data == 48) {  //Произвольный размер		   
			cout << "Введите размер поля (ширина, высота): ";
			cin >> width >> height;

			cout << "Введите кол-во шашек:";
			cin >> countFigures;
			//Проверить на корректность поля
			break;
		}
	}
}

void Game::enteringGameModeParameters()
{
	while (true) { //Создаем игроков
		int data = _getch();
		if (data == 49) {  //Игрок - Игрок
			this->player_1 = new User(this->matrix, PlayerNumber::One);
			this->player_2 = new User(this->matrix, PlayerNumber::Two);
			break;
		}

		else if (data == 50) {  //Игрок - Компьютер
			this->player_1 = new Bot(this->matrix, PlayerNumber::One);
			this->player_2 = new User(this->matrix, PlayerNumber::Two);
			break;
		}

		else if (data == 51) {  //Компьютер - Компьютер
			this->player_1 = new Bot(this->matrix, PlayerNumber::One);
			this->player_2 = new Bot(this->matrix, PlayerNumber::Two);
			break;
		}
	}
}

void Game::start(){
	try {
		this->menu_1(); //Меню выбора размера поля

		int width, height, countFigures; //Параметры матрицы

		this->enteringMatrixParameters(width, height, countFigures); //Получение параметров матрицы

		this->matrix = new Matrix(width, height); //Инициализируем матрицу

		this->menu_2(); //Меню выбора режима игры

		//Важно!!! Расстановка фигур должна быть до добавления игроков, так как bot должен понимать где и сколько его фигур
		this->matrix->setFigures(countFigures); //Расставляем фигуры на доске

		this->enteringGameModeParameters();

		this->run();//Запускаем игру
	}
	catch (Exception e) {
		if (e.code == ErrorCode::Warning) {
			cout << "Предупреждение! " << endl;
			cout << e.text << endl;
		}
		else {
			cout << "Ошибка! " << endl;
			cout << "file: " << e.file << endl;
			cout << "line: " << e.line << endl;
			cout << "text: " << e.text << endl;
			abort();
		}
	}
	catch (...) {
		cout << "Фатальная ошибка системы!" << endl;
		abort();
	}



}



void Game::run()
{
	vector<Coordinate*> path; //Создаем массив где будет хранится путь по клеткам до конечной точки
	system("cls");
	this->matrix->show(); 
	srand(time(NULL));

	
	while (true) {  //Игровой цыкл

		try {
			cout << "Ходит игрок 1" << endl;
			player_1->move(path);		 
			this->matrix->animate(path);  //Анимация движения по клеткам
			
			//Проверка на победу
			if (player_1->isWin()) {
				cout << "Player 1 is win" << endl;
				break;
			}

			clearPath(path);//Очищаем историю

			cout << "Ходит игрок 2" << endl;
			player_2->move(path);
		
			this->matrix->animate(path);  //Анимация движения по клеткам 
		    //Проверка на победу
			if (player_2->isWin()) {
				cout << "Player 2 is win" << endl;
				break;
			}
			clearPath(path);//Очищаем историю

			//Проверка на то, что кто-то выйграл
		}
		catch (...) {
			cout << "error" << endl;
		}

	}
}

void Game::clearPath(vector<Coordinate*>& path)
{
	for (int i = 0; i < path.size(); i++)
		delete path[i];
	path.clear();
}

 


