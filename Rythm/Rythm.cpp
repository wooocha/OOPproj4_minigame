#pragma comment(lib,"winmm.lib")
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>

#define WINDOW_WIDTH 100
#define WINDOW_HEIGHT 21
#define ARROW_NUM 224
#define MAXIMUM_LEVEL 15

using namespace std;

// 게임 상태를 나타내며 ONGOING을 제외한 상태에서는 모두 게임이 종료되어야 함.
enum GAME_STATE {
	ONGOING = 0,
	P1WIN,
	P2WIN,
	P1OVER,
	P2OVER
};

// ↑↓←→ 입력에 대한 코드값
enum KEYBOARD {
	UP = 72,
	LEFT = 75,
	RIGHT = 77,
	DOWN = 80
};
// P1 : (0,1) ~ (79, 9) 방향키 / P2 : (0, 11) ~ (79, 19) wasd // y+10 차이
class Player {
private:
	unsigned int life = 3;
	unsigned int level = 1;
	unsigned int numOfInput = 0;
	vector<int> answervec;
	string questionstr = "";
	string answerstr = "";
	COORD start = { 0, 1 }; // 맵 부분 제외하고 가용가능한 첫 좌표

public:
	Player() {}
	Player(short n) : start{ 0, 1 + 10 * n } {} // 플레이어 1,2 화면 구분
	~Player() {}
	unsigned int getLife() const {
		return life;
	}
	unsigned int getLevel() const {
		return level;
	}
	unsigned int getNumOfInput() const {
		return numOfInput;
	}
	vector<int>& getAvec() {
		return answervec;
	}
	string& getQstr() {
		return questionstr;
	}
	string& getAstr() {
		return answerstr;
	}
	COORD getStart() const {
		return start;
	}

	void isPressed() {
		++numOfInput;
	}
	void minusLife() {
		--life;
	}
	void nextLevel() {
		++level;
	}
	void clear() {
		answervec.clear();
		questionstr.clear();
		answerstr.clear();
		numOfInput = 0;
	}
	bool isOver() const {
		return life < 1;
	}
	bool isWin() const {
		return getLevel() > MAXIMUM_LEVEL;
	}
};

void setConsoleView() {
	// 제목 설정
	system("title Keyboard Battle");
	// 창 크기 지정
	char* command = new char[30];
	sprintf_s(command, 30, "mode con: lines=%d cols=%d", WINDOW_HEIGHT, WINDOW_WIDTH);
	system(command);
	delete[] command;
	// 창 크기 고정
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
	// 편집모드 해제
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS);

	// 커서 숨김
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

// 게임 시작 또는 재시작 시 준비시간 부여
void printTimer(int second) {
	clock_t prevtime, curtime;
	prevtime = clock();
	for (int i = 0; i <= second; i++) {
		while (difftime(curtime = clock(), prevtime) < 1000 * (i + 1)) {};
		gotoxy(WINDOW_WIDTH / 2 - 5, WINDOW_HEIGHT / 2);
		cout << second - i;
	}
}

