#include "Game.h" 

void Game::menu_1()
{
	cout << "�������� ������� ����:" << endl;

	cout << "1) 9 X 9 (�� 9 �����)" << endl;
	cout << "2) 12 X 12 (�� 25 �����)" << endl;
	cout << "3) 15 X 15 (�� 36 �����)" << endl; 
	cout << "0) ����� ������� ���." << endl;
}

void Game::menu_2()
{
	cout << "�������� ����� ����:" << endl;

	cout << "1) ����� - �����" << endl;
	cout << "2) ����� - ���������" << endl;
	cout << "3) ��������� - ���������" << endl;
}

void Game::enteringMatrixParameters(int& width, int& height, int& countFigures)
{
	while (true) {
		int data = _getch(); //����������� ������� ������

		//������� �� ������ � ������� Ascii
		if (data == 49) {  //9 X 9
			width = 9, height = 9, countFigures = 9; break;
		}

		else if (data == 50) {  //12 X 12
			width = 12, height = 12, countFigures = 25;  break;
		}

		else if (data == 51) {  //15 X 15
			width = 15, height = 15, countFigures = 36;  break;
		} 

		else if (data == 48) {  //������������ ������		   
			cout << "������� ������ ���� (������, ������): ";
			cin >> width >> height;

			cout << "������� ���-�� �����:";
			cin >> countFigures;
			//��������� �� ������������ ����
			break;
		}
	}
}

void Game::enteringGameModeParameters()
{
	while (true) { //������� �������
		int data = _getch();
		if (data == 49) {  //����� - �����
			this->player_1 = new User(this->matrix, PlayerNumber::One);
			this->player_2 = new User(this->matrix, PlayerNumber::Two);
			break;
		}

		else if (data == 50) {  //����� - ���������
			this->player_1 = new Bot(this->matrix, PlayerNumber::One);
			this->player_2 = new User(this->matrix, PlayerNumber::Two);
			break;
		}

		else if (data == 51) {  //��������� - ���������
			this->player_1 = new Bot(this->matrix, PlayerNumber::One);
			this->player_2 = new Bot(this->matrix, PlayerNumber::Two);
			break;
		}
	}
}

void Game::start(){
	try {
		this->menu_1(); //���� ������ ������� ����

		int width, height, countFigures; //��������� �������

		this->enteringMatrixParameters(width, height, countFigures); //��������� ���������� �������

		this->matrix = new Matrix(width, height); //�������������� �������

		this->menu_2(); //���� ������ ������ ����

		//�����!!! ����������� ����� ������ ���� �� ���������� �������, ��� ��� bot ������ �������� ��� � ������� ��� �����
		this->matrix->setFigures(countFigures); //����������� ������ �� �����

		this->enteringGameModeParameters();

		this->run();//��������� ����
	}
	catch (Exception e) {
		if (e.code == ErrorCode::Warning) {
			cout << "��������������! " << endl;
			cout << e.text << endl;
		}
		else {
			cout << "������! " << endl;
			cout << "file: " << e.file << endl;
			cout << "line: " << e.line << endl;
			cout << "text: " << e.text << endl;
			abort();
		}
	}
	catch (...) {
		cout << "��������� ������ �������!" << endl;
		abort();
	}



}



void Game::run()
{
	vector<Coordinate*> path; //������� ������ ��� ����� �������� ���� �� ������� �� �������� �����
	system("cls");
	this->matrix->show(); 
	srand(time(NULL));

	
	while (true) {  //������� ����

		try {
			cout << "����� ����� 1" << endl;
			player_1->move(path);		 
			this->matrix->animate(path);  //�������� �������� �� �������
			
			//�������� �� ������
			if (player_1->isWin()) {
				cout << "Player 1 is win" << endl;
				break;
			}

			clearPath(path);//������� �������

			cout << "����� ����� 2" << endl;
			player_2->move(path);
		
			this->matrix->animate(path);  //�������� �������� �� ������� 
		    //�������� �� ������
			if (player_2->isWin()) {
				cout << "Player 2 is win" << endl;
				break;
			}
			clearPath(path);//������� �������

			//�������� �� ��, ��� ���-�� �������
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

 


