#include "BSBoard.h"
#include <new>

USING_NS_CC;
using namespace std;

BATTLE_SCENE_NS_BEG

BSBoard* BSBoard::create(BattleScene *scn, PlayerIdType id) {
	auto board = new (nothrow) BSBoard(scn, id);

	if (board && board->init()) 
		return board;
	
	delete board;
	board = nullptr;
	return board;
}

BSBoard::BSBoard(BattleScene *scn, PlayerIdType id) : btlScn(scn), ownerId(id){

}

BSBoard::~BSBoard() {
	CCLOG("BSBoard::dtor %d", ownerId);
}

bool BSBoard::init() {


	return true;
}

BATTLE_SCENE_NS_END