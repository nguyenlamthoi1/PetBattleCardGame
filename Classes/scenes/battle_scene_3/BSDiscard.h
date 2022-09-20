#pragma once

#include "base_classes/EventsHandler/IEventsHandler.h"

#include "BSDefine.h"

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>
#include <array>

BATTLE_SCENE_NS_BEG

class BattleScene;
class BSCard;
class CardHolder;

class BSDiscardPile : public IEventsHandler
{
public:
	/// All Events ///
	static const std::string EV_DRAW_ACTION_DONE; // Event Data is Null

	static const float CardScale;

	using CardId = std::string;
	static std::shared_ptr<BSDiscardPile> create(BattleScene *scn, const PlayerIdType &id);
	BSDiscardPile(BattleScene *scn, const PlayerIdType &id);
	virtual ~BSDiscardPile();

	void pushCardFromHolder(BSCard *card, CardHolder *holder, float delay = 0.0f, const std::function<void()> &onDone = nullptr);
	void pushCardsFromHolder(const std::vector<BSCard*> &cardVec, CardHolder *holder,  const std::function<void()> &onDone = nullptr);

	//void drawFromDeck(const std::vector<CardId> &idvec);
	//std::vector<BSCard*> draw(const std::vector<unsigned int> &v);
protected:

	virtual bool init();

	BattleScene *btlScn = nullptr;
	cocos2d::ui::Layout *pPanel = nullptr;
	PlayerIdType pid;

	cocos2d::ui::Layout *discardBoard = nullptr;
	cocos2d::Node *cardMarker = nullptr;
	cocos2d::ui::Text *numLabel = nullptr;

	std::vector<BSCard*> cardVec;
};

BATTLE_SCENE_NS_END