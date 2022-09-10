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
class PetCard;

class BSBoard final {
public:
	friend class BattleScene;
//private:
	static BSBoard* create(BattleScene *scn, PlayerIdType id);
	BSBoard(BattleScene *scn, PlayerIdType ownerId);
	~BSBoard();

	bool init();

	bool checkCanAddPetCard(PetCard *card, cocos2d::Node* dest) const;

	PlayerIdType ownerId = PlayerIdInvalid;
	BattleScene *btlScn = nullptr;

	cocos2d::ui::Layout *boardPanel = nullptr; // * Parent cua tat ca node khac

	size_t maxBenchCapacity = DEFAULT_MAX_BENCH_CAPACITY; // default : 5
	size_t curBenchNum = 0;

	float holderScale = 1.0f;
	cocos2d::ui::Layout *benchBoard = nullptr;
	std::vector<CardHolder*> benchHolders;

	cocos2d::ui::Layout *activeBoard = nullptr;
	CardHolder *activeHolder = nullptr;

	// Getters
	CardHolder* getActiveHolder() const;
	const std::vector<CardHolder*>& getBenchHolders() const;
	const std::vector<cocos2d::Node*> getAllHolders() const;

	static const std::string BENCH_BOARD_NAME;
	static const std::string ACTIVE_BOARD_NAME;
	bool isBenchBoard(cocos2d::Node* node) const { return node == benchBoard; }
	bool isActiveBoard(cocos2d::Node* node) const { return node == activeBoard; }
	cocos2d::ui::Layout* getBenchBoard() const { return benchBoard; }
	cocos2d::ui::Layout* getActiveBoard() const { return activeBoard; }

	bool hasActivePet() const;
	bool isBenchFull() const;

	bool addPetOnActive(PetCard *card, const std::function<void()> &onDone = nullptr);
	bool addPetOnBoard(PetCard *card, const std::function<void()> &onDone = nullptr);

	// Utilize functions
	void alignHoldersOnBenchBoard(bool forceDoLayout);

	// Checker
};

BATTLE_SCENE_NS_END

#endif // __BS_BOARD_H__
