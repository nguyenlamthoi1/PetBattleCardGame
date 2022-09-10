#pragma once

#include "base_classes/EventsHandler/IEventsHandler.h"

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

class BSCoinFlipper final : public IEventsHandler {
public:
	enum class FlipType {
		None,
		Flip_1,
		Flip_Mul
	};

	/// All Events ///
	static const std::string EV_FLIP_DONE;
	struct EV_FlipDoneData {
		FlipType type = FlipType::None;
	};

	using OnFlipEnded = std::function<void()>;
	class FlipperData;

	using SideType = bool;
	const static SideType HEADS = true;
	const static SideType TAILS = false;

	static BSCoinFlipper* create(BattleScene *scn);
	BSCoinFlipper(BattleScene *scn);
	~BSCoinFlipper();

	void startFlip1Coin(const PlayerIdType &whoFlip, SideType resSide);
	void startFlipMulCoins(PlayerIdType whoFlip, unsigned int n);
	void hideFlip1(float afterDt = 0.0f) const;

	cocos2d::Node* getRoot() const { return root; }

	enum class EventType {
		Flip_Action_Ended
	};
	void registFlipEndCallbackOnce(const OnFlipEnded &f);

private:
	static const std::string DELAY_SCHEDULER;
	using DelayFunc = std::function<void(float t)>;

	bool init();

	BattleScene *btlScn = nullptr;

	cocos2d::Node *root = nullptr;
	cocos2d::Node *flip1Panel = nullptr;
	cocos2d::Node *flipMulPanel = nullptr;
	cocos2d::Sprite *coinSprite = nullptr;

	static const std::string FLIP_BEGIN_ANIM;
	static const std::string FLIP_END_ANIM;

	FlipType curFlipType = FlipType::None; // Loai Flip dang thuc hien
	PlayerIdType currentFlipper = PlayerIdInvalid; // Player dang thuc hien flip
	std::unordered_map<PlayerIdType, std::shared_ptr<FlipperData>> flipperDatas; // Ket Qua Flip

	void createFlipAnimation(PlayerIdType whoFlip);
	void onFlipActionDone(); // * Called khi flip action ket thuc

	void doFlip1Coin_Flip1Type(const PlayerIdType &whoFlip, SideType resSide); // * call sau khi call startFlip1Coin
	void doFlip1Coin_FlipMulType(PlayerIdType whoFlip); // * call sau khi call startFlipMulCoins
	
	void notifyEvent(EventType ev);
	void callDelayFunc(const DelayFunc &f, float t = 0.0f);
	std::list<OnFlipEnded> onFlipEndedOnce; // List cac callback
};


class BSCoinFlipper::FlipperData {
public:
	FlipperData(const PlayerIdType &id, std::string coin) : pid(id), usedCoin(coin) {};
	~FlipperData() = default;

	void setFlipResult(std::initializer_list<SideType> vec);
	void setFlipResult(const std::vector<SideType> &vec);
	bool hasResult() const { return !resVec.empty(); }
	void clearResult();
	unsigned int getFlipCount() const { return resVec.size(); }

	// Thong tin Flipper
	PlayerIdType pid;
	std::string usedCoin; // Loai Coin duoc su dung

	/// Thong tin Animation cua Flip Coin
	std::vector<std::string> flipVec; //  Sprite Frame Name Array cho animation flip
	std::string SFNHeads;
	std::string SFNTails;

	// Thong tin ket qua Flip
	std::vector<SideType> resVec; // Vector ket qua cua tat ca cac lan toss coin
	std::unordered_map<SideType, unsigned int> resMap; // Ket qua bao nhieu lan heads, bao nhieu lan tails
};

BATTLE_SCENE_NS_END