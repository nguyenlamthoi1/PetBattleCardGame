#pragma once

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

class BSCard;

class BSResources final {
public:
	using CardId = std::string;
	using BSResPtr = std::shared_ptr<BSResources>;

	BSResources();
	virtual ~BSResources();

	BSCard* getBSCard(const CardId &cid = "");

private:
	std::vector<BSCard*> cardVec;

	void clean(); // * Xoa resources

};


BATTLE_SCENE_NS_END