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

BATTLE_SCENE_NS_BEG

class BattleScene;
class BSCard;

class BSPrizePile : public IEventsHandler
{
public:
	friend class BSPrizeSelector;

	/// All Events ///
	static const std::string EV_DRAW_ACTION_DONE; // Event Data is Null

	static const float CardScale;

	using CardId = std::string;
	static std::shared_ptr<BSPrizePile> create(BattleScene *scn, const PlayerIdType &id);
	BSPrizePile(BattleScene *scn, const PlayerIdType &id);
	virtual ~BSPrizePile();

	void addCard(BSCard *card, unsigned int idx = 0, float delay = 0.0f);

	void drawFromDeck(const std::vector<CardId> &idvec);
	void drawFromSelected();
	void drawFromSelected(std::initializer_list<unsigned int> idxList);
	void showSelectPanel();
protected:

	virtual bool init();

	BattleScene *btlScn = nullptr;
	cocos2d::ui::Layout *pPanel = nullptr;
	PlayerIdType pid;

	std::vector<unsigned int> selectedVec;

	cocos2d::ui::Layout *prizeBoard = nullptr;
	cocos2d::Node *cardMarker = nullptr;
	cocos2d::ui::Text *numLabel = nullptr;

	//std::vector<BSCard*> cardVec;
	std::map<unsigned int, BSCard*> cardMap;
	unsigned int capacity = 0;
	unsigned int curCount = 0;
};

BATTLE_SCENE_NS_END