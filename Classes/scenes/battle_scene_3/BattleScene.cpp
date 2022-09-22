#include "BattleScene.h"

#include "GameManager.h"

#include "game/BattleMaster.h"
#include "game/player_actions/PlayerAction.h"

#include "data/PlayerData.h"
#include "data/CardData.h"
#include "scene_actions/BSAction.h"

#include "BSResources.h"
#include "BSHand.h"
#include "BSDeck.h"
#include "BSCard.h"
#include "BSBoard.h"
#include "BSNotifier.h"
#include "BSCoinFlipper.h"
#include "BSPrizePile.h"
#include "BSPlayer.h"
#include "BSDiscard.h"
#include "CardHolder.h"
#include "prize_selector/BSPrizeSelector.h"
#include "card_selector/BSCardSelector.h"
#include "move_selector/BSMoveSelector.h"

#include "scenes/title_scene/TitleScene.h"

#include <stdlib.h>
#include <ctime>

#include "data/PlayerData.h"
#include "ui/UIHelper.h"

#include <new>
#include <algorithm>

USING_NS_CC;
//WIDGET_TOUCH_USE_NS;
using namespace std;
//UTILIZE_USE_NS;

class PlayerData;

BATTLE_SCENE_NS_BEG;

BattleScene *BattleScene::curBtlScn = nullptr;

std::shared_ptr<BSResources> BattleScene::getResources() {
	return getScene()->bsres;
}

BattleScene* BattleScene::getScene() {
	return curBtlScn;
}

BattleScene* BattleScene::getScn() {
	return curBtlScn;
}

BattleScene* BattleScene::create() {
	auto scn = new (nothrow) BattleScene();
	curBtlScn = scn;
	if (scn && scn->init()) {
		scn->autorelease();
		return scn;
	}
	else {
		delete scn;
		scn = nullptr;
		curBtlScn = nullptr;
	}
	
	return scn;
}

BattleScene::BattleScene() :
bsres(make_shared<BSResources>())
{
}

BattleScene::~BattleScene(){
	curBtlScn = nullptr;
	removeAllChildren();
	//clearPipeline();
	//detailedCard = nullptr;

#if DEBUG_CLEAN > 0
	checkClean();
#endif
}

void BattleScene::checkClean() {
//#if DEBUG_CLEAN > 0
//	for (const auto& e : pipeline) {
//		CCASSERT(e == nullptr, "checkClean:: mem leak!");
//	}
//
//	CCASSERT(detailedCard == nullptr, "checkClean:: mem leak!");
//#endif
}

