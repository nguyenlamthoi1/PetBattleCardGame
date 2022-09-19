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
	enum class HolderType {
		Active,
		Bench
	};

	using EnergyId = std::string;

	static cocos2d::Size HOLDER_SIZE;

	static CardHolder* createActive(BattleScene *btlScn, PlayerIdType id);
	static CardHolder* createBench(BattleScene *btlScn, PlayerIdType id);

	CardHolder(HolderType hType, BattleScene *btlScn, PlayerIdType id);
	virtual ~CardHolder();

	virtual bool init() override;
	void setHolderSizeH(float h);
	void setHolderSizeW(float w);
	
	bool tryAddBasicPetCard(PetCard *card, const std::function<void()> &onDone = nullptr);
	bool tryAddEnergyCard(EnergyCard *card, const std::function<void()> &onDone = nullptr);

	void setFlipPetCard(bool flip);

	void updateInfoPanel(bool show);

	// Checker
	bool hasPetCard();
	bool isActiveSpot();
	bool canEvolveTo(PetCard *card);
	PetCard *getActivePetCard() const { return petCard; }
	unsigned int getDmgCounter() const { return dmgCounter; }
	std::vector<PetCard*>& getPrevEvCards() { return preEvCardVec; }
	std::vector<EnergyCard*>& getAttachedEnergyCards() { return energyCardVec; }
protected:
	virtual void clear();

	HolderType type;
	BattleScene *btlScn = nullptr;
	PlayerIdType ownerId = PlayerIdInvalid;

	// Thong tin cua pet trong tran dau
	int dmgCounter = 0;
	int playedTurn = -1;

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
	cocos2d::ui::Layout *showInfoPanel = nullptr;

	// Utilize functions

	void onTouchHolder();
};

BATTLE_SCENE_NS_END


#endif // __CARD_HOLDER_H__
