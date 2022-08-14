#ifndef __BS_COIN_FLIPPER_H__
#define __BS_COIN_FLIPPER_H__

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

class BSCoinFlipper final {
public:
	friend class BattleScene;

	using OnFlipEnded = std::function<void()>;

	enum class FlipType {
		None,
		Flip_1,
		Flip_Mul
	};

	static BSCoinFlipper* create(BattleScene *scn);
	BSCoinFlipper(BattleScene *scn);
	~BSCoinFlipper();

	void startFlip1Coin(PlayerIdType whoFlip);
	void startFlipMulCoins(PlayerIdType whoFlip, unsigned int n);
	cocos2d::Node* getNode() const { return root; }

	enum class EventType {
		Flip_Action_Ended
	};
	void registFlipEndCallbackOnce(const OnFlipEnded &f);

private:

	bool init();

	BattleScene *btlScn = nullptr;

	cocos2d::Node *root = nullptr;
	cocos2d::Node *flip1Panel = nullptr;
	cocos2d::Node *flipMulPanel = nullptr;
	cocos2d::Sprite *coinSprite = nullptr;

	static const std::string FLIP_BEGIN_ANIM;
	static const std::string FLIP_END_ANIM;

	using SideType = unsigned char;
	const SideType HEADS = 1;
	const SideType TAILS = 0;

	struct FlipInfo {
		friend class BSCoinFlipper;
		PlayerIdType ownerId = PlayerIdInvalid;
		std::string usedCoin;
		std::vector<SideType> resVec; // Vector ket qua cua tat ca cac lan toss coin
		unsigned char doneCount = 0; // Tong so lan da toss coin -> Neu doneCount == resVec.size() Thi da hoan thanh Flip Coin Action
		std::unordered_map<SideType, unsigned int> result; // Ket qua bao nhieu lan heads, bao nhieu lan tails
		void clear() {
			resVec.clear();
			result.clear();
			doneCount = 0;
		}
		FlipInfo(PlayerIdType id, const std::string coin) : ownerId(id), usedCoin(coin){}
		FlipInfo() = default;
	private:
		std::vector<std::string> flipVec; //  Sprite Frame Name Array cho animation flip
		std::string SFNHeads;
		std::string SFNTails;
	};

	FlipType curFlipType = FlipType::None;
	PlayerIdType currentFlipper = PlayerIdInvalid;
	std::unordered_map<PlayerIdType, FlipInfo> flipInfos;

	std::list<OnFlipEnded> onFlipEndedOnce; // List cac callback

	void createFlipAnimation(PlayerIdType whoFlip);
	void onFlipActionDone(); // * Called khi flip action ket thuc
	void doFlip1Coin(PlayerIdType whoFlip);
	
	void notifyEvent(EventType ev);
};

BATTLE_SCENE_NS_END

#endif // __BS_COIN_FLIPPER_H__
