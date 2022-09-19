#pragma once

#include "cocos2d.h"
#include "../BSDefine.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>
#include <memory>

class EventHandler;

namespace MGame {
	class BattleMaster;
	class PlayerAction;
};

BATTLE_SCENE_NS_BEG

class BattleScene;

class BSAction {
public:
	friend class BattleScene;

	enum class ActionType{
		None,

		Custom,
		Sequence,

		Wait,
		Draw_Card,
		Draw_Pize_Cards,
		
		StartSetup,
		StartSetupActive,
		SetupActive,
		StartSetupBench,
		SetupBench,
		FirstPlayerFlip,
		EndSetup,

		ChooseTurnAction,
		AttachEnergy,

		Select_Prize,
		Get_Prize_Cards
	};

	enum class State{
		Wait,
		Processed,
		Done
	};

	using ActionPtr = std::shared_ptr<BSAction>;

	BSAction();
	virtual ~BSAction();

	virtual void executeOn(BattleScene *btlScn) = 0;
	virtual ActionType getType() const = 0;

	State state = State::Wait;
};


class SequenceAction : public BSAction {
public:
	using ActionPtr = std::shared_ptr<BSAction>;

	static std::shared_ptr<SequenceAction> create(std::initializer_list<ActionPtr> list);
	
	SequenceAction() = default;
	SequenceAction(std::initializer_list<ActionPtr> list);
	virtual ~SequenceAction();

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::Sequence; }

	std::list<std::shared_ptr<BSAction>> actions;
};

class CustomAction : public BSAction{
public:
	using CustomFunction = std::function<void()>;
	CustomAction(const CustomFunction &f);
	virtual ~CustomAction();

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::Custom; }
protected:
	std::function<void()> doFunc;
};

class WaitAction : public BSAction {
public:
	static const std::string WAIT_ACTION_SCHEDULER;
	WaitAction(float t);
	virtual ~WaitAction();

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::Wait; }
protected:
	float waitTime = 0.0f;
};

class DrawCardAction : public BSAction {
public:
	using CardId = std::string;
	using CardList = std::initializer_list<CardId>;

	DrawCardAction() = default;
	DrawCardAction(const PlayerIdType &id);
	DrawCardAction(const PlayerIdType &id, size_t n, CardList list);
	DrawCardAction(const PlayerIdType &id, size_t n, const std::vector<CardId> &list);

	virtual ~DrawCardAction();

	virtual void setDraw(size_t, CardList list);
	virtual void setDraw(size_t, const std::vector<CardId> &list);

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::Custom; }
	
protected:
	PlayerIdType pid = PlayerIdInvalid;
	size_t drawnNum = 0;
	std::vector<CardId> drawnCards;
	std::shared_ptr<EventHandler> onDrawnDone;
};

class DrawPrizeCards : public BSAction {
public:
	using CardId = std::string;
	using CardList = std::initializer_list<CardId>;

	DrawPrizeCards(const PlayerIdType &id) : pid(id) {};
	DrawPrizeCards(const PlayerIdType &id, const std::vector<CardId> &list);
	DrawPrizeCards(const PlayerIdType &id, CardList list);
	virtual ~DrawPrizeCards() = default;

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::Draw_Pize_Cards; }

protected:
	PlayerIdType pid = PlayerIdInvalid;
	std::vector<CardId> drawnCards;
	std::shared_ptr<EventHandler> onDrawnDone;

};

class GetPrizeCards : public BSAction {
public:
	using CardId = std::string;

	GetPrizeCards(const PlayerIdType &id, const const std::vector<unsigned int> &vec) : pid(id), idxVec(vec.begin(), vec.end()) {};
	GetPrizeCards(const PlayerIdType &id, std::initializer_list<unsigned int> vec) : pid(id), idxVec(vec.begin(), vec.end()) {}
	virtual ~GetPrizeCards() = default;

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::Get_Prize_Cards; }

protected:
	PlayerIdType pid = PlayerIdInvalid;
	std::vector<unsigned int> idxVec;
	std::shared_ptr<EventHandler> onGetDone;
};


//class FlipCoinAction : public BSAction {
//public:
//	enum class FlipType {
//		None, 
//		Flip_1,
//		Flip_Mul,
//		Flip_Until_Tails
//	};
//	static FlipCoinAction* createFlip1Coin(std::shared_ptr<BattleManager> &btlMgr, PlayerIdType id);
//	static FlipCoinAction* createFlipMulCoins(std::shared_ptr<BattleManager> &btlMgr, PlayerIdType id, unsigned int count);
//	static FlipCoinAction* createFlipUntilTails(std::shared_ptr<BattleManager> &btlMgr, PlayerIdType id);
//
//	FlipCoinAction(std::shared_ptr<BattleManager> &btlMgr, PlayerIdType id);
//	virtual ~FlipCoinAction();
//
//	virtual void start() override;
//	virtual void end() override;
//	virtual ActionType getType() override { return ActionType::Custom; }
//
//protected:
//	PlayerIdType playerId = PlayerIdInvalid;
//	FlipType flipType = FlipType::None;
//	unsigned int flipNum = 0;
//	bool untilTails = false;
//};
//
//class EndGameAction : public BSAction {
//public:
//	//static EndGameAction* createFoundWinner();
//	//static EndGameAction* createDrawMatch();
//};
//



