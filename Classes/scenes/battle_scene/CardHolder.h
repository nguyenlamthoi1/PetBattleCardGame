#ifndef __CARD_HOLDER_H__
#define __CARD_HOLDER_H__

#include "BSDefine.h"
#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>

BATTLE_SCENE_NS_BEG

class BSCard;
class PetCard;
class EnergyCard;

class CardHolder : public cocos2d::ui::Layout {
public:
	static cocos2d::Size HOLDER_SIZE;
	static CardHolder* create();
	CardHolder();
	virtual ~CardHolder();
	virtual bool init() override;
	void setHolderSizeH(float h);
	void setHolderSizeW(float w);
	bool tryAddPetCard(PetCard *card);
	void updateInfoPanel(bool show);
protected:
	PetCard *petCard = nullptr;
	std::vector<PetCard*> preEvCardVec;
	std::vector<EnergyCard*> energyCardVec;

	cocos2d::Node *cardMarker = nullptr;

	cocos2d::ui::Text *flyingText = nullptr;
	void showFlyingText(const std::string &s);

	cocos2d::Node *node = nullptr;
};

BATTLE_SCENE_NS_END


#endif // __CARD_HOLDER_H__
