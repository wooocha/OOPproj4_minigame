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
    int dice[5]; // 주사위가 나타내는 숫자
    bool select[5]; // 주사위가 선택되었는지
    Dice_Pos player; // 플레이어 커서 위치 x값은 주사위선택, y값은 점수선택에 사용
    int max_turn;
    int turn; // 현재 턴 
    int reRollCount; // 주사위를 다시 던진 횟수 (최대 2번)
    int score[10]; // 점수판
    int totalScore;
    std::string status; // 현재 게임 상태를 보여줄 string
    bool mode; // 카드 선택중일때 false 점수 선택중일떄 true

public:
    Dice();
    void start();
    void init();
    void setDice();
    void moveCur();
    void draw();
    void drawStatus(); // 게임 중 상태창 출력
    void setTotalScore(); // 총점을 계산하여 저장하는 함수
    int getScore(int);// 각 점수판에 따라 점수를 계산하여 반환하는 함수
    void selectScore();// 점수판에서 enter를 입력했을때 점수를 선택하여 할당하는 함수
    void selectDice();// 주사위를 선택하는 함수
    int rollDice();// 1~6까지 랜덤한 수를 반환하는 함수
    void reRoll();// 선택한 주사위에 다시 랜덤값을 할당하는 함수
    void printResult(); // 결과창 출력
    void SetTextColor(int);
};

#endif