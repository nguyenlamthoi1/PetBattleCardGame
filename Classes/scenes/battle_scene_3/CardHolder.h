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
	using OnSelectFunc = std::function<void(CardHolder *holder)>;

	enum class HolderType {
		Active,
		Bench
	};

	using EnergyId = std::string;

	static cocos2d::Size HOLDER_SIZE;

	static CardHolder* createActive(BattleScene *btlScn, PlayerIdType id);
	static CardHolder* createBench(BattleScene *btlScn, PlayerIdType id, unsigned int benchIdx);

	CardHolder(HolderType hType, BattleScene *btlScn, PlayerIdType id, unsigned int hIdx = 0);
	virtual ~CardHolder();

	virtual bool init() override;
	void setHolderSizeH(float h);
	void setHolderSizeW(float w);
	
	bool tryAddBasicPetCard(PetCard *card, const std::function<void()> &onDone = nullptr);
	bool tryAddEnergyCard(EnergyCard *card, const std::function<void()> &onDone = nullptr);
	bool tryEvolveTo(PetCard *card, const std::function<void()> &onDone = nullptr);
	
	bool tryAttackActiveOpp(CardHolder* taker, unsigned int totalDmg, bool triggerWeak, bool triggerResist, const std::function<void()> &onDone = nullptr);
	void onTakeDamage(unsigned int totalDmg, bool triggerWeak, bool triggerResist);

	void setFlipPetCard(bool flip);

	void updateInfoPanel(bool show);
	void updateDmgImg(bool anim = false);

	void addEnergyItem(const std::string eType, unsigned int eNum);
	void launchFlyingMsg(const std::string &msg, cocos2d::Color3B color, float delay = 0.0f);
	
	void removePetAndAllCards(std::vector<BSCard*> &vec);
	// Checker
	PlayerIdType getOwnerId() const { return ownerId; }
	unsigned int getHolderIdx() const { return holderIdx; }
	bool isType(HolderType hType) const { return type == hType; }
	PlayerIdType getPid() const { return ownerId; }
	bool checkCanUseMove(unsigned int moveIdx) const;
	bool hasPetCard();
	bool isActiveSpot();
	bool canEvolveTo(PetCard *card);
	PetCard *getActivePetCard() const { return petCard; }
	unsigned int getDmgCounter() const { return dmgCounter; }
	std::vector<PetCard*>& getPrevEvCards() { return preEvCardVec; }
	std::vector<EnergyCard*>& getAttachedEnergyCards() { return energyCardVec; }

	void switchWithHolder(CardHolder *holder, const std::function<void()> &onDone = nullptr);

	void setOnSelectCallback(const OnSelectFunc & func) { onSelectCb = func; }
	void setSelectable(bool e);
	void setSelected(bool selected);
	bool checkSelected() const {return isSelected;}
protected:
	virtual void clear();

	const static cocos2d::Color3B SELECTED_COLOR;
	const static cocos2d::Color3B UNSELECTED_COLOR;
	cocos2d::ui::Layout *selectLayout = nullptr;
	OnSelectFunc onSelectCb;
	bool isSelected = false;

	HolderType type;
	BattleScene *btlScn = nullptr;
	PlayerIdType ownerId = PlayerIdInvalid;
	unsigned int holderIdx = 0;

	// Thong tin cua pet trong tran dau
	unsigned int dmgCounter = 0;
	unsigned int maxHp = 0;
	int playedTurn = 0;

	PetCard *petCard = nullptr;
	std::vector<PetCard*> preEvCardVec;
	std::vector<EnergyCard*> energyCardVec;


	cocos2d::Node *cardMarker = nullptr;
	//cocos2d::Node *evCardMarker = nullptr;
	cocos2d::Node *energyCardMarker = nullptr;

	cocos2d::ui::ImageView *dmgImg = nullptr;
	cocos2d::ui::Text *dmgTxt = nullptr;

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

	struct HolderData {
		// Thong tin cua pet trong tran dau
		unsigned int dmgCounter = 0;
		unsigned int maxHp = 0;
		int playedTurn = 0;
		PetCard *petCard = nullptr;
		std::vector<PetCard*> preEvCardVec;
		std::vector<EnergyCard*> energyCardVec;
		std::map<EnergyId, BSPrefabNS::EnergyItem*> energyItemMap;
	};

	void replaceWithNewNode(cocos2d::Node *);

};



BATTLE_SCENE_NS_END


#endif // __CARD_HOLDER_H__