bool BattleScene::init() {
	if (!Scene::init())
		return false;
	srand(time(NULL));
	std::vector<string> oppVec = { "OPP_0", "OPP_1" };
	int randIdx = rand() % oppVec.size();

	pid = "Player";
	oid = oppVec[randIdx];
	pids.clear();
	pids.push_back(pid);
	pids.push_back(oid);

	auto pool = GM_POOL;
	root = pool->tryGetNodeCsb("ccstd_csb/battle_scene/battle_scene.csb");
	ui::Helper::doLayout(root);
	if (!root)
		return false;
	auto director = Director::getInstance();
	auto winSize = director->getWinSize(); // Kich thuoc man hinh
	root->setContentSize(winSize);
	ui::Helper::doLayout(root);
	this->addChild(root);

	playerPanels[pid] = dynamic_cast<ui::Layout*>(root->getChildByName("P1_Panel"));
	playerPanels[oid] = dynamic_cast<ui::Layout*>(root->getChildByName("P2_Panel"));

	// Khoi tao 2 player data
	auto dataMgr = GM_DATA_MGR;
	playerData[pid] = dataMgr->getPlayerData();
	playerData[oid] = dataMgr->getOpponentData("OPP_0"); // TODO: Truyen tham so doi thu vao

	// Khoi tao Hand
	auto handLayout = dynamic_cast<ui::Layout*>(root->getChildByName("P1_Hand_Panel"));
	hands.insert({ pid, BSHand::createShPtr(handLayout, pid) });
	handLayout = dynamic_cast<ui::Layout*>(root->getChildByName("P2_Hand_Panel"));
	hands.insert({ oid, BSHand::createShPtr(handLayout, oid) });

	auto playerPanel = root->getChildByName("P1_Panel");
	auto opponentPanel = root->getChildByName("P2_Panel");

	// Khoi tao deck
	auto deckNode = playerPanel->getChildByName("Deck");
	decks[pid] = BSDeck::createShPtr(deckNode, pid);
	deckNode = opponentPanel->getChildByName("Deck");
	decks[oid] = BSDeck::createShPtr(deckNode, oid);

	// Khoi tao Board
	boards[pid] = shared_ptr<BSBoard>(BSBoard::create(this, pid));
	boards[oid] = shared_ptr<BSBoard>(BSBoard::create(this, oid));

	// Khoi tao PrizePile
	prizePiles[pid] = BSPrizePile::create(this, pid);
	prizePiles[oid] = BSPrizePile::create(this, oid);

	// Khoi tao DiscardPile
	discardPiles[pid] = BSDiscardPile::create(this, pid);
	discardPiles[oid] = BSDiscardPile::create(this, oid);

	// Khoi tao Coin Flipper
	coinFlipper = shared_ptr<BSCoinFlipper>(BSCoinFlipper::create(this));
	coinFlipper->getRoot()->setVisible(false);

	// Khoi tao du lieu player trong game
	players[pid] = make_shared<BSPlayer>(this, pid);
	players[oid] = make_shared<BSPlayer>(this, oid);

	// Khoi tao Notifier
	notifier = shared_ptr<BSNotifier>(BSNotifier::create(this));
	notifier->getNode()->setVisible(true);

	// Khoi tao endButton
	endTurnBtn = dynamic_cast<ui::Button*>(playerPanel->getChildByName("EndTurnButton"));
	endTurnBtn->setVisible(false);

	// Khoi tao Top Layer
	topLayout = dynamic_cast<ui::Layout*>(root->getChildByName("Top_Layer"));
	if (!topLayout)
		return false;
	initTopLayer();

	// Khoi tao PrizeSelector
	prizeSelector = BSPrizeSelector::create(this);
	prizeSelector->getRoot()->setVisible(false);

	// Khoi tao CardSelector
	cardSelector = BSCardSelector::create(this);
	cardSelector->getRoot()->setVisible(false);

	// Khoi tao MoveSelector
	moveSelector = BSMoveSelector::create(this);
	moveSelector->getRoot()->setVisible(false);

	// + Detailed Card
	detailLayout = dynamic_cast<ui::Layout*>(root->getChildByName("Detail_Layout"));

	// Khoi tao ending layout
	endLayout = dynamic_cast<ui::Layout*>(root->getChildByName("End_Layout"));
	endLayout->setVisible(false);

	// Khoi tao Loading Layout
	loadingLayout = dynamic_cast<ui::Layout*>(root->getChildByName("Loading_Layout"));
	if (!loadingLayout)
		return false;
	loadingLayout->setVisible(true);
	return true;
}

void BattleScene::onEnterTransitionDidFinish() {
	Scene::onEnterTransitionDidFinish();
	startLoadingAndInit();
}

void BattleScene::startLoadingAndInit() {
	// Hien thi loading layout
	loadingLayout->setVisible(true);
	loadingLayout->setOpacity(255);

	// Load cac tai nguyen trong game
	// TODO

	// Init Scene
	initGame();

	// Tat hien thi loading layout va bat dau Game
	loadingLayout->runAction(
		Sequence::create(
			DelayTime::create(0.5f),
			FadeOut::create(0.5f),
			CallFunc::create([this]() {
				loadingLayout->setVisible(false);
				startGame();
				}),
			nullptr));
}

void BattleScene::initGame() {
}

void BattleScene::startGame() {
	startPipeline();

	auto bm = MGame::BattleMaster::get();


	turnCount = 0;
	phase = Phase::Start;

	bm->initGame(pid, oid);
	bm->startGame();
}

void BattleScene::initTopLayer() {
	// P1
	auto portraitImg1 = dynamic_cast<ui::ImageView*>(topLayout->getChildByName("Portrait_P1")->getChildByName("Portrait_Image"));
	portraitImg1->loadTexture(playerData[pid]->getAvatarImg(), ui::Widget::TextureResType::PLIST);
	// P2
	auto portraitImg2 = dynamic_cast<ui::ImageView*>(topLayout->getChildByName("Portrait_P2")->getChildByName("Portrait_Image"));
	portraitImg2->loadTexture(playerData[oid]->getAvatarImg(), ui::Widget::TextureResType::PLIST);

	btn1 = dynamic_cast<ui::Button*>(topLayout->getChildByName("Button_1"));
	btn1->addClickEventListener(CC_CALLBACK_1(BattleScene::func1, this));
	btn2 = dynamic_cast<ui::Button*>(topLayout->getChildByName("Button_2"));
	btn2->addClickEventListener(CC_CALLBACK_1(BattleScene::func2, this));

}


void BattleScene::onExit() {
	Scene::onExit();
}

///-------------------///
///Getters and Setters///
///------------------///

