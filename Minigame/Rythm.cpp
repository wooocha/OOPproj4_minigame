#pragma comment(lib,"winmm.lib")
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>
#include "Rythm.h"

#define WINDOW_WIDTH 100
#define WINDOW_HEIGHT 21
#define ARROW_NUM 224
#define MAXIMUM_LEVEL 15

using namespace std;


namespace R {
	// ���� ���¸� ��Ÿ���� ONGOING�� ������ ���¿����� ��� ������ ����Ǿ�� ��.


	// ����� �Է¿� ���� �ڵ尪
	enum KEYBOARD {
		UP = 72,
		LEFT = 75,
		RIGHT = 77,
		DOWN = 80
	};
	// P1 : (0,1) ~ (79, 9) ����Ű / P2 : (0, 11) ~ (79, 19) wasd // y+10 ����

	void setConsoleView() {
		// ���� ����
		system("title Keyboard Battle");
		// â ũ�� ����
		char* command = new char[30];
		sprintf_s(command, 30, "mode con: lines=%d cols=%d", WINDOW_HEIGHT, WINDOW_WIDTH);
		system(command);
		delete[] command;
		// â ũ�� ����
		HWND consoleWindow = GetConsoleWindow();
		SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
		// ������� ����
		SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS);

		// Ŀ�� ����
		CONSOLE_CURSOR_INFO cursorInfo = { 0, };
		cursorInfo.dwSize = 1;
		cursorInfo.bVisible = FALSE;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);


	}

	void gotoxy(int x, int y)
	{
		COORD Pos;
		Pos.X = x;
		Pos.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
	}
}
using namespace R;

//Player class
void Player::isPressed() {
	++numOfInput;
}
void Player::minusLife() {
	--life;
}
void Player::nextLevel() {
	++level;
}
void Player::clear() {
	answervec.clear();
	questionstr.clear();
	answerstr.clear();
	numOfInput = 0;
}
bool Player::isOver() const {
	return life < 1;
}
bool Player::isWin() const {
	return getLevel() > MAXIMUM_LEVEL;
}




// ���� ���� �Ǵ� ����� �� �غ�ð� �ο�
void Rythm::printTimer(int second) {
	clock_t prevtime, curtime;
	prevtime = clock();
	for (int i = 0; i <= second; i++) {
		while (difftime(curtime = clock(), prevtime) < 1000 * (i + 1)) {};
		gotoxy(WINDOW_WIDTH / 2 - 5, WINDOW_HEIGHT / 2);
		cout << second - i;
	}
}

