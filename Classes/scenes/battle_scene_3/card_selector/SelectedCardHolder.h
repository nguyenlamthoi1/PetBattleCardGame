#pragma once

#include "../BSDefine.h"

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIImageView.h"
#include "ui/UIText.h"


BATTLE_SCENE_NS_BEG
class BSCard;
class BSUnselectedCardHolder;

class BSSelectedCardHolder : public cocos2d::ui::Layout {
public:
	using CardId = std::string;
	static BSSelectedCardHolder* create();
	BSSelectedCardHolder();
	virtual ~BSSelectedCardHolder();

	cocos2d::Node* getRoot() const { return root; }
	void setSelType(GConfig::SelectType type) { selType = type; }
	bool hasCard() const { return card != nullptr; }
	bool canAddCard(BSCard *card) const;
	bool addCard(BSCard* card, BSUnselectedCardHolder *holder);
	BSCard* removeCard();

	BSUnselectedCardHolder *fromHolder = nullptr;
protected:
	virtual bool init() override;

	BSCard *card = nullptr;
	cocos2d::Node *root = nullptr;
	cocos2d::ui::Layout *container = nullptr;
	cocos2d::ui::ImageView *numPanel = nullptr;
	cocos2d::ui::Text *numLabel = nullptr;
	cocos2d::Node *cardMarker = nullptr;

	GConfig::SelectType selType;
};


BATTLE_SCENE_NS_END