// 게임오버를 나타내는 출력용 함수
void gameOver(const Player& p) {
	DWORD dw;
	COORD pos = p.getStart();
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', WINDOW_WIDTH * (WINDOW_HEIGHT - 3) / 2, pos, &dw);
	gotoxy(0, pos.Y + 2);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	cout << "           ■■■       ■      ■    ■  ■■■■    ■■   ■      ■  ■■■■  ■■■" << "\n"
		<< "         ■            ■■     ■■■■  ■        ■    ■  ■    ■   ■        ■   ■" << "\n"
		<< "         ■   ■■    ■  ■    ■ ■ ■  ■■■■  ■    ■   ■  ■    ■■■■  ■■■" << "\n"
		<< "          ■   ■    ■■■■   ■    ■  ■        ■    ■    ■■     ■        ■   ■" << "\n"
		<< "           ■■■   ■      ■  ■    ■  ■■■■    ■■       ■      ■■■■  ■    ■";

	(pos.Y < 10) ? pos.Y += 10 : pos.Y -= 10;
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', WINDOW_WIDTH * (WINDOW_HEIGHT - 3) / 2, pos, &dw);
	gotoxy(0, pos.Y + 2);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	cout << "                                     ■    ■  ■■■■  ■    ■" << endl
		<< "                                     ■ ■ ■    ■■    ■■  ■" << endl
		<< "                                     ■ ■ ■    ■■    ■ ■ ■" << endl
		<< "                                      ■■■     ■■    ■  ■■" << endl
		<< "                                      ■  ■   ■■■■  ■    ■";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

// 승자를 나타내는 출력용 함수
void gameWin(const Player& p) {
	DWORD dw;
	COORD pos = p.getStart();
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', WINDOW_WIDTH * (WINDOW_HEIGHT - 3) / 2, pos, &dw);
	gotoxy(0, pos.Y + 2);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	cout << "                                     ■    ■  ■■■■  ■    ■" << endl
		<< "                                     ■ ■ ■    ■■    ■■  ■" << endl
		<< "                                     ■ ■ ■    ■■    ■ ■ ■" << endl
		<< "                                      ■■■     ■■    ■  ■■" << endl
		<< "                                      ■  ■   ■■■■  ■    ■";

	(pos.Y < 10) ? pos.Y += 10 : pos.Y -= 10;
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', WINDOW_WIDTH * (WINDOW_HEIGHT - 3) / 2, pos, &dw);
	gotoxy(0, pos.Y + 2);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	cout << "                                 ■         ■■    ■■■■  ■■■■" << endl
		<< "                                 ■       ■    ■  ■        ■" << endl
		<< "                                 ■       ■    ■  ■■■■  ■■■■" << endl
		<< "                                 ■       ■    ■        ■  ■" << endl
		<< "                                 ■■■■   ■■    ■■■■  ■■■■";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

// 0~MAXIMUM_LEVEL(현재는 20) 사이에 속하는 무작위 수 20개를 받아 배열크기 지정
// 배열의 크기 = 문제 화살표의 개수
// sort 사용하여 배열 크기를 오름차순으로 정렬
// ↑↓←→ 중 하나를 무작위로 각 배열에 저장하여 문제 생성
void makeArrow(vector<int> v[]) {
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

// 플레이어 level 값에 따라 문제 표시
void printArrow(const vector<int>* v, Player& p) {
	if (p.isWin()) return;
	vector<int>::const_iterator it;
	int index = p.getLevel() - 1;
	string questionStr = p.getQstr();
	for (it = v[index].begin(); it != v[index].end(); it++) {
		switch (*it) {
		case UP:
			questionStr += "↑ ";
			break;
		case DOWN:
			questionStr += "↓ ";
			break;
		case LEFT:
			questionStr += "← ";
			break;
		case RIGHT:
			questionStr += "→ ";
			break;
		}
	}
	gotoxy(p.getStart().X + 20, p.getStart().Y + 3);
	cout << questionStr;

}

// 플레이어의 한 입력에 대한 정오 판정
void checkAnswer(vector<int> v[], Player& p) {
	int index = p.getLevel() - 1;
	int inputNum = p.getNumOfInput();
	vector<int> currentInput = p.getAvec();

	if (v[index].size() <= inputNum) return;
	if (v[index][inputNum] != currentInput[inputNum]) p.minusLife();
	p.isPressed();
}

// 플레이어 상태 값에 따라 life 정보 표시
void drawInfo(const Player& p) {
	gotoxy(p.getStart().X, p.getStart().Y + 1);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	for (int i = 0; i < p.getLife(); i++) {
		cout << "♥";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	for (int i = 0; i < 3 - p.getLife(); i++) {
		cout << "♡";
	}
	gotoxy(WINDOW_WIDTH - 20, p.getStart().Y + 1);
	cout << "Level : " << p.getLevel() << " / " << MAXIMUM_LEVEL;

}

void drawMap() {
	gotoxy(0, 0);
	for (int i = 0; i < WINDOW_WIDTH / 2; i++) {
		cout << "▦";
	}
	gotoxy(0, WINDOW_HEIGHT - 1);
	for (int i = 0; i < WINDOW_WIDTH / 2; i++) {
		cout << "▦";
	}

	gotoxy(0, (WINDOW_HEIGHT - 1) / 2);
	for (int i = 0; i < WINDOW_WIDTH; i++) {
		cout << "=";
	}

	gotoxy(0, 4);
	cout << "■■■    ■■" << endl
		<< "■   ■ ■■■" << endl
		<< "■■■    ■■" << endl
		<< "■        ■■" << endl
		<< "■        ■■";

	gotoxy(0, 14);
	cout << "■■■  ■■■" << endl
		<< "■   ■     ■" << endl
		<< "■■■  ■■■" << endl
		<< "■      ■" << endl
		<< "■      ■■■";

	gotoxy(16, 5);
	cout << ">>>";
	gotoxy(16, 15);
	cout << ">>>";
}

// 사용자의 방향키 입력수가 현재 문제보다 많아지는 상황
bool isNext(const vector<int> v[], const Player& p) {
	int index = p.getLevel() - 1;
	int inputNum = p.getNumOfInput();
	return v[index].size() <= inputNum;
}

// 입력 칸 비우고 다음 단계 설정
void setNext(const vector<int> v[], Player& p) {
	DWORD dw;
	COORD pos = { p.getStart().X + 20, p.getStart().Y + 3 };
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', MAXIMUM_LEVEL * 3, pos, &dw);
	pos.Y++;
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', MAXIMUM_LEVEL * 3, pos, &dw);
	p.clear();
	p.nextLevel();
}

// 승패여부를 확인하여 게임 상태 값을 가져옴
GAME_STATE getState(const Player& p1, const Player& p2) {
	if (p1.isWin()) return P1WIN;
	if (p2.isWin()) return P2WIN;
	if (p1.isOver()) return P1OVER;
	if (p2.isOver()) return P2OVER;
	return ONGOING;
}

void gameScreen() {
	system("cls");
	GAME_STATE state = ONGOING;
	Player p1, p2(1);

	vector<int> questionvec[MAXIMUM_LEVEL]; // 전체 문제세트를 미리 설정, 각 플레이어 레벨에 따라 순차 접근
	int input = 0;
	bool valid;

	printTimer(3);
	PlaySound(TEXT("8BitAdventure.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	drawMap();
	drawInfo(p1);
	drawInfo(p2);
	makeArrow(questionvec);
	printArrow(questionvec, p1);
	printArrow(questionvec, p2);
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); // Sleep 이전 입력 제거

	while (state == ONGOING) {
		input = 0;
		valid = true;
		if (_kbhit()) {
			input = _getch();
			if ((input == ARROW_NUM) && !p1.isOver()) {
				switch (_getch()) {
				case UP:
					p1.getAvec().push_back(UP);
					p1.getAstr() += "↑ ";
					break;
				case DOWN:
					p1.getAvec().push_back(DOWN);
					p1.getAstr() += "↓ ";
					break;
				case LEFT:
					p1.getAvec().push_back(LEFT);
					p1.getAstr() += "← ";
					break;
				case RIGHT:
					p1.getAvec().push_back(RIGHT);
					p1.getAstr() += "→ ";
					break;
				default:
					valid = false;
					break;
				}
				// ↑↓←→만을 유효값으로 판정하고 문제 화살표 개수 == 유효 입력수이면 다음 문제 출력
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
					p2.getAstr() += "↑ ";
					break;
				case 'S':
				case 's':
					p2.getAvec().push_back(DOWN);
					p2.getAstr() += "↓ ";
					break;
				case 'A':
				case 'a':
					p2.getAvec().push_back(LEFT);
					p2.getAstr() += "← ";
					break;
				case 'D':
				case 'd':
					p2.getAvec().push_back(RIGHT);
					p2.getAstr() += "→ ";
					break;
				default:
					valid = false;
					break;
				}
				// wasd(WASD)만을 유효값으로 판정
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
			// 플레이어 진행 상황에 따른 게임상태 업데이트
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
	// 승패 결과 확인
	PlaySound(TEXT("win.wav"), NULL, SND_NODEFAULT | SND_FILENAME);
	PlaySound(NULL, NULL, 0);
	Sleep(3000);
}



int main() {
	setConsoleView();
	while (true)
		gameScreen();

}