shared_ptr<PlayerData> BattleScene::getPlayerData(const PlayerIdType &id) const { return playerData.at(id); }
shared_ptr<BSHand> BattleScene::getHand(const PlayerIdType &id) const { return hands.at(id); }
shared_ptr<BSDeck> BattleScene::getDeck(const PlayerIdType &id) const { return decks.at(id); }
shared_ptr<BSNotifier> BattleScene::getNotifier() const { return notifier; }
shared_ptr<BSBoard> BattleScene::getBoard(const PlayerIdType &id) const { return boards.at(id); }
shared_ptr<BSCoinFlipper> BattleScene::getCoinFlipper() const { return coinFlipper; }
const vector<PlayerIdType>& BattleScene::getPids() const { return pids; }
shared_ptr<BSPrizePile> BattleScene::getPrizePile(const PlayerIdType &id) const { return prizePiles.at(id); }
shared_ptr<BSPrizeSelector> BattleScene::getPrizeSelector() const { return prizeSelector; }
shared_ptr<BSCardSelector> BattleScene::getCardSelector() const { return cardSelector; }
shared_ptr<BSMoveSelector> BattleScene::getMoveSelector() const { return moveSelector; }
shared_ptr<BSPlayer> BattleScene::getBSPlayer(const PlayerIdType &id) const { return players.at(id); }
shared_ptr<BSDiscardPile> BattleScene::getDiscardPile(const PlayerIdType &id) const { return discardPiles.at(id); }


Node* BattleScene::getPrizePileNode(const PlayerIdType &id) const {
	string prefix = id == PLAYER ? "P1" : "P2";
	auto pPanel = root->getChildByName(prefix + "_Panel"); // P1_Panel or P2_Panel
	if (!pPanel)
		return nullptr;
	return pPanel->getChildByName("Prize_Board");
}


///----------------------///
///Action pipline members///
///----------------------///

const string BattleScene::PIPELINE_SCHEDULER = "PIPELINE_SCHEDULER";

void BattleScene::updatePipeline(float dt) {
	//if (waitTime > 0.0f) {
	//	waitTime -= dt;
	//	return;
	//}
	if (!pipeline.empty()) {
		auto first = *(pipeline.begin());
		if (first->state == BSAction::State::Wait) {
			auto sqAction = dynamic_pointer_cast<SequenceAction>(first);
			if (sqAction) {
				pipeline.pop_front();
				pipeline.insert(pipeline.begin(), sqAction->actions.begin(), sqAction->actions.end());
			}
			else {
				pipState = PipelineState::Process;
				first->executeOn(this);
			}
		}
		else if (first->state == BSAction::State::Done) { // Action ket thuc
			pipeline.pop_front();
		}
	}
}

void BattleScene::insertBehindAction(const ActionPtr &atPtr, const std::vector<ActionPtr> &actionVec) {
	auto insertItr = pipeline.begin();
	while (insertItr != pipeline.cend())
	{
		if ((*insertItr) == atPtr)
			break;
		++insertItr;
	}
	if (insertItr != pipeline.cend())
		pipeline.insert(insertItr, actionVec.begin(), actionVec.end());
}

void BattleScene::startPipeline() {
	schedule(CC_SCHEDULE_SELECTOR(BattleScene::updatePipeline));
}

void BattleScene::stopPipeline() {
	unschedule(CC_SCHEDULE_SELECTOR(BattleScene::updatePipeline));
}

bool BattleScene::onPlayerAttachEnergy(const PlayerIdType &playerId, unsigned int handIdx, PlaceType PlaceType, unsigned int benchIdx) {
	//auto &hand = hands.at(playerId);
	//if (!hand->checkIdxValid(handIdx))
	//	return false;
	//if (pipeline.empty())
	//	return false;

	//auto curAction = dynamic_pointer_cast<WaitInputPlayer>(pipeline.front()); // Lay action hien tai cua BattleScene
	//if (!curAction)
	//	return false;
	//auto bm = MGame::BattleMaster::get();
	//return curAction->onReceivePlayerInput(bm, pAction);

	//auto curAction = dynamic_pointer_cast<WaitInputPlayer>(pipeline.front()); // Lay action hien tai cua BattleScene
	//if (!curAction)
	//	return false;

	//shared_ptr<MGame::PlayerAction> playerMove;
	//if (place == PlaceType::Active) {
	//	if (curAction->getType() == BSAction::ActionType::StartSetupActive) {
	//		playerMove = make_shared<MGame::PA_SetupActive>(playerId, handIdx);
	//	}
	//	/*else if (curAction->getType() == BSAction::ActionType::StartSetupBench) {

	//	}*/
	//}
	//else {
	//	if (curAction->getType() == BSAction::ActionType::StartSetupBench) {
	//		playerMove = make_shared<MGame::PA_SetupBench>(playerId, handIdx);
	//	}
	//}

	//if (!playerMove)
	//	return false;

	//auto bm = MGame::BattleMaster::get();
	//return curAction->onReceivePlayerInput(bm, playerMove);

	return true;
}

