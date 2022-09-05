#include "BSPlayer.h"

using namespace std;
USING_NS_CC;

BATTLE_SCENE_NS_BEG

/*
	BSOpponent Class
*/

BSOpponent::BSOpponent(BattleScene *scn, PlayerIdType pid) : BSPlayer(scn, pid) {}

BSOpponent::~BSOpponent(){}

/////////////////////////////////////////////////


BATTLE_SCENE_NS_END