#include "BSPlayer.h"

using namespace std;
USING_NS_CC;

BATTLE_SCENE_NS_BEG

/*
	BSControlledOpponent Class
*/

BSControlledOpponent::BSControlledOpponent(BattleScene *scn, PlayerIdType pid) : BSOpponent(scn, pid) {}

BSControlledOpponent::~BSControlledOpponent(){}

void BSControlledOpponent::startSetup() {
	BSPlayer::startSetup();
}

void BSControlledOpponent::startTurn() {
	BSPlayer::startTurn();
}

BATTLE_SCENE_NS_END