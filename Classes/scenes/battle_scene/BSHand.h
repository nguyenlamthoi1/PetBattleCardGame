#ifndef __BS_HAND_H__
#define __BS_HAND_H__
#include "cocos2d.h"
#include "ui/UILayout.h"

#include "BSDefine.h"
#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>

BATTLE_SCENE_NS_BEG

class BattleScene;
class BSCard;

class DrawCardAction;

class BSHand final : public cocos2d::ui::Layout {
public:
	friend class BattleScene;
	void drawCards(size_t n);
private:
	static BSHand* create(BattleScene *scn, PlayerIdType id);

	BSHand(BattleScene *scn, PlayerIdType ownerId);
	~BSHand();

	bool init() override;

	PlayerIdType ownerId = PlayerIdInvalid;
	BattleScene *btlScn;
	std::vector<BSCard*> cards;
};

BATTLE_SCENE_NS_END

#endif // __BS_HAND_H__
