/*
Yacht Dice 의 룰을 따서 만든 게임입니다.

게임이 시작하면 플레이어는 좌우 방향키와 스페이스바를 이용해 다시 굴릴 주사위를 선택합니다.
선택한 주사위는 색깔이 바뀌고 다시 선택하면 선택이 풀립니다.

결정이 끝나면 엔터키로 선택한 주사위를 다시 굴립니다. 두번의 기회가 주어집니다.

두번의 기회가 끝나면 사용자가 원하는 점수를 엔터키를 눌러 선택합니다.

선택하면 글씨 색깔이 바뀌고 점수가 할당됩니다. 하얀색 글씨의 점수는 현재 주사위로 얻을 수 있는 점수를 표시합니다.

한번 선택한 점수는 다시 고를 수 없고, 모든 점수를 선택하면 게임이 종료됩니다.

전략적으로 점수를 채워나가 높은 점수를 받는 게임입니다.

// 점수 기준 //
ONE~SIX 의 점수는 각 주사위의 숫자 * 개수 입니다.

CHOICE 는 모든 주사위 숫자의 합입니다.

STRAIGHT 는 12345 또는 23456 처럼 연속된 숫자들만 있는 경우입니다.(순서는 관계없습니다.)

FOURCARD 는 같은 숫자가 4개 이상 있는 경우에 점수가 주어집니다.

FIVECARD 는 가장 나오기 힘든 조합으로 같은 숫자 5개가 있는 경우입니다.
//////////////

*/


#include <iostream>
#include <Windows.h>
#include <string>
#include <conio.h>
#include <random>
#include <time.h>
#include <algorithm>

using namespace std;

#define INIT_POS 1

void CursorView(bool b)
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
    ENTER = 13,
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

class Dice {
private:
    int dice[5]; // 주사위가 나타내는 숫자
    bool select[5]; // 주사위가 선택되었는지
    Pos player; // 플레이어 커서 위치 x값은 주사위선택, y값은 점수선택에 사용
    int max_turn;
    int turn; // 현재 턴 
    int reRollCount; // 주사위를 다시 던진 횟수 (최대 2번)
    int score[10]; // 점수판
    int totalScore;
    string status; // 현재 게임 상태를 보여줄 string
    bool mode; // 카드 선택중일때 false 점수 선택중일떄 true

public:
    Dice() {};
    void start() {
        CursorView(false);
        init();

        int key = 0;
        while (true) {
            mode = reRollCount > 1 ? true : false;

            key = GetKeyDown();    //키보드 입력을 받는 함수
            if (key == KEYBOARD::IS_ARROW)
            {
                //화살표 입력시
                moveCur();
            }
            if (key == KEYBOARD::SPACE)
            {
                //스페이스바 입력시
                if(mode == false)
                selectDice();
            }
            if (key == KEYBOARD::ENTER)
            {
                //엔터 입력시
                if (mode == false) // reRoll
                    reRoll();
                else {
                    selectScore(); // Select Score
                    setDice();
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

    void init() {
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

    void setDice() {
        for (int i = 0; i < 5; i++) {
            dice[i] = rollDice();
        }
    }
    

    void moveCur() {
        // 방향키를 입력받으면, 먼저 원래 있던 player cursor를 지운다.
        GotoXY(INIT_POS + (player.x * 7), INIT_POS + 3);
        cout << "   ";
        GotoXY(INIT_POS + (36), INIT_POS + (player.y * 2) + 3);
        cout << "   ";
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

    void draw() {
        SetTextColor(WHITE);
        int c = 0;
        for (int cnt = 0; cnt < 3; cnt++) {
            GotoXY(INIT_POS, INIT_POS + (cnt));
            for (int x = 0; x < 5; ++x)
            {
                c = dice[x];
                if (select[x] == true) // n번째 주사위가 select 면 색깔바꿈
                {
                    SetTextColor(MINT);
                }
                if (cnt == 0) cout << "┌──┐   ";
                if (cnt == 1) cout << "│ " << c << "│   ";
                if (cnt == 2) cout << "└──┘   ";
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
        cout << "TOTALSCORE : " << totalScore;



        GotoXY(40, 4);
        if (score[CHOICE] < 0) {
            SetTextColor(WHITE);
            cout << "CHOICE : " << getScore(CHOICE) <<"  ";
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

    void drawStatus() { // 게임 중 상태창 출력
        GotoXY(0, 17);
        cout << "┌──────────────────────────────────┐   " << endl;
        cout << "                                   " << endl;
        cout << "                                   " << endl;
        cout << "       " << status << endl;
        cout << "                                   " << endl;
        cout << "                                   " << endl;
        cout << "└──────────────────────────────────┘ ";
    }

    void setTotalScore() { // 총점을 계산하여 저장하는 함수
        totalScore = 0;
        for (int i = 0; i < 10; i++) {
            if (score[i] >= 0) {
                totalScore += score[i];
            }
        }
    }

    int getScore(int num) { // 각 점수판에 따라 점수를 계산하여 반환하는 함수
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
            int temp[5]; // 점수계산에 사용할 임시배열
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

    void selectScore() { // 점수판에서 enter를 입력했을때 점수를 선택하여 할당하는 함수
        if (score[player.y] < 0) {
            score[player.y] = getScore(player.y);
            reRollCount = 0;
            turn++;
        }
    }

    void selectDice() { // 주사위를 선택하는 함수
        if (select[player.x] == false) {
            select[player.x] = true;
        }
        else select[player.x] = false;
    }

    int rollDice() { // 1~6까지 랜덤한 수를 반환하는 함수
        random_device random;

        // random_device 를 통해 난수 생성 엔진을 초기화 한다.
        mt19937 gen(random());

        // 0 부터 15 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
        uniform_int_distribution<int> dis(1, 6);
        return dis(gen);
        
    }

    void reRoll() { // 선택한 주사위에 다시 랜덤값을 할당하는 함수
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

    void printResult() { // 결과창 출력
        status = "Your Score : " + to_string(totalScore) +"          ";
        drawStatus();
        Sleep(5000);
    }

    void SetTextColor(int color)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }
};














int main()
{
    Dice game;
    game.start();

    return 0;
}

