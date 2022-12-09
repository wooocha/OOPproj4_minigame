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
    char map[4][4]; // player가 보는 맵
    char answerMap[4][4]; // 답을 저장한 맵
    Pos player; // 플레이어의 위치
    Pos flipPos[2]; // 플레이어가 뒤집은 카드위치
    int flipCount; // 플레이어가 카드를 뒤집은 횟수 (<2)
    int matchedCardCount; //플레이가 맞춘 카드 쌍의 개수
    double timeCount; // 소요시간
    clock_t startTime; // 게임 시작 시간

public:

    Matching();
    void start();
    void init(); // 초기화
    void flipCard(); // 스페이스바 입력시 카드를 뒤집는('?'에서 값으로 바꾸는) 연산
    void moveCur();
    void draw();
    void setCard(); // 게임이 시작할때 카드위치를 랜덤하게 설정해주는 함수
    void checkCard(); // player가 선택한 카드들이 같은 모양인지 확인하는 함수
    void printResult();// 게임이 끝나면 결과창 출력
    void setTimeCount();
    void SetTextColor(int);

};

#endif
