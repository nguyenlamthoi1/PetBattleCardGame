#include "BSPlayer.h"

using namespace std;
USING_NS_CC;

BATTLE_SCENE_NS_BEG

/*
	BSGamer Class
*/

BSGamer::BSGamer(BattleScene *scn, PlayerIdType pid) : BSPlayer(scn, pid) {}

BSGamer::~BSGamer(){}

void BSGamer::startSetup() {
	BSPlayer::startSetup();
}

void BSGamer::startTurn() {
	BSPlayer::startTurn();
}


BATTLE_SCENE_NS_END