class WaitInputPlayer : public BSAction {
public:
	virtual bool onReceivePlayerInput(const std::shared_ptr<MGame::BattleMaster> &bm, const std::shared_ptr<MGame::PlayerAction> &pAction) { return false; }
};

//-----------//
//SET UP FLOW//
//-----------//

class StartSetupAction : public BSAction {
public:
	const static std::string START_SETUP_TXT;

	StartSetupAction() = default;
	virtual ~StartSetupAction() = default;

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::StartSetup; }
};

class StartSetupActive : public WaitInputPlayer {
public:
	const static std::string PLAYER_SETUP_TXT;
	const static std::string OPP_SETUP_TXT;

	StartSetupActive(const PlayerIdType &id) : pid(id) {};
	virtual ~StartSetupActive() = default;

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { 
		return ActionType::StartSetupActive; 
	}
	virtual bool onReceivePlayerInput(const std::shared_ptr<MGame::BattleMaster> &bm, const std::shared_ptr<MGame::PlayerAction> &pAction) override;
protected:
	PlayerIdType pid;
};

class StartSetupBench : public WaitInputPlayer {
public:
	static const std::string PLAYER_SETUP_TXT;
	static const std::string OPP_SETUP_TXT;

	StartSetupBench(const PlayerIdType &id) : pid(id) {}
	virtual ~StartSetupBench() = default;

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::StartSetupBench; }
	virtual bool onReceivePlayerInput(const std::shared_ptr<MGame::BattleMaster> &bm, const std::shared_ptr<MGame::PlayerAction> &pAction) override;

protected:
	PlayerIdType pid;
};

class DoSetupPetActive : public BSAction {
public:
	static const std::string OPP_DO_SETUP_TXT;
	DoSetupPetActive(const PlayerIdType &id, unsigned int hIdx) : pid(id), handIdx(hIdx) {}
	virtual ~DoSetupPetActive() = default;

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::SetupActive; }
protected:
	PlayerIdType pid;
	unsigned int handIdx;
};

class DoSetupPetBench : public BSAction {
public:
	DoSetupPetBench(const PlayerIdType &id, unsigned int hIdx) : pid(id), handIdx(hIdx) {}
	virtual ~DoSetupPetBench() = default;

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::SetupBench; }

protected:
	PlayerIdType pid;
	unsigned int handIdx = 0;
};

class FlipCoinGetFirstPlayer : public BSAction {
public:
	static const std::string YOU_GO_FIRST_TXT;
	static const std::string YOU_GO_SECOND_TXT;

	FlipCoinGetFirstPlayer(const PlayerIdType &pid) : firstId(pid){}
	virtual ~FlipCoinGetFirstPlayer() = default;

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::FirstPlayerFlip; }
protected:
	PlayerIdType firstId;
	bool resSide;
	std::shared_ptr<EventHandler> onFlipDone;
};

class DoEndSetup : public BSAction {
public:
	static const std::string OPP_DO_SETUP_TXT;
	DoEndSetup() = default;
	virtual ~DoEndSetup() = default;

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::EndSetup; }
};


//-----------//
//-----------//
//-----------//


// Selection Classes //

class SelectPrizeAction : public WaitInputPlayer {
public:
	//const static std::string PLAYER_DO_ACTION;
	//const static std::string OPP_DO_ACTION;

	SelectPrizeAction(const PlayerIdType &id, unsigned int selectNum) : pid(id), num(selectNum) {};
	virtual ~SelectPrizeAction() = default;

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::Select_Prize;}
	virtual bool onReceivePlayerInput(const std::shared_ptr<MGame::BattleMaster> &bm, const std::shared_ptr<MGame::PlayerAction> &pAction) override;
protected:
	PlayerIdType pid;
	unsigned int num = 0;

};

//-------------------//

class DoTurnStart : public BSAction {
public:
	static const std::string PLAYER_TXT;
	static const std::string OPP_TXT;
	DoTurnStart(const PlayerIdType &id) : pid(id) {}
	virtual ~DoTurnStart() = default;

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::Sequence; }
protected:
	PlayerIdType pid;
};

class PlayerChooseTurnAction : public WaitInputPlayer {
public:
	//const static std::string PLAYER_DO_ACTION;
	//const static std::string OPP_DO_ACTION;

	PlayerChooseTurnAction(const PlayerIdType &id) : pid(id) {};
	virtual ~PlayerChooseTurnAction() = default;

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::ChooseTurnAction;}
	virtual bool onReceivePlayerInput(const std::shared_ptr<MGame::BattleMaster> &bm, const std::shared_ptr<MGame::PlayerAction> &pAction) override;
protected:
	PlayerIdType pid;
	unsigned int num = 0;

};

class PlayerEnergyCard : public BSAction {
public:
	PlayerEnergyCard(const PlayerIdType &id, unsigned int handIdx, bool active, unsigned int bIdx = 0) : 
		pid(id),
		hIdx(handIdx),
		isActive(active),
		benchIdx(bIdx)
	{}
	virtual ~PlayerEnergyCard() = default;

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::AttachEnergy; }
protected:
	PlayerIdType pid;
	unsigned int hIdx;
	bool isActive = false;
	unsigned int benchIdx;
};


class GameOverAction : public BSAction {
public:
	
	GameOverAction(const PlayerIdType &pid = "");
	virtual ~GameOverAction();

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::Sequence; }
protected:
	PlayerIdType winnerId;
};



BATTLE_SCENE_NS_END