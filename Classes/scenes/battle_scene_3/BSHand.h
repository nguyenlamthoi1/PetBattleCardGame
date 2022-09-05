#pragma once

#include "base_classes/EventsHandler/IEventsHandler.h"

#include "cocos2d.h"
#include "ui/UILayout.h"

#include "BSDefine.h"
#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>
#include <memory>

BATTLE_SCENE_NS_BEG

class BattleScene;
class BSCard;

class DrawCardAction;

/*
	- Class dieu khien Hand Object
	- Hand Object la Layout, child cua BattleScene
	- Hand Object chua cac Card tren tay cua Player
*/
class BSHand : public IEventsHandler{
public:
	/// All Events ///
	static const std::string EV_DRAW_ACTION_DONE; // Event Data is Null

	using CardId = std::string;
	using HandPtr = std::shared_ptr<BSHand>;

	void drawCards(size_t n, std::function<void()> f = nullptr);
	void drawCards(size_t n, const std::vector<CardId> &v, bool hideCards = false);

	static HandPtr createShPtr(cocos2d::ui::Layout *layout, const PlayerIdType &id);
	BSHand(cocos2d::ui::Layout *layout, PlayerIdType ownerId);
	~BSHand();

protected:
	bool init();

	//// Utilize functions
	//bool onDragEnd(BSCard *card, cocos2d::Node *dest);
	//void onDragBack(BSCard *card);
	//void onDragSucceeded();
	//void updateCardPositions();

protected:
	PlayerIdType pid = PlayerIdInvalid;
	cocos2d::ui::Layout *handLayout = nullptr;
	std::vector<BSCard*> cards;
};

BATTLE_SCENE_NS_END