#include "Rythm.h"
#include "MatchingGame.h"
#include "Dice.h"
#include <iostream>
using namespace std;

int main()
{
	int input = 0;
	cin >> input;
	switch (input) {
	case 1 :
	{
		Rythm* rythm = new Rythm();
		rythm->gameScreen();
		break;
	}
	case 2 :
	{
		Matching* match = new Matching();
		match->start();
		break;
	}
	case 3 :
	{
		Dice* dice = new Dice();
		dice->start();
		break;
	}
	}
}