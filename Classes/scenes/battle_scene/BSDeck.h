#ifndef __BS_DECK_H__
#define __BS_DECK_H__

#include "cocos2d.h"
#include "ui/UIText.h"
#include "BSDefine.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>

BATTLE_SCENE_NS_BEG

class BattleScene;
class BSCard;

class BSDeck final {
public:
	friend class BattleScene;

	std::vector<BSCard*> drawTop(size_t n);

	/*
		Hint: function return end iterator neu draw khong thanh cong, nguoc lai return iterator den vi tri phan tu dau tien duoc add
	*/
	std::vector<BSCard*>::iterator drawTop(size_t n, std::vector<BSCard*> &v);
	void clear();

private:
	static BSDeck* create(BattleScene *scn, PlayerIdType playerId);
	BSDeck(BattleScene *scn, PlayerIdType playerId);
	~BSDeck();
	bool init();

	PlayerIdType ownerId = PlayerIdInvalid;
	BattleScene *btlScn = nullptr;
	std::vector<BSCard*> cards;
	cocos2d::Sprite *deckIcon = nullptr;
	cocos2d::ui::Text *deckText = nullptr;
};

BATTLE_SCENE_NS_END

#endif // __BS_DECK_H__