bool BattleScene::onPlayerPetCard(const PlayerIdType &playerId, unsigned int handIdx, PlaceType place) {
	auto &hand = hands.at(playerId);
	if (!hand->checkIdxValid(handIdx))
		return false;
	if (pipeline.empty())
		return false;

	auto curAction = dynamic_pointer_cast<WaitInputPlayer>(pipeline.front()); // Lay action hien tai cua BattleScene
	if (!curAction)
		return false;

	shared_ptr<MGame::PlayerAction> playerMove;

	if (curAction->getType() == BSAction::ActionType::StartSetupActive && place == PlaceType::Active) {
		playerMove = make_shared<MGame::PA_SetupActive>(playerId, handIdx);
	}
	else if (curAction->getType() == BSAction::ActionType::StartSetupBench && place == PlaceType::Bench)
	{
		playerMove = make_shared<MGame::PA_SetupBench>(playerId, handIdx);
	}
	else if (curAction->getType() == BSAction::ActionType::ChooseTurnAction) {
		if(place == PlaceType::Bench)
			playerMove = make_shared<MGame::PA_PlayPetCardToBench>(playerId, handIdx);
	}

	if (!playerMove)
		return false;

	auto bm = MGame::BattleMaster::get();
	return curAction->onReceivePlayerInput(bm, playerMove);
}

bool BattleScene::onPlayerEndTurn(const PlayerIdType &playerId) {
	if (pipeline.empty())
		return false;

	auto curAction = dynamic_pointer_cast<WaitInputPlayer>(pipeline.front()); // Lay action hien tai cua BattleScene
	if (!curAction)
		return false;

	shared_ptr<MGame::PlayerAction> playerMove = make_shared<MGame::PA_EndTurn>(playerId);

	auto bm = MGame::BattleMaster::get();
	return curAction->onReceivePlayerInput(bm, playerMove);
}

bool BattleScene::onPlayerDoAction(const shared_ptr<MGame::PlayerAction> &pAction) {
	auto curAction = dynamic_pointer_cast<WaitInputPlayer>(pipeline.front()); // Lay action hien tai cua BattleScene
	if (!curAction)
		return false;
	auto bm = MGame::BattleMaster::get();
	return curAction->onReceivePlayerInput(bm, pAction);
}


void BattleScene::pushAction(const ActionPtr &ptr) {
	pipeline.push_back(ptr);
}

void BattleScene::pushActions(initializer_list<ActionPtr> list) {
	for (const auto &action : list)
		pipeline.push_back(action);
}

void BattleScene::popFront() {
	pipeline.pop_front();
}

void BattleScene::clearPipeline() {
	pipeline.clear();
}

void BattleScene::showCardDetail(const shared_ptr<const CardData> &data) {
	if (data->type == CardData::Type::Pet) {
		showPetCardDetail(dynamic_pointer_cast<const PetCardData>(data));
	}
	else if (data->type == CardData::Type::Energy) {
		showEnergyCardDetail(dynamic_pointer_cast<const EnergyCardData>(data));
	}
}

void BattleScene::showPetCardDetail(const shared_ptr<const PetCardData> &data){
	if (!detailedPetCard) {
		detailedPetCard = bsres->getBSCard(data->id);
		detailLayout->addChild(detailedPetCard);

		auto dir = Director::getInstance();
		auto vSize = dir->getVisibleSize();
		detailedPetCard->setPosition(Vec2(vSize.width / 2, vSize.height / 2 + 20));
	}
	else {
		auto dtmgr = GM_DATA_MGR;
		auto cardData = dtmgr->getCardData(data->id);
		detailedPetCard->initWithData(cardData);
	}

	detailedPetCard->setVisible(true);
	detailedPetCard->setFlip(false);
	detailedPetCard->setNormalSize();
}
void BattleScene::showEnergyCardDetail(const shared_ptr<const EnergyCardData> &data){
	if (!detailedEnergyCard) {
		detailedEnergyCard = bsres->getBSCard(data->id);
		detailLayout->addChild(detailedEnergyCard);
		
		auto dir = Director::getInstance();
		auto vSize = dir->getVisibleSize();
		detailedEnergyCard->setPosition(Vec2(vSize.width / 2, vSize.height / 2 + 20));
	}
	else {
		auto dtmgr = GM_DATA_MGR;
		auto cardData = dtmgr->getCardData(data->id);
		detailedEnergyCard->initWithData(cardData);
	}

	detailedEnergyCard->setFlip(false);
	detailedEnergyCard->setOpacity(0);
	detailedEnergyCard->setNormalSize();
	
}
void BattleScene::showItemCardDetail(const shared_ptr<const CardData> &data){}
void BattleScene::showSotCardDetail(const shared_ptr<const CardData> &data){}

