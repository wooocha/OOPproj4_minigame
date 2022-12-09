/*
Yacht Dice �� ���� ���� ���� �����Դϴ�.

������ �����ϸ� �÷��̾�� �¿� ����Ű�� �����̽��ٸ� �̿��� �ٽ� ���� �ֻ����� �����մϴ�.
������ �ֻ����� ������ �ٲ�� �ٽ� �����ϸ� ������ Ǯ���ϴ�.

������ ������ ����Ű�� ������ �ֻ����� �ٽ� �����ϴ�. �ι��� ��ȸ�� �־����ϴ�.

�ι��� ��ȸ�� ������ ����ڰ� ���ϴ� ������ ����Ű�� ���� �����մϴ�.

�����ϸ� �۾� ������ �ٲ�� ������ �Ҵ�˴ϴ�. �Ͼ�� �۾��� ������ ���� �ֻ����� ���� �� �ִ� ������ ǥ���մϴ�.

�ѹ� ������ ������ �ٽ� �� �� ����, ��� ������ �����ϸ� ������ ����˴ϴ�.

���������� ������ ä������ ���� ������ �޴� �����Դϴ�.

// ���� ���� //
ONE~SIX �� ������ �� �ֻ����� ���� * ���� �Դϴ�.

CHOICE �� ��� �ֻ��� ������ ���Դϴ�.

STRAIGHT �� 12345 �Ǵ� 23456 ó�� ���ӵ� ���ڵ鸸 �ִ� ����Դϴ�.(������ ��������ϴ�.)

FOURCARD �� ���� ���ڰ� 4�� �̻� �ִ� ��쿡 ������ �־����ϴ�.

FIVECARD �� ���� ������ ���� �������� ���� ���� 5���� �ִ� ����Դϴ�.
//////////////
*/
// totalScore ��¼���, setDice() ��ġ���� start -> selectScore

#include "Dice.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <conio.h>
#include <random>
#include <time.h>
#include <algorithm>

using namespace std;

#define INIT_POS 1

namespace D {
    void CursorView(bool b)
    {
        CONSOLE_CURSOR_INFO cursorInfo = { 0, };
        cursorInfo.dwSize = 1; //Ŀ�� ���� (1 ~ 100)
        cursorInfo.bVisible = b; //Ŀ�� Visible TRUE(����) FALSE(����)
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    }

    void GotoXY(int x, int y)
    {
        COORD pos;
        pos.X = x;
        pos.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    }

    int GetKeyDown()
    {
        if (_kbhit() != 0)
        {
            return _getch();
        }
        return 0;
    }

    //Ű���� �Է��� ������ ���� enum
    enum KEYBOARD
    {
        IS_ARROW = 224,
        UP = 72,
        LEFT = 75,
        RIGHT = 77,
        DOWN = 80,
        SPACE = 32,
        ENTER = 13,
    };

    //������ �����س��� enum
    enum COLOR
    {
        GREEN = 10,
        MINT,
        RED,
        PINK,
        YELLOW,
        WHITE,
    };

    enum SCORE
    {
        CHOICE = 0,
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        STRAIGHT = 7,
        FOURCARD,
        FIVECARD,
    };

}
using namespace D;

Dice::Dice() {};
void Dice::start() {

    CursorView(false);
    init();

    int key = 0;
    while (true) {
        mode = reRollCount > 1 ? true : false;

        key = GetKeyDown();    //Ű���� �Է��� �޴� �Լ�
        if (key == KEYBOARD::IS_ARROW)
        {
            //ȭ��ǥ �Է½�
            moveCur();
        }
        if (key == KEYBOARD::SPACE)
        {
            //�����̽��� �Է½�
            if (mode == false)
                selectDice();
        }
        if (key == KEYBOARD::ENTER)
        {
            //���� �Է½�
            if (mode == false) // reRoll
                reRoll();
            else {
                selectScore(); // Select Score
            }
        }
        if (!mode) status = "Pick the dice to reroll";
        else status = "Pick the score you want";
        draw();

        if (turn > 10) {
            draw();
            printResult();
            init();
        }
    }
    CursorView(true);
}

void Dice::init() {
    player.x = 0;
    player.y = 0;
    turn = 1;
    reRollCount = 0;
    totalScore = 0;
    mode = false;
    status = "Pick the dice to reroll";
    for (int i = 0; i < 10; i++) {
        score[i] = -1;
    }
    for (int i = 0; i < 5; i++) {
        select[i] = false;
        dice[i] = 0;
    }
    setDice();
}

void Dice::setDice() {
    for (int i = 0; i < 5; i++) {
        dice[i] = rollDice();
    }
}


void Dice::moveCur() {
    // ����Ű�� �Է¹�����, ���� ���� �ִ� player cursor�� �����.
    GotoXY(INIT_POS + (player.x * 7), INIT_POS + 3);
    cout << "   ";
    GotoXY(INIT_POS + (36), INIT_POS + (player.y * 2) + 3);
    cout << "   ";
    //����Ű �Է��� �޾Ƽ�, player�� �׿� �°� �̵���Ų��.
    switch (_getch())
    {
    case KEYBOARD::LEFT:
        player.x--;
        if (player.x <= 0)
        {
            player.x = 0;
        }
        break;
    case KEYBOARD::RIGHT:
        player.x++;
        if (player.x >= 4)
        {
            player.x = 4;
        }
        break;
    case KEYBOARD::UP:
        player.y--;
        if (player.y <= 0)
        {
            player.y = 0;
        }
        break;
    case KEYBOARD::DOWN:
        player.y++;
        if (player.y >= 9)
        {
            player.y = 9;
        }
        break;
    }
}

void Dice::draw() {
    SetTextColor(WHITE);
    int c = 0;
    for (int cnt = 0; cnt < 3; cnt++) {
        GotoXY(INIT_POS, INIT_POS + (cnt));
        for (int x = 0; x < 5; ++x)
        {
            c = dice[x];
            if (select[x] == true) // n��° �ֻ����� select �� ����ٲ�
            {
                SetTextColor(MINT);
            }
            if (cnt == 0) cout << "��������   ";
            if (cnt == 1) cout << "�� " << c << "��   ";
            if (cnt == 2) cout << "��������   ";
            SetTextColor(WHITE);
        }
    }

    //draw player cursor
    if (mode == false) {
        GotoXY(INIT_POS + (player.x * 7), INIT_POS + 3);
        cout << "  ^";
    }
    else {
        GotoXY(INIT_POS + (36), INIT_POS + (player.y * 2) + 3);
        cout << "  >";
    }


    //draw scoreboard
    GotoXY(40, 1);
    cout << "REROLL : " << reRollCount << "/" << 2;
    setTotalScore();
    SetTextColor(RED);
    GotoXY(40, 2);
    cout << "TOTALSCORE : " << totalScore << "  ";



    GotoXY(40, 4);
    if (score[CHOICE] < 0) {
        SetTextColor(WHITE);
        cout << "CHOICE : " << getScore(CHOICE) << "  ";
    }
    else {
        SetTextColor(MINT);
        cout << "CHOICE : " << score[CHOICE] << "  ";
    }
    GotoXY(40, 6);
    if (score[ONE] < 0) {
        SetTextColor(WHITE);
        cout << "ONE : " << getScore(ONE) << "  ";
    }
    else {
        SetTextColor(MINT);
        cout << "ONE : " << score[ONE] << "  ";
    }
    GotoXY(40, 8);
    if (score[TWO] < 0) {
        SetTextColor(WHITE);
        cout << "TWO : " << getScore(TWO) << "  ";
    }
    else {
        SetTextColor(MINT);
        cout << "TWO : " << score[TWO] << "  ";
    }
    GotoXY(40, 10);
    if (score[THREE] < 0) {
        SetTextColor(WHITE);
        cout << "THREE : " << getScore(THREE) << "  ";
    }
    else {
        SetTextColor(MINT);
        cout << "THREE : " << score[THREE] << "  ";
    }
    GotoXY(40, 12);
    if (score[FOUR] < 0) {
        SetTextColor(WHITE);
        cout << "FOUR : " << getScore(FOUR) << "  ";
    }
    else {
        SetTextColor(MINT);
        cout << "FOUR : " << score[FOUR] << "  ";
    }
    GotoXY(40, 14);
    if (score[FIVE] < 0) {
        SetTextColor(WHITE);
        cout << "FIVE : " << getScore(FIVE) << "  ";
    }
    else {
        SetTextColor(MINT);
        cout << "FIVE : " << score[FIVE] << "  ";
    }
    GotoXY(40, 16);
    if (score[SIX] < 0) {
        SetTextColor(WHITE);
        cout << "SIX : " << getScore(SIX) << "  ";
    }
    else {
        SetTextColor(MINT);
        cout << "SIX : " << score[SIX] << "  ";
    }
    GotoXY(40, 18);
    if (score[STRAIGHT] < 0) {
        SetTextColor(WHITE);
        cout << "STRAIGHT : " << getScore(STRAIGHT) << "  ";
    }
    else {
        SetTextColor(MINT);
        cout << "STRAIGHT : " << score[STRAIGHT] << "  ";
    }
    GotoXY(40, 20);
    if (score[FOURCARD] < 0) {
        SetTextColor(WHITE);
        cout << "FOURCARD : " << getScore(FOURCARD) << "  ";
    }
    else {
        SetTextColor(MINT);
        cout << "FOURCARD : " << score[FOURCARD] << "  ";
    }
    GotoXY(40, 22);
    if (score[FIVECARD] < 0) {
        SetTextColor(WHITE);
        cout << "FIVECARD : " << getScore(FIVECARD) << "  ";
    }
    else {
        SetTextColor(MINT);
        cout << "FIVECARD : " << score[FIVECARD] << "  ";
    }
    SetTextColor(WHITE);
    drawStatus();
}

void Dice::drawStatus() { // ���� �� ����â ���
    GotoXY(0, 17);
    cout << "������������������������������������������������������������������������   " << endl;
    cout << "                                   " << endl;
    cout << "                                   " << endl;
    cout << "       " << status << endl;
    cout << "                                   " << endl;
    cout << "                                   " << endl;
    cout << "������������������������������������������������������������������������ ";
}

void Dice::setTotalScore() { // ������ ����Ͽ� �����ϴ� �Լ�
    totalScore = 0;
    for (int i = 0; i < 10; i++) {
        if (score[i] >= 0) {
            totalScore += score[i];
        }
    }
}

int Dice::getScore(int num) { // �� �����ǿ� ���� ������ ����Ͽ� ��ȯ�ϴ� �Լ�
    int result = 0;
    switch (num)
    {
    case CHOICE:
        for (int i = 0; i < 5; i++) {
            result += dice[i];
        }
        break;
    case ONE:
        for (int i = 0; i < 5; i++) {
            if (dice[i] == 1) {
                result += 1;
            }
        }
        break;
    case TWO:
        for (int i = 0; i < 5; i++) {
            if (dice[i] == 2) {
                result += 2;
            }
        }
        break;
    case THREE:
        for (int i = 0; i < 5; i++) {
            if (dice[i] == 3) {
                result += 3;
            }
        }
        break;
    case FOUR:
        for (int i = 0; i < 5; i++) {
            if (dice[i] == 4) {
                result += 4;
            }
        }
        break;
    case FIVE:
        for (int i = 0; i < 5; i++) {
            if (dice[i] == 5) {
                result += 5;
            }
        }
        break;
    case SIX:
        for (int i = 0; i < 5; i++) {
            if (dice[i] == 6) {
                result += 6;
            }
        }
        break;
    case STRAIGHT: {
        int temp[5]; // ������꿡 ����� �ӽù迭
        int cnt = 0;
        copy(dice, dice + 5, temp);
        sort(temp, temp + 5);
        for (int i = 0; i < 4; i++) {
            if (temp[i + 1] - temp[i] == 1)
                cnt++;
            else break;
        }
        if (cnt == 4) result = 25;
        break;
    }
    case FOURCARD:
    {
        int temp[5];
        int cnt = 0;
        int root = 0;
        copy(dice, dice + 5, temp);
        sort(temp, temp + 5);
        if (temp[0] == temp[1]) root = temp[0];
        else if (temp[4] == temp[3]) root = temp[4];
        else break;

        for (int i = 0; i < 5; i++) {
            if (temp[i] == root)
                cnt++;
        }
        if (cnt >= 4) result = 15 + 4 * root;
        break;
    }

    case FIVECARD:
    {
        int cnt = 0;
        for (int i = 0; i < 4; i++) {
            if (dice[i + 1] == dice[i])
                cnt++;
            else break;
        }
        if (cnt == 4) result = 40;
        break;
    }
    }
    return result;
}

void Dice::selectScore() { // �����ǿ��� enter�� �Է������� ������ �����Ͽ� �Ҵ��ϴ� �Լ�
    if (score[player.y] < 0) {
        score[player.y] = getScore(player.y);
        reRollCount = 0;
        turn++;
        setDice();
    }
}

void Dice::selectDice() { // �ֻ����� �����ϴ� �Լ�
    if (select[player.x] == false) {
        select[player.x] = true;
    }
    else select[player.x] = false;
}

int Dice::rollDice() { // 1~6���� ������ ���� ��ȯ�ϴ� �Լ�
    random_device random;

    // random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
    mt19937 gen(random());

    // 0 ���� 15 ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.
    uniform_int_distribution<int> dis(1, 6);
    return dis(gen);

}

void Dice::reRoll() { // ������ �ֻ����� �ٽ� �������� �Ҵ��ϴ� �Լ�
    if (reRollCount < 5) {
        for (int i = 0; i < 5; i++) {
            if (select[i] == true) {
                dice[i] = rollDice();
            }
            select[i] = false;
        }
        reRollCount++;
    }
}

void Dice::printResult() { // ���â ���
    status = "Your Score : " + to_string(totalScore) + "          ";
    drawStatus();
    Sleep(5000);
}

void Dice::SetTextColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