// ���ӿ����� ��Ÿ���� ��¿� �Լ�
void Rythm::gameOver(const Player& p) {
	DWORD dw;
	COORD pos = p.getStart();
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', WINDOW_WIDTH * (WINDOW_HEIGHT - 3) / 2, pos, &dw);
	gotoxy(0, pos.Y + 2);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	cout << "           ����       ��      ��    ��  �����    ���   ��      ��  �����  ����" << "\n"
		<< "         ��            ���     �����  ��        ��    ��  ��    ��   ��        ��   ��" << "\n"
		<< "         ��   ���    ��  ��    �� �� ��  �����  ��    ��   ��  ��    �����  ����" << "\n"
		<< "          ��   ��    �����   ��    ��  ��        ��    ��    ���     ��        ��   ��" << "\n"
		<< "           ����   ��      ��  ��    ��  �����    ���       ��      �����  ��    ��";

	(pos.Y < 10) ? pos.Y += 10 : pos.Y -= 10;
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', WINDOW_WIDTH * (WINDOW_HEIGHT - 3) / 2, pos, &dw);
	gotoxy(0, pos.Y + 2);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	cout << "                                     ��    ��  �����  ��    ��" << endl
		<< "                                     �� �� ��    ���    ���  ��" << endl
		<< "                                     �� �� ��    ���    �� �� ��" << endl
		<< "                                      ����     ���    ��  ���" << endl
		<< "                                      ��  ��   �����  ��    ��";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

// ���ڸ� ��Ÿ���� ��¿� �Լ�
void Rythm::gameWin(const Player& p) {
	DWORD dw;
	COORD pos = p.getStart();
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', WINDOW_WIDTH * (WINDOW_HEIGHT - 3) / 2, pos, &dw);
	gotoxy(0, pos.Y + 2);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	cout << "                                     ��    ��  �����  ��    ��" << endl
		<< "                                     �� �� ��    ���    ���  ��" << endl
		<< "                                     �� �� ��    ���    �� �� ��" << endl
		<< "                                      ����     ���    ��  ���" << endl
		<< "                                      ��  ��   �����  ��    ��";

	(pos.Y < 10) ? pos.Y += 10 : pos.Y -= 10;
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', WINDOW_WIDTH * (WINDOW_HEIGHT - 3) / 2, pos, &dw);
	gotoxy(0, pos.Y + 2);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	cout << "                                 ��         ���    �����  �����" << endl
		<< "                                 ��       ��    ��  ��        ��" << endl
		<< "                                 ��       ��    ��  �����  �����" << endl
		<< "                                 ��       ��    ��        ��  ��" << endl
		<< "                                 �����   ���    �����  �����";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

// 0~MAXIMUM_LEVEL(����� 20) ���̿� ���ϴ� ������ �� 20���� �޾� �迭ũ�� ����
// �迭�� ũ�� = ���� ȭ��ǥ�� ����
// sort ����Ͽ� �迭 ũ�⸦ ������������ ����
// ����� �� �ϳ��� �������� �� �迭�� �����Ͽ� ���� ����
void Rythm::makeArrow(vector<int> v[]) {
	srand((unsigned int)time(NULL));
	int n;
	int temp[MAXIMUM_LEVEL];

	for (int i = 0; i < MAXIMUM_LEVEL; i++) {
		n = rand() % MAXIMUM_LEVEL + 1;
		temp[i] = n;
	}
	sort(temp, temp + MAXIMUM_LEVEL);

	for (int i = 0; i < MAXIMUM_LEVEL; i++) {
		for (int j = 0; j < temp[i]; j++) {
			n = rand() % 4;
			switch (n) {
			case 0:
				v[i].push_back(UP);
				break;
			case 1:
				v[i].push_back(DOWN);
				break;
			case 2:
				v[i].push_back(LEFT);
				break;
			case 3:
				v[i].push_back(RIGHT);
				break;
			}
		}

	}

}

// �÷��̾� level ���� ���� ���� ǥ��
void Rythm::printArrow(const vector<int>* v, Player& p) {
	if (p.isWin()) return;
	vector<int>::const_iterator it;
	int index = p.getLevel() - 1;
	string questionStr = p.getQstr();
	for (it = v[index].begin(); it != v[index].end(); it++) {
		switch (*it) {
		case UP:
			questionStr += "�� ";
			break;
		case DOWN:
			questionStr += "�� ";
			break;
		case LEFT:
			questionStr += "�� ";
			break;
		case RIGHT:
			questionStr += "�� ";
			break;
		}
	}
	gotoxy(p.getStart().X + 20, p.getStart().Y + 3);
	cout << questionStr;

}

// �÷��̾��� �� �Է¿� ���� ���� ����
void Rythm::checkAnswer(vector<int> v[], Player& p) {
	int index = p.getLevel() - 1;
	int inputNum = p.getNumOfInput();
	vector<int> currentInput = p.getAvec();

	if (v[index].size() <= inputNum) return;
	if (v[index][inputNum] != currentInput[inputNum]) p.minusLife();
	p.isPressed();
}

// �÷��̾� ���� ���� ���� life ���� ǥ��
void Rythm::drawInfo(const Player& p) {
	gotoxy(p.getStart().X, p.getStart().Y + 1);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	for (int i = 0; i < p.getLife(); i++) {
		cout << "��";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	for (int i = 0; i < 3 - p.getLife(); i++) {
		cout << "��";
	}
	gotoxy(WINDOW_WIDTH - 20, p.getStart().Y + 1);
	cout << "Level : " << p.getLevel() << " / " << MAXIMUM_LEVEL;

}

void Rythm::drawMap() {
	gotoxy(0, 0);
	for (int i = 0; i < WINDOW_WIDTH / 2; i++) {
		cout << "��";
	}
	gotoxy(0, WINDOW_HEIGHT - 1);
	for (int i = 0; i < WINDOW_WIDTH / 2; i++) {
		cout << "��";
	}

	gotoxy(0, (WINDOW_HEIGHT - 1) / 2);
	for (int i = 0; i < WINDOW_WIDTH; i++) {
		cout << "=";
	}

	gotoxy(0, 4);
	cout << "����    ���" << endl
		<< "��   �� ����" << endl
		<< "����    ���" << endl
		<< "��        ���" << endl
		<< "��        ���";

	gotoxy(0, 14);
	cout << "����  ����" << endl
		<< "��   ��     ��" << endl
		<< "����  ����" << endl
		<< "��      ��" << endl
		<< "��      ����";

	gotoxy(WINDOW_WIDTH - 10, 7);
	cout << "��";
	gotoxy(WINDOW_WIDTH - 12, 8);
	cout << "��";
	gotoxy(WINDOW_WIDTH - 10, 8);
	cout << "��";
	gotoxy(WINDOW_WIDTH - 8, 8);
	cout << "��";

	gotoxy(WINDOW_WIDTH - 10, 17);
	cout << "W";
	gotoxy(WINDOW_WIDTH - 12, 18);
	cout << "A";
	gotoxy(WINDOW_WIDTH - 10, 18);
	cout << "S";
	gotoxy(WINDOW_WIDTH - 8, 18);
	cout << "D";

	gotoxy(16, 5);
	cout << ">>>";
	gotoxy(16, 15);
	cout << ">>>";
}

// ������� ����Ű �Է¼��� ���� �������� �������� ��Ȳ
bool Rythm::isNext(const vector<int> v[], const Player& p) {
	int index = p.getLevel() - 1;
	int inputNum = p.getNumOfInput();
	return v[index].size() <= inputNum;
}

// �Է� ĭ ���� ���� �ܰ� ����
void Rythm::setNext(const vector<int> v[], Player& p) {
	DWORD dw;
	COORD pos = { p.getStart().X + 20, p.getStart().Y + 3 };
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', MAXIMUM_LEVEL * 3, pos, &dw);
	pos.Y++;
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', MAXIMUM_LEVEL * 3, pos, &dw);
	p.clear();
	p.nextLevel();
}

// ���п��θ� Ȯ���Ͽ� ���� ���� ���� ������
GAME_STATE Rythm::getState(const Player& p1, const Player& p2) {
	if (p1.isWin()) return P1WIN;
	if (p2.isWin()) return P2WIN;
	if (p1.isOver()) return P1OVER;
	if (p2.isOver()) return P2OVER;
	return ONGOING;
}

void Rythm::gameScreen() {
	setConsoleView();
	system("cls");
	GAME_STATE state = ONGOING;
	Player p1, p2(1);

	vector<int> questionvec[MAXIMUM_LEVEL]; // ��ü ������Ʈ�� �̸� ����, �� �÷��̾� ������ ���� ���� ����
	int input = 0;
	bool valid;

	printTimer(3);
	PlaySound(TEXT("8BitAdventure.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
	drawMap();
	drawInfo(p1);
	drawInfo(p2);
	makeArrow(questionvec);
	printArrow(questionvec, p1);
	printArrow(questionvec, p2);
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); // Sleep ���� �Է� ����

	while (state == ONGOING) {
		input = 0;
		valid = true;
		if (_kbhit()) {
			input = _getch();
			if ((input == ARROW_NUM) && !p1.isOver()) {
				switch (_getch()) {
				case UP:
					p1.getAvec().push_back(UP);
					p1.getAstr() += "�� ";
					break;
				case DOWN:
					p1.getAvec().push_back(DOWN);
					p1.getAstr() += "�� ";
					break;
				case LEFT:
					p1.getAvec().push_back(LEFT);
					p1.getAstr() += "�� ";
					break;
				case RIGHT:
					p1.getAvec().push_back(RIGHT);
					p1.getAstr() += "�� ";
					break;
				default:
					valid = false;
					break;
				}
				// ����游�� ��ȿ������ �����ϰ� ���� ȭ��ǥ ���� == ��ȿ �Է¼��̸� ���� ���� ���
				if (valid) {
					gotoxy(p1.getStart().X + 20, p1.getStart().Y + 4);
					cout << p1.getAstr();
					checkAnswer(questionvec, p1);
				}
			}

			if (!p2.isOver()) {
				switch (input) {
				case 'W':
				case 'w':
					p2.getAvec().push_back(UP);
					p2.getAstr() += "�� ";
					break;
				case 'S':
				case 's':
					p2.getAvec().push_back(DOWN);
					p2.getAstr() += "�� ";
					break;
				case 'A':
				case 'a':
					p2.getAvec().push_back(LEFT);
					p2.getAstr() += "�� ";
					break;
				case 'D':
				case 'd':
					p2.getAvec().push_back(RIGHT);
					p2.getAstr() += "�� ";
					break;
				default:
					valid = false;
					break;
				}
				// wasd(WASD)���� ��ȿ������ ����
				if (valid) {
					gotoxy(p2.getStart().X + 20, p2.getStart().Y + 4);
					cout << p2.getAstr();
					checkAnswer(questionvec, p2);
				}
			}
			if (isNext(questionvec, p1)) {
				setNext(questionvec, p1);
				printArrow(questionvec, p1);
			}
			if (isNext(questionvec, p2)) {
				setNext(questionvec, p2);
				printArrow(questionvec, p2);
			}
			drawInfo(p1);
			drawInfo(p2);
			// �÷��̾� ���� ��Ȳ�� ���� ���ӻ��� ������Ʈ
			state = getState(p1, p2);
			switch (state) {
			case ONGOING:
				break;
			case P1WIN:
				gameWin(p1);
				break;
			case P2WIN:
				gameWin(p2);
				break;
			case P1OVER:
				gameOver(p1);
				break;
			case P2OVER:
				gameOver(p2);
				break;
			}

		}
	}
	PlaySound(NULL, NULL, 0);
	// ���� ��� Ȯ��
	PlaySound(TEXT("win.wav"), NULL, SND_NODEFAULT | SND_FILENAME);
	PlaySound(NULL, NULL, 0);
	Sleep(3000);
}