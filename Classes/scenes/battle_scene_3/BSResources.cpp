#include "BSResources.h"
#include "BSCard.h"
#include "GameManager.h"

using namespace std;
USING_NS_CC;

BATTLE_SCENE_NS_BEG

BSResources::BSResources(){}

BSResources::~BSResources(){
	clean();
}

void BSResources::clean() {
	CCLOG("Erase &d BSCard", cardVec.size());
	for (const auto card : cardVec) 
		card->release();
	cardVec.clear();
}

BSCard* BSResources::getBSCard(const CardId &cid) {
	// Create new BSCard
	auto dtmgr = GM_DATA_MGR;
	auto cardData = dtmgr->getCardData(cid);
	auto bsCard = BSCard::createWithData(cardData);
	bsCard->retain();
	cardVec.push_back(bsCard);
	return bsCard;
}


BATTLE_SCENE_NS_END