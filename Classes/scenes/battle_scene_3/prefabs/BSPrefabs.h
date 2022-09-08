#pragma once

#include "../BSDefine.h"

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>
#include <string>

#define BS_PREFABS_NS_BEG namespace BSPrefabNS {
#define BS_PREFABS_NS_END }
#define BS_PREFABS_USE_NS using namespace BSPrefabNS // Ket thuc bang ';'

BS_PREFABS_NS_BEG

/*
	Class EnergyItem
*/

class EnergyItem : public cocos2d::Node {
public:
	using EnergyId = std::string;

	static EnergyItem* create(EnergyId eid, unsigned int c = 1);

	cocos2d::Sprite *energySprite = nullptr;
	cocos2d::ui::Text *energyText = nullptr;

	void stack(unsigned int inc, bool doAnim = true);
	cocos2d::Size getEnergySize() const;
protected:
	static std::unordered_map<EnergyId, std::string> energySFNMap;

	EnergyItem(EnergyId eid, unsigned int c);
	virtual ~EnergyItem();

	virtual bool init() override;

	EnergyId id; // Ex : "Fire" or "Water"
	unsigned int count = 0;

	cocos2d::Node *root = nullptr;
};

BS_PREFABS_NS_END

