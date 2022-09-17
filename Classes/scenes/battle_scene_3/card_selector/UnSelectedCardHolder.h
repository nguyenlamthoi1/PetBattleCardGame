#pragma once

#include "../BSDefine.h"

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIImageView.h"
#include "ui/UIText.h"


BATTLE_SCENE_NS_BEG

class BSCard;

class BSUnselectedCardHolder : public cocos2d::ui::Layout {
public:
	using CardId = std::string;
	static BSUnselectedCardHolder* create();
	BSUnselectedCardHolder();
	virtual ~BSUnselectedCardHolder();

	cocos2d::Node* getRoot() const { return root; }
	bool addCard(BSCard *card);
	BSCard* removeCard();
protected:
	virtual bool init() override;

	cocos2d::Node *root = nullptr;
	cocos2d::ui::Layout *container = nullptr;
	cocos2d::ui::ImageView *numPanel = nullptr;
	cocos2d::ui::Text *numLabel = nullptr;
	cocos2d::Node *cardMarker = nullptr;

	std::vector<BSCard*> cardVec;
};


BATTLE_SCENE_NS_END