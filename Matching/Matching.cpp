#include <iostream>
#include <Windows.h>
#include <string>
#include <conio.h>
#include <random>
#include <time.h>

using namespace std;

#define INIT_POS 1

void CursorView(bool b) // ������ Ŀ���� ���̰ų� ����� �Լ�
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

struct Pos
{
    int x;
    int y;
};

//Ű���� �Է��� ������ ���� enum
enum KEYBOARD
{
    IS_ARROW = 224,
    UP = 72,
    LEFT = 75,
    RIGHT = 77,
    DOWN = 80,
    SPACE = 32,
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

class Matching {
private :
    char map[4][4]; // player�� ���� ��
    char answerMap[4][4]; // ���� ������ ��
    Pos player; // �÷��̾��� ��ġ
    Pos flipPos[2]; // �÷��̾ ������ ī����ġ
    int flipCount; // �÷��̾ ī�带 ������ Ƚ�� (<2)
    int matchedCardCount; //�÷��̰� ���� ī�� ���� ����
    double timeCount; // �ҿ�ð�
    clock_t startTime; // ���� ���� �ð�
public :
    Matching() {};

    void start() {
        CursorView(false);
        init();
        
        int key = 0;
        while (true) {
            key = GetKeyDown();    //Ű���� �Է� ����

            if (key == KEYBOARD::IS_ARROW)
            {
                //ȭ��ǥ �Է½�
                moveCur();
            }
            if (key == KEYBOARD::SPACE)
            {
                //�����̽��� �Է½�
                flipCard();
            }

            draw(); // ȭ�� �׸���
            checkCard(); // �÷��̾ ������ ī����� ���� ¦���� Ȯ��
            setTimeCount(); //�ҿ�ð� ���,���

            if (matchedCardCount == 8) { // ¦�� ��� ���߾����� ��� ��� �� �����
                draw();
                printResult();
                init();
            }
        }
        CursorView(true);
    }

    void init() { // �ʱ�ȭ
        player.x = 0;
        player.y = 0;
        flipCount = 0;
        matchedCardCount = 0;
        setCard();
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                map[i][j] = '?';
            }
       }
        startTime = clock();
    }

    void flipCard() { // �����̽��� �Է½� ī�带 ������(?���� ������ �ٲٴ�) ����
        
        if (map[player.y][player.x] == '?') { // player�� ����Ű�� ī�尡 ? �϶��� ī�带 �����´�.
            flipPos[flipCount].x = player.x;
            flipPos[flipCount].y = player.y;
            map[player.y][player.x] = answerMap[player.y][player.x];
            flipCount++;
        }
        
    }

    void moveCur() {
        // ����Ű�� �Է¹�����, ���� ���� �ִ� player cursor�� �����.
        GotoXY(INIT_POS + (player.x * 7), INIT_POS + (player.y * 4) + 3); 
        printf("   ");
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
            if (player.x >= 3)
            {
                player.x = 3;
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
            if (player.y >= 3)
            {
                player.y = 3;
            }
            break;
        }
    }

    void draw() {
        SetTextColor(WHITE);
        char c = '\0';
        for (int y = 0; y < 4; y++)
        {
            for (int cnt = 0; cnt < 3; cnt++) {
                GotoXY(INIT_POS, INIT_POS + (4 * y + cnt));
                for (int x = 0; x < 4; ++x)
                {
                    c = map[y][x];
                    if (c != '?')
                    {
                        SetTextColor(MINT);
                    }
                    if (cnt == 0) cout << "��������   ";
                    if (cnt == 1) cout << "�� " << c << "��   ";
                    if (cnt == 2) cout << "��������   ";
                    SetTextColor(WHITE);
                }
            }
        }
        //player draw
        GotoXY(INIT_POS + (player.x * 7), INIT_POS + (player.y * 4) + 3);
        cout << "  ^";
        GotoXY(30, 1);
        cout << "COUNT : " << matchedCardCount << "/8";
    }

    void setCard() { // ������ �����Ҷ� ī����ġ�� �����ϰ� �������ִ� �Լ�
        
        answerMap[0][0] = 'A';
        answerMap[0][1] = 'A';
        answerMap[0][2] = 'B';
        answerMap[0][3] = 'B';
        answerMap[1][0] = 'C';
        answerMap[1][1] = 'C';
        answerMap[1][2] = 'D';
        answerMap[1][3] = 'D';
        answerMap[2][0] = 'E';
        answerMap[2][1] = 'E';
        answerMap[2][2] = 'F';
        answerMap[2][3] = 'F';
        answerMap[3][0] = 'G';
        answerMap[3][1] = 'G';
        answerMap[3][2] = 'H';
        answerMap[3][3] = 'H';
        random_device random;

        // random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
        mt19937 gen(random());

        // 0 ���� 15 ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.
        uniform_int_distribution<int> dis(0, 15);

        for (int i = 0; i < 16; i++) { // shuffle
            char temp;
            int rand = dis(gen);
            temp = answerMap[i / 4][i % 4];
            answerMap[i / 4][i % 4] = answerMap[rand / 4][rand % 4];
            answerMap[rand / 4][rand % 4] = temp;
        }
        
    }


    void drawCard() { // ī�� ��ġ�� ���¿� ���� ī�带 �׷��ִ� �Լ�

    }

     void checkCard() { // player�� ������ ī����� ���� ������� Ȯ���ϴ� �Լ�
        if (flipCount == 2) {
            if (answerMap[flipPos[0].y][flipPos[0].x] == answerMap[flipPos[1].y][flipPos[1].x]) {
                matchedCardCount++;
            }
            else {
                map[flipPos[0].y][flipPos[0].x] = '?';
                map[flipPos[1].y][flipPos[1].x] = '?';
            }
            Sleep(300);
            flipCount = 0;
        }
    }

    void printResult() { // ������ ������ ���â ���
        GotoXY(0, 17);
        cout << "������������������������������������������������������������������������������������������������   " <<endl;
        cout << "                                               " <<endl;
        cout << "                                               " << endl;
        cout << "              " << "RECORD : " << timeCount / 1000 <<" Sec" << endl;
        cout << "                                                " << endl;
        cout << "                                               " << endl;
        cout << "������������������������������������������������������������������������������������������������ ";
    }

    void setTimeCount() {
        GotoXY(30, 3);
        clock_t end = clock();
        timeCount = (double)(end - startTime);
        cout << "TIME : " << timeCount / 1000 << "sec";
    }

    void SetTextColor(int color)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }
};


int main()
{
    Matching game;
    game.start();
    return 0;
}
