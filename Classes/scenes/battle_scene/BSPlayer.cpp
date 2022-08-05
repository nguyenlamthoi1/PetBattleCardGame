#include "BSPlayer.h"

using namespace std;
USING_NS_CC;

BATTLE_SCENE_NS_BEG

BSPlayer::BSPlayer(PlayerIdType i) : id(i) {

}

BSPlayer::~BSPlayer() {
	CCLOG("BSPlayer::dtor called");
}

BATTLE_SCENE_NS_END