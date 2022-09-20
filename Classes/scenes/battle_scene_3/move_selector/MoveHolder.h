#pragma once

#include "base_classes/EventsHandler/IEventsHandler.h"

#include "../BSDefine.h"

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"
#include "ui/UIImageView.h"
#include "ui/UIListView.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>

class MoveData;

BATTLE_SCENE_NS_BEG

class MoveHolder : public cocos2d::ui::Layout
{
public:
	
	using ClickUseBtnFunc = std::function<void(MoveHolder*)>;
	using CardId = std::string;

	static MoveHolder* create(const std::shared_ptr<const MoveData> &data);
	MoveHolder(const std::shared_ptr<const MoveData> &data);
	virtual ~MoveHolder();

	cocos2d::Node* getRoot() const { return root; }

	void cleanUI();
	void setEnabledUseBtn(bool enabled);
	void setClickUseBtnFunc(const ClickUseBtnFunc &f) { onUseMove = f; }

	const std::map<std::string, unsigned int>& getRequiredEnergies() const;
protected:

	virtual bool init();

	cocos2d::Node *root = nullptr;
	cocos2d::ui::Layout *container = nullptr;
	cocos2d::ui::Text *moveNameLb = nullptr;
	cocos2d::ui::Text *dmgLb = nullptr;
	cocos2d::ui::Button *useBtn = nullptr;
	cocos2d::ui::Layout *energyPanel = nullptr;
	cocos2d::ui::Layout *desBoard = nullptr;
	std::shared_ptr<const MoveData> data;
	
	ClickUseBtnFunc onUseMove = nullptr;

	void setMoveDes(const std::string &s);
	void updateEnergyPanel();
};

BATTLE_SCENE_NS_END