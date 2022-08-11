#ifndef __CARD_HOLDER_H__
#define __CARD_HOLDER_H__

#include "BSDefine.h"
#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"
#include "ui/UIImageView.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>

namespace BSPrefabNS {
	class EnergyItem;
};

BATTLE_SCENE_NS_BEG

class BattleScene;
class BSCard;
class PetCard;
class EnergyCard;

class CardHolder : public cocos2d::ui::Layout {
public:
	using EnergyId = std::string;

	static cocos2d::Size HOLDER_SIZE;
	static CardHolder* create(BattleScene *btlScn, PlayerIdType id);
	CardHolder(BattleScene *btlScn, PlayerIdType id);
	virtual ~CardHolder();
	virtual bool init() override;
	void setHolderSizeH(float h);
	void setHolderSizeW(float w);
	
	bool tryAddPetCard(PetCard *card);
	bool tryAddEnergyCard(EnergyCard *card);

	void updateInfoPanel(bool show);
protected:
	virtual void clear();

	BattleScene *btlScn = nullptr;
	PlayerIdType ownerId = PlayerIdInvalid;

	int dmgCounter = 0;

	PetCard *petCard = nullptr;
	std::vector<PetCard*> preEvCardVec;
	std::vector<EnergyCard*> energyCardVec;

	cocos2d::Node *cardMarker = nullptr;
	cocos2d::Node *energyCardMarker = nullptr;

	cocos2d::ui::Text *flyingText = nullptr;
	void showFlyingText(const std::string &s);

	cocos2d::ui::Layout *hpPanel = nullptr;
	cocos2d::ui::ImageView *petTypeImg = nullptr;
	cocos2d::ui::Text *hpText = nullptr;

	cocos2d::ui::Layout *energyPanel = nullptr;
	std::map<EnergyId, BSPrefabNS::EnergyItem*> energyItemMap;

	cocos2d::Node *node = nullptr;

};

BATTLE_SCENE_NS_END


#endif // __CARD_HOLDER_H__
