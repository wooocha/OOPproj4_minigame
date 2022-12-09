#include <iostream>
#include <Windows.h>
#include <string>
#include <conio.h>
#include <random>
#include <time.h>

using namespace std;

#define INIT_POS 1

void CursorView(bool b) // 게임중 커서를 보이거나 숨기는 함수
{
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1; //커서 굵기 (1 ~ 100)
    cursorInfo.bVisible = b; //커서 Visible TRUE(보임) FALSE(숨김)
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

//키보드 입력을 저장해 놓은 enum
enum KEYBOARD
{
    IS_ARROW = 224,
    UP = 72,
    LEFT = 75,
    RIGHT = 77,
    DOWN = 80,
    SPACE = 32,
};

//색상을 저장해놓은 enum
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
    char map[4][4]; // player가 보는 맵
    char answerMap[4][4]; // 답을 저장한 맵
    Pos player; // 플레이어의 위치
    Pos flipPos[2]; // 플레이어가 뒤집은 카드위치
    int flipCount; // 플레이어가 카드를 뒤집은 횟수 (<2)
    int matchedCardCount; //플레이가 맞춘 카드 쌍의 개수
    double timeCount; // 소요시간
    clock_t startTime; // 게임 시작 시간
public :
    Matching() {};

    void start() {
        CursorView(false);
        init();
        
        int key = 0;
        while (true) {
            key = GetKeyDown();    //키보드 입력 받음

            if (key == KEYBOARD::IS_ARROW)
            {
                //화살표 입력시
                moveCur();
            }
            if (key == KEYBOARD::SPACE)
            {
                //스페이스바 입력시
                flipCard();
            }

            draw(); // 화면 그리기
            checkCard(); // 플레이어가 선택한 카드들이 같은 짝인지 확인
            setTimeCount(); //소요시간 계산,출력

            if (matchedCardCount == 8) { // 짝을 모두 맞추었을시 결과 출력 후 재시작
                draw();
                printResult();
                init();
            }
        }
        CursorView(true);
    }

    void init() { // 초기화
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

    void flipCard() { // 스페이스바 입력시 카드를 뒤집는(?에서 값으로 바꾸는) 연산
        
        if (map[player.y][player.x] == '?') { // player가 가리키는 카드가 ? 일때만 카드를 뒤집는다.
            flipPos[flipCount].x = player.x;
            flipPos[flipCount].y = player.y;
            map[player.y][player.x] = answerMap[player.y][player.x];
            flipCount++;
        }
        
    }

    void moveCur() {
        // 방향키를 입력받으면, 먼저 원래 있던 player cursor를 지운다.
        GotoXY(INIT_POS + (player.x * 7), INIT_POS + (player.y * 4) + 3); 
        printf("   ");
        //방향키 입력을 받아서, player를 그에 맞게 이동시킨다.
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
                    if (cnt == 0) cout << "┌──┐   ";
                    if (cnt == 1) cout << "│ " << c << "│   ";
                    if (cnt == 2) cout << "└──┘   ";
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

    void setCard() { // 게임이 시작할때 카드위치를 랜덤하게 설정해주는 함수
        
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

        // random_device 를 통해 난수 생성 엔진을 초기화 한다.
        mt19937 gen(random());

        // 0 부터 15 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
        uniform_int_distribution<int> dis(0, 15);

        for (int i = 0; i < 16; i++) { // shuffle
            char temp;
            int rand = dis(gen);
            temp = answerMap[i / 4][i % 4];
            answerMap[i / 4][i % 4] = answerMap[rand / 4][rand % 4];
            answerMap[rand / 4][rand % 4] = temp;
        }
        
    }


    void drawCard() { // 카드 위치와 상태에 따라 카드를 그려주는 함수

    }

     void checkCard() { // player가 선택한 카드들이 같은 모양인지 확인하는 함수
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

    void printResult() { // 게임이 끝나면 결과창 출력
        GotoXY(0, 17);
        cout << "┌──────────────────────────────────────────────┐   " <<endl;
        cout << "                                               " <<endl;
        cout << "                                               " << endl;
        cout << "              " << "RECORD : " << timeCount / 1000 <<" Sec" << endl;
        cout << "                                                " << endl;
        cout << "                                               " << endl;
        cout << "└──────────────────────────────────────────────┘ ";
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