void BattleScene::hideCardDetail() {
	if (detailedPetCard)
		detailedPetCard->setVisible(false);
	if (detailedEnergyCard)
		detailedEnergyCard->setVisible(false);
}

void BattleScene::setEnableEndTurnButton(bool e) {
	endTurnBtn->setEnabled(e);
	endTurnBtn->setVisible(e);
}

void BattleScene::setClickEndButton(const EndClickFunc &f) {
	endTurnBtn->addClickEventListener(f);
}


void BattleScene::enablePlayerChooseTurnAction(const PlayerIdType &id) {
	auto hand = this->getHand(pid);
	
	hand->enabledDragForTurnAction();
	this->setEnableEndTurnButton(true);
	this->setClickEndButton([this](Ref*) {
		bool suc = onPlayerEndTurn(pid);
		if (suc)
			setEnableEndTurnButton(false);
		});
}

void BattleScene::showHolderInfo(CardHolder *holder) {
	auto ownerId = holder->getOwnerId();
	auto player = getBSPlayer(ownerId);
	auto canUseMove = ownerId == PLAYER && !player->actionExceedsLimit(BSPlayer::TurnAction::Attack);
	auto canRetreat = ownerId == PLAYER && !player->actionExceedsLimit(BSPlayer::TurnAction::Retreat);
	moveSelector->showInfoHolder(holder, canUseMove, canRetreat);
}




/////////

void BattleScene::func1(Ref *sender) {
	moveSelector->showInfoHolder(nullptr);
}

void BattleScene::func2(Ref *sender) {
	moveSelector->hide();
}

void BattleScene::onTurnStart(const PlayerIdType &id) {
	phase = Phase::PlayerTurn;

	if (updateTurnCountAfterSetup) {
		turnCount = 0;
		updateTurnCountAfterSetup = false;
	}
	else
		turnCount += 1;
	curPlayerId = id;
	players.at(id)->resetDoneCount();
}

void BattleScene::onEndSetup() {
	turnCount = 0;
	updateTurnCountAfterSetup = true;
}

void BattleScene::onEndGame(const PlayerIdType &winnerId) {
	phase = Phase::End;
	stopPipeline();
	showEndLayout(winnerId);
}

void BattleScene::showEndLayout(const PlayerIdType &winnerId) {
	endLayout->setVisible(true);
	
	auto lang = GM_LANG;
	static const string OPP_WIN_TXT = "TXT_BS_OPPONENT_WIN";
	static const string PLAYER_WIN_TXT = "TXT_BS_PLAYER_WIN";
	static const string DRAW_TXT = "TXT_BS_DRAW_GAME";
	string resultText;

	if (winnerId.empty()) {
		resultText = lang->getString(DRAW_TXT);
	}
	else {
		if (winnerId == PLAYER) {
			resultText = lang->getString(PLAYER_WIN_TXT);
		}
		else {
			resultText = lang->getString(OPP_WIN_TXT);
		}
	}
	auto winText = dynamic_cast<ui::Text*>(endLayout->getChildByName("End_Text"));
	winText->setString(resultText);
	winText->stopAllActions();
	auto middlePos = winText->getPosition();
	auto dest = middlePos;
	winText->setPosition(middlePos + Vec2(0, 400));
	winText->runAction(EaseBackOut::create(MoveTo::create(0.6f, dest)));

	auto ctnBtn = dynamic_cast<ui::Button*>(endLayout->getChildByName("ContinueBtn"));
	ctnBtn->addClickEventListener([this](Ref *sender) {
		continueGame();
		});

	auto homeBtn = dynamic_cast<ui::Button*>(endLayout->getChildByName("HomeBtn"));
	ctnBtn->addClickEventListener([this](Ref *sender) {
		backHome();
		});
}

void BattleScene::continueGame() {
	auto gm = GameManager::getInstance();
	gm->changeSceneFade(BattleSceneNS::BattleScene::create());
}

void BattleScene::backHome() {
	auto gm = GameManager::getInstance();
	gm->playTitleSceneFade();
}

BATTLE_SCENE_NS_END
