#ifndef __BS_CARD_H__
#define __BS_CARD_H__

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"

#include "BSDefine.h"
#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>

class CardData;
class PetCardData;

BATTLE_SCENE_NS_BEG

class BSCard : public cocos2d::ui::Layout {
public:
	static const cocos2d::Size CARD_SIZE;
	static BSCard* createWithData(const CardData *data);

protected:
	BSCard();
	virtual ~BSCard();
	virtual bool init() override = 0;
	virtual bool initWithData(const CardData* data) = 0;
};

class PetCard : public BSCard {
public:
	static PetCard* create(const PetCardData *data);
	static PetCard* create();

	virtual bool init() override;
	virtual bool initWithData(const CardData *data) override;

	const PetCardData *data = nullptr;

	cocos2d::Node *cardNode = nullptr;
	cocos2d::ui::Text *nameText = nullptr;
	cocos2d::ui::Text *hpText = nullptr;

	bool isEmpty() const { return !data; }
};

BATTLE_SCENE_NS_END

#endif // __BS_CARD_H__
