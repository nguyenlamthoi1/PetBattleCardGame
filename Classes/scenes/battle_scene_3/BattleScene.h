#pragma once

#include "BSDefine.h"

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"
#include "ui/UIImageView.h"
#include "ui/UIButton.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>

class PlayerData;
class CardData;
class PetCardData;
class EnergyCardData;

namespace MGame {
	class BattleMaster;
	//class PlayerAction;
}

BATTLE_SCENE_NS_BEG

class BSResources;
class BSDeck;

class BSCard;
class EnergyCard;
class PetCard;
class BSBoard;

//class BSAction;
//class BSPlayer;
//class BSCoinFlipper;
class BSNotifier;
class BSHand;
class BSAction;

//namespace MGame {
//	class BattleManager;
//}


namespace BATTLE_SCENE_Z {
	const float DETAILED_CARD = 1000;
};

class BattleScene final : public cocos2d::Scene
{
public:
	enum class Phase {
		None,
		Start,
		Setup,
		PlayerTurn,
		BetweenTurn,
		End
	};

	static BattleScene* BattleScene::getScene();
	static BattleScene* getScn();
	static BattleScene* create();
private:
	static BattleScene *curBtlScn;
public:
	static std::shared_ptr<BSResources> getResources();

	cocos2d::Node* getScnRoot() const { return root; } // * Root node load tu file csb

	std::shared_ptr<PlayerData> getPlayerData(const PlayerIdType &id) const;
	std::shared_ptr<BSHand> getHand(const PlayerIdType &id) const;
	std::shared_ptr<BSDeck> getDeck(const PlayerIdType &id) const;
	std::shared_ptr<BSNotifier> getNotifier() const;
	std::shared_ptr<BSBoard> getBoard(const PlayerIdType &id) const;
	
	//std::shared_ptr<BSPlayer> getPlayer(PlayerIdType id) { return players[id]; }
	//std::shared_ptr<BSCoinFlipper> getCoinFlipper() { return coinFlipper; }

	PlayerIdType getPlayerId() const { return pid; }
	PlayerIdType getOpponentId() const { return oid; }

	unsigned int getTurnCount() const { return turnCount; }

private:
	BattleScene();
	virtual ~BattleScene();

	virtual bool init() override;
	void initTopLayer();

	virtual void onEnterTransitionDidFinish() override;
	virtual void onExit() override;

	void startLoadingAndInit();
	void initGame();
	void startGame();

	std::shared_ptr<BSResources> bsres; // Quan ly cac Asset duoc tao trong Scene nhu: Card,..

	std::unordered_map<PlayerIdType, std::shared_ptr<PlayerData>> playerData; // * Thong tin data cua moi nguoi choi, bao gom: deck, card back, loai coin duoc dung, avatar,..

	Phase phase = Phase::None;
	unsigned int turnCount = 0;
	unsigned int curPlayer = 0;
	std::vector<PlayerIdType> pids;

	PlayerIdType pid = PlayerIdInvalid; // Id cua nguoi choi
	PlayerIdType oid = PlayerIdInvalid; // Id cua doi thu

	///Graphic Data member///
	cocos2d::Node *root = nullptr; // * Scene load tu csb

	std::unordered_map<PlayerIdType, cocos2d::ui::Layout*> playerPanels; // Panel chua cac graphic objects cua moi nguoi choi
	cocos2d::ui::Layout *topLayout = nullptr;
	cocos2d::ui::Layout *loadingLayout = nullptr;

	std::unordered_map<PlayerIdType, std::shared_ptr<BSHand>> hands;
	std::unordered_map<PlayerIdType, std::shared_ptr<BSDeck>> decks;
	std::unordered_map<PlayerIdType, std::shared_ptr<BSBoard>> boards;

	std::shared_ptr<BSNotifier> notifier;

	cocos2d::ui::Button *endTurnBtn = nullptr;

//	std::shared_ptr<BSCoinFlipper> coinFlipper;

	///Action pipline members///
	const static std::string PIPELINE_SCHEDULER;
	enum class PipelineState{
		Wait,
		Process,
	};
	PipelineState pipState = PipelineState::Wait;
	using ActionPtr = std::shared_ptr<BSAction>;
	std::list<ActionPtr> pipeline;
	float waitTime = 0.0f; // Neu waitTime la thoi gian pipeline phai doi truoc khi tiep tuc xu ly pipeline

public:
	void updatePipeline(float dt);
	void startPipeline();
	void stopPipeline();
	void clearPipeline();
	void pushAction(const ActionPtr &ptr);
	void pushActions(std::initializer_list<ActionPtr> list);
	void popFront();
	void insertBehindAction(const ActionPtr &ptr, const std::vector<ActionPtr> &actionVec);
	void setPipelineWait(float t) { waitTime = t; }
	
	///--Action pipeline-end--
	enum class PlaceType{
		None,
		Active,
		Bench,
		Supporter,
		Item
	};
	bool onPlayerPetCard(const PlayerIdType &playerId, unsigned int handIdx, PlaceType PlaceType);
	bool onPlayerEndTurn(const PlayerIdType &playerId);

	void setEnableEndTurnButton(bool enabled);
	using EndClickFunc = std::function<void(cocos2d::Ref*)>;
	void setClickEndButton(const EndClickFunc &f);
private:
	void checkClean();

public:
	void showCardDetail(const std::shared_ptr<const CardData> &data);
	void hideCardDetail();
private:
	void showPetCardDetail(const std::shared_ptr<const PetCardData> &data);
	void showEnergyCardDetail(const std::shared_ptr<const EnergyCardData> &data);
	void showItemCardDetail(const std::shared_ptr<const CardData> &data);
	void showSotCardDetail(const std::shared_ptr<const CardData> &data);

	cocos2d::Node *detailLayout = nullptr;
	BSCard *detailedPetCard = nullptr;
	BSCard *detailedEnergyCard = nullptr;
};

BATTLE_SCENE_NS_END