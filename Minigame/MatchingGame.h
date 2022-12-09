#ifndef MATCHING_H_
#define MATCHING_H_

#include <time.h>

struct Pos
{
    int x;
    int y;
};

class Matching {
private:
    char map[4][4]; // player�� ���� ��
    char answerMap[4][4]; // ���� ������ ��
    Pos player; // �÷��̾��� ��ġ
    Pos flipPos[2]; // �÷��̾ ������ ī����ġ
    int flipCount; // �÷��̾ ī�带 ������ Ƚ�� (<2)
    int matchedCardCount; //�÷��̰� ���� ī�� ���� ����
    double timeCount; // �ҿ�ð�
    clock_t startTime; // ���� ���� �ð�

public:

    Matching();
    void start();
    void init(); // �ʱ�ȭ
    void flipCard(); // �����̽��� �Է½� ī�带 ������('?'���� ������ �ٲٴ�) ����
    void moveCur();
    void draw();
    void setCard(); // ������ �����Ҷ� ī����ġ�� �����ϰ� �������ִ� �Լ�
    void checkCard(); // player�� ������ ī����� ���� ������� Ȯ���ϴ� �Լ�
    void printResult();// ������ ������ ���â ���
    void setTimeCount();
    void SetTextColor(int);

};

#endif
