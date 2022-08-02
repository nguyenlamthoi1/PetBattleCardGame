#ifndef __BS_BOARD_H__
#define __BS_BOARD_H__

#include "BSDefine.h"
#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>

BATTLE_SCENE_NS_BEG

class BattleScene;
class CardHolder;

class BSBoard final {
public:
	friend class BattleScene;
private:
	static BSBoard* create(BattleScene *scn, PlayerIdType id);
	BSBoard(BattleScene *scn, PlayerIdType ownerId);
	~BSBoard();

	bool init();

	PlayerIdType ownerId = PlayerIdInvalid;
	BattleScene *btlScn = nullptr;

	cocos2d::ui::Layout *boardPanel = nullptr; // * Parent cua tat ca node khac

	size_t maxBenchCapacity = DEFAULT_MAX_BENCH_CAPACITY; // default : 5
	float holderScale = 1.0f;
	cocos2d::ui::Layout *benchBoard = nullptr;
	std::vector<CardHolder*> benchHolders;

	cocos2d::ui::Layout *activeBoard = nullptr;
	CardHolder *activeHolder = nullptr;

	// Utilize functions
	void alignHoldersOnBenchBoard(bool forceDoLayout);
};

BATTLE_SCENE_NS_END

#endif // __BS_BOARD_H__
