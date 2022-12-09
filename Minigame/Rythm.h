#ifndef RYTHM_H_
#define RYTHM_H_

#include <string>
#include <vector>
#include <windows.h>


enum GAME_STATE {
	ONGOING = 0,
	P1WIN,
	P2WIN,
	P1OVER,
	P2OVER
};

class Player {
private:
	unsigned int life = 3;
	unsigned int level = 1;
	unsigned int numOfInput = 0;
	std::vector<int> answervec;
	std::string questionstr = "";
	std::string answerstr = "";
	COORD start = { 0, 1 }; // �� �κ� �����ϰ� ���밡���� ù ��ǥ

public:
	Player() {}
	Player(short n) : start{ 0, 1 + 10 * n } {} // �÷��̾� 1,2 ȭ�� ����
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
	std::vector<int>& getAvec() {
		return answervec;
	}
	std::string& getQstr() {
		return questionstr;
	}
	std::string& getAstr() {
		return answerstr;
	}
	COORD getStart() const {
		return start;
	}

	void isPressed();
	void minusLife();
	void nextLevel();
	void clear();
	bool isOver() const;
	bool isWin() const;

};

class Rythm {
public:
	void printTimer(int second);
	void gameOver(const Player&);
	void gameWin(const Player&);
	void makeArrow(std::vector<int>[]);
	void printArrow(const std::vector<int>*, Player&);
	void checkAnswer(std::vector<int>[], Player&);
	void drawInfo(const Player&);
	void drawMap();
	bool isNext(const std::vector<int>[], const Player&);
	void setNext(const std::vector<int>[], Player&);
	GAME_STATE getState(const Player&, const Player&);
	void gameScreen();
};

#endif