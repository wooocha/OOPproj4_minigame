#ifndef DICE_H
#define DICE_H
#include <string>


struct Dice_Pos
{
    int x;
    int y;
};

class Dice {
private:
    int dice[5]; // �ֻ����� ��Ÿ���� ����
    bool select[5]; // �ֻ����� ���õǾ�����
    Dice_Pos player; // �÷��̾� Ŀ�� ��ġ x���� �ֻ�������, y���� �������ÿ� ���
    int max_turn;
    int turn; // ���� �� 
    int reRollCount; // �ֻ����� �ٽ� ���� Ƚ�� (�ִ� 2��)
    int score[10]; // ������
    int totalScore;
    std::string status; // ���� ���� ���¸� ������ string
    bool mode; // ī�� �������϶� false ���� �������ϋ� true

public:
    Dice();
    void start();
    void init();
    void setDice();
    void moveCur();
    void draw();
    void drawStatus(); // ���� �� ����â ���
    void setTotalScore(); // ������ ����Ͽ� �����ϴ� �Լ�
    int getScore(int);// �� �����ǿ� ���� ������ ����Ͽ� ��ȯ�ϴ� �Լ�
    void selectScore();// �����ǿ��� enter�� �Է������� ������ �����Ͽ� �Ҵ��ϴ� �Լ�
    void selectDice();// �ֻ����� �����ϴ� �Լ�
    int rollDice();// 1~6���� ������ ���� ��ȯ�ϴ� �Լ�
    void reRoll();// ������ �ֻ����� �ٽ� �������� �Ҵ��ϴ� �Լ�
    void printResult(); // ���â ���
    void SetTextColor(int);
};

#endif