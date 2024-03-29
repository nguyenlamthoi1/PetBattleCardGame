#include "BSHand.h"
#include "BSCard.h"
#include "BSDeck.h"
#include "BSBoard.h"
#include "BSPlayer.h"
#include "CardHolder.h"
#include "BSNotifier.h"
#include "data/CardData.h"

#include "BattleScene.h"
#include "GameManager.h"
#include "common/ResourcePool.h"
#include "common/Utilize.h"
#include "components/WidgetTouchComponent.h"
#include "common/Utilize.h"
#include "ui/UIHelper.h"

#include "game/player_actions/PlayerAction.h"

using namespace std;
USING_NS_CC;
BATTLE_SCENE_NS_BEG

const float BSHand::CardScale = 0.25f;

BSHand::HandPtr BSHand::createShPtr(ui::Layout *layout, const PlayerIdType &id) {
	auto ret = make_shared<BSHand>(layout, id);
	if (ret && ret->init())
		return ret;
	return nullptr;
}

BSHand::BSHand(cocos2d::ui::Layout *layout, PlayerIdType id) : handLayout(layout), pid(id){
	CCLOG("BSHand::Ctor %p", this);
}

BSHand::~BSHand() {
	CCLOG("BSHand::Dtor %p", this);
}

bool BSHand::init() {
	if (!handLayout)
		return false;

	auto dir = Director::getInstance();
	auto visibleSize = dir->getVisibleSize();

	constexpr float CARD_HEIGHT = 142.0f;
	handLayout->setContentSize(Size(visibleSize.width, CARD_HEIGHT));
	handLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	handLayout->setBackGroundColor(Color3B(150, 200, 255));
	handLayout->setBackGroundColorOpacity(100);
	handLayout->setAnchorPoint(Vec2(0, 0));
	return true;
}

void BSHand::drawCards(const vector<CardId> &idVec, bool hideCards) {
	auto btlScn = BattleScene::getScn();

	if (idVec.empty()) {
		auto lang = GM_LANG;
		auto notifier = btlScn->getNotifier();
		notifier->showMsgAndHideAfter(lang->getString(pid == PLAYER ? "TXT_BS_PLAYER_CAN_NOT_DRAW" : "TXT_BS_OPP_CAN_NOT_DRAW"), 1.6f);
		handLayout->runAction(Sequence::create(
			DelayTime::create(1.0f),
			CallFunc::create([this](){
				dispatchEvent(EV_DRAW_ACTION_DONE);
				}),
			nullptr));
		return;
	}
	
	auto &deck = btlScn->getDeck(pid);
	auto drawnVec = deck->drawTop(idVec);

	size_t total = cards.size() + drawnVec.size(); // Tong so card sau khi draw
	auto handSize = handLayout->getContentSize();

	/// *Anchor Point cua card: (0.5, 0.5)

	auto cardW = BSCard::CARD_SIZE.width;
	float cardSpace = 5.0f; // Khoang cach giua nhung la bai
	const float minCardSpace = -cardW / 2; // cardSpace >= -cardW / 2 (Neu cardSpace < 0 thi 2 card se dinh vao nhau)
	auto totalW = total * cardW + (total - 1) * cardSpace; // Tong width cua tat ca card tren hand

	if (totalW > handSize.width)  // Neu vuot qua hand width thi giam cardSpace
	{
		totalW = handSize.width;
		cardSpace = (totalW - total * cardW) / (total - 1);
	}

	auto x = (handSize.width - totalW) / 2.0 + cardW / 2.0;
	decltype(x) y = handSize.height / 2;

	// * Sap xep nhung card da co tren tay truoc
	size_t i = 0;
	double d = cardW + cardSpace;
	for (auto &card : cards) { // * Nhung card nay da duoc addChild
		auto action = MoveTo::create(0.5f, Vec2(x + d * i, y));
		card->runAction(action);
		++i;
	}

	// * Duyet vong lap tren danh sach card duoc draw
	auto startPos = Vec2(handSize.width + cardW + 30, handSize.height);
	for (size_t index = 0; index < drawnVec.size(); ++index) { // * Nhung card nay chua duoc addChild
		auto card = drawnVec[index];
		handLayout->addChild(card);
		card->setPosition(startPos);
		card->setFlip(hideCards);
		card->setScale(CardScale);

		// Them hold touch
		WidgetTouchNS::setWidgetTouchHold(card, [card, this, btlScn](WIDGET_TOUCH_HANDLER_PARAMS) {
			auto data = card->getData();
			btlScn->showCardDetail(data);
			}, 0.3f);

		// Them end Touch
		WidgetTouchNS::setWidgetTouchEnded(card, [card, this, btlScn](WIDGET_TOUCH_HANDLER_PARAMS) {
			btlScn->hideCardDetail();
			});

		card->setTouchEnabled(true);
		card->setDragEnabled(false);
		card->setSwallowTouches(true);

		if (index == drawnVec.size() - 1) {
			auto action = Sequence::create(MoveTo::create(0.8f + index * 0.2f, Vec2(x + d * i, y)), CallFunc::create([this]() {
				dispatchEvent(EV_DRAW_ACTION_DONE);
				}), nullptr);
			card->runAction(action);
		}
		else {
			auto action = MoveTo::create(0.8f + index * 0.2f, Vec2(x + d * i, y));
			card->runAction(action);
		}

		++i;
	}
	cards.insert(cards.cend(), drawnVec.cbegin(), drawnVec.cend());

}

BSCard * BSHand::getCardAt(unsigned int idx) const {
	if (idx >= cards.size())
		return nullptr;
	return cards[idx];
}

void BSHand::putToHand(const vector<BSCard*> &drawnVec, DrawFromType fromPlace, bool hideCards) {
	auto btlScn = BattleScene::getScn();

	size_t total = cards.size() + drawnVec.size(); // Tong so card sau khi draw
	auto handSize = handLayout->getContentSize();

	/// *Anchor Point cua card: (0.5, 0.5)

	auto cardW = BSCard::CARD_SIZE.width;
	float cardSpace = 5.0f; // Khoang cach giua nhung la bai
	const float minCardSpace = -cardW / 2; // cardSpace >= -cardW / 2 (Neu cardSpace < 0 thi 2 card se dinh vao nhau)
	auto totalW = total * cardW + (total - 1) * cardSpace; // Tong width cua tat ca card tren hand

	if (totalW > handSize.width)  // Neu vuot qua hand width thi giam cardSpace
	{
		totalW = handSize.width;
		cardSpace = (totalW - total * cardW) / (total - 1);
	}

	auto x = (handSize.width - totalW) / 2.0 + cardW / 2.0;
	decltype(x) y = handSize.height / 2;

	// * Sap xep nhung card da co tren tay truoc
	size_t i = 0;
	double d = cardW + cardSpace;
	for (auto &card : cards) { // * Nhung card nay da duoc addChild
		auto action = MoveTo::create(0.5f, Vec2(x + d * i, y));
		card->runAction(action);
		++i;
	}

	// * Duyet vong lap tren danh sach card duoc draw
	auto startPos = Vec2(handSize.width + cardW + 30, handSize.height);
	switch (fromPlace) {
	case DrawFromType::Prize:
		auto fromNode = btlScn->getPrizePileNode(pid);
		startPos = Utilize::mnode::getLocalPos(fromNode, handLayout);
		break;
	}

	for (size_t index = 0; index < drawnVec.size(); ++index) { // * Nhung card nay chua duoc addChild
		auto card = drawnVec[index];
		handLayout->addChild(card);

		card->setPosition(startPos); // *Giu vi tri cu
		card->setFlip(hideCards);
		card->setScale(CardScale);

		// Them hold touch
		WidgetTouchNS::setWidgetTouchHold(card, [card, this, btlScn](WIDGET_TOUCH_HANDLER_PARAMS) {
			auto data = card->getData();
			btlScn->showCardDetail(data);
			}, 0.3f);

		// Them end Touch
		WidgetTouchNS::setWidgetTouchEnded(card, [card, this, btlScn](WIDGET_TOUCH_HANDLER_PARAMS) {
			btlScn->hideCardDetail();
			});

		card->setTouchEnabled(true);
		card->setDragEnabled(false);
		card->setSwallowTouches(true);

		if (index == drawnVec.size() - 1) {
			auto action = Sequence::create(MoveTo::create(0.8f + index * 0.2f, Vec2(x + d * i, y)), CallFunc::create([this]() {
				dispatchEvent(EV_PUT_TO_HAND);
				}), nullptr);
			card->runAction(action);
		}
		else {
			auto action = MoveTo::create(0.8f + index * 0.2f, Vec2(x + d * i, y));
			card->runAction(action);
		}

		++i;
	}
	cards.insert(cards.cend(), drawnVec.cbegin(), drawnVec.cend());

}


const string BSHand::EV_DRAW_ACTION_DONE = "EV_DRAW_ACTION_DONE";
const string BSHand::EV_PUT_TO_HAND = "EV_PUT_TO_HAND";


void BSHand::onDragBack(BSCard *cardNode) {
	auto comp = MyComponentNS::getComponent<DragComponent>(cardNode, COMPONENT_KEY::DRAG);
	if (comp) {
		auto cardWidget = dynamic_cast<ui::Widget*>(cardNode);
		auto wOldPos = comp->getOrgWorldPos();
		auto orgParent = comp->getOrgParent();
		cardWidget->removeFromParent();
		handLayout->addChild(cardWidget);
		auto lOldPos = handLayout->convertToNodeSpaceAR(wOldPos);
		cardWidget->setZOrder(comp->getOrgZ());
		cardWidget->setTouchEnabled(false);
		cardWidget->runAction(
			Sequence::create(MoveTo::create(0.2f, lOldPos),
				CallFunc::create([cardWidget]() {cardWidget->setTouchEnabled(true); }),
				nullptr));
	}
}

bool BSHand::onDragEnd(BSCard *card, cocos2d::Node *dest) {
	return false;
	auto holder = dynamic_cast<CardHolder*>(dest);
	auto data = card->getData();
	bool suc = false;
	auto btlScn = BattleScene::getScene();
	switch (data->type) {
	case CardData::Type::Pet: 
	{
		auto petCard = dynamic_cast<PetCard*>(card);
		//suc = btlScn->playerTryPlayPetCard(ownerId, petCard, holder);
	}
		break;
	case CardData::Type::Energy:
	{
		auto energyCard = dynamic_cast<EnergyCard*>(card);
		//bool suc = holder->tryAddEnergyCard(energyCard);
		//suc = btlScn->getBattleManager()->playerTryPlayEnergyCard(ownerId, energyCard, holder);
	}
		break;
	}

	if (suc) {
		Utilize::mvector::removeElement(cards, card);
		updateCardPositions();
	}

	//return suc;
	return true;
}
//
void BSHand::updateCardPositions() {
	size_t total = cards.size(); // Tong so card hien tai
	auto handSize = handLayout->getContentSize();

	// *Anchor Point cua card: (0.5, 0.5)

	auto cardW = BSCard::CARD_SIZE.width;
	float cardSpace = 5.0f; // Khoang cach giua nhung la bai
	const float minCardSpace = -cardW / 2; // cardSpace >= -cardW / 2 (Neu cardSpace < 0 thi 2 card se dinh vao nhau)
	auto totalW = total * cardW + (total - 1) * cardSpace; // Tong width cua tat ca card tren hand

	if (totalW > handSize.width)  // Neu vuot qua hand width thi giam cardSpace
	{
		totalW = handSize.width;
		cardSpace = (totalW - total * cardW) / (total - 1);
	}

	auto x = (handSize.width - totalW) / 2.0 + cardW / 2.0;
	decltype(x) y = handSize.height / 2;

	size_t i = 0;
	double d = cardW + cardSpace;
	for (auto &card : cards) { // * Nhung card nay da duoc addChild
		auto action = MoveTo::create(0.3f, Vec2(x + d * i, y));
		card->runAction(action);
		++i;
	}
}

void BSHand::enabledDragForTurnAction() {
	auto btlScn = BattleScene::getScene();

	unsigned int handIdx = 0;
	for (auto &card : cards) {
		if (card->getType() == BSCard::Type::Pet)
			setDragForPetCard(dynamic_cast<PetCard*>(card), handIdx);
		else if(card->getType() == BSCard::Type::Energy)
			setDragForEnergyCard(dynamic_cast<EnergyCard*>(card), handIdx);
		++handIdx;
	}
}

void BSHand::setEnableDragSetupActive(bool enabled) {
	auto btlScn = BattleScene::getScene();

	unsigned int handIdx = 0;
	for (auto &card : cards) {
		if (card->getType() == BSCard::Type::Pet)
			setDragForPetCard(dynamic_cast<PetCard*>(card), handIdx);
		else
			card->setDragEnabled(false);
		++handIdx;
	}
}



void BSHand::setDragForCard(BSCard* card, unsigned int handIdx) {
	if (card->getType() == BSCard::Type::Pet)
		setDragForPetCard(dynamic_cast<PetCard*>(card), handIdx);
	else if(card->getType() == BSCard::Type::Energy)
		setDragForEnergyCard(dynamic_cast<EnergyCard*>(card), handIdx);
}


void BSHand::setDragForPetCard(PetCard *petCard, unsigned int handIdx) {
	if (!petCard)
		return;

	auto pData = petCard->getPetData();
	auto isBasic = pData->isBasicCard();

	auto btlScn = BattleScene::getScene();
	auto board = btlScn->getBoard(pid);
	auto player = btlScn->getBSPlayer(pid);
	if (isBasic) {
		// Dich den cua Basic Card la Active hoac Bench
		vector<Node*> dests;
		dests.push_back(board->getActiveBoard());
		dests.push_back(board->getBenchBoard());
		if (!player->actionExceedsLimit(BSPlayer::TurnAction::PlayPetCard))
			petCard->setDragHandler(dests,
				[this, btlScn](Node * cardNode, Node *dest) { // Drag Begin Callback
					btlScn->hideCardDetail();
				},
				[this, board, btlScn, handIdx](Node *cardNode, Node *dest) { // Drag End Callback
					auto petCard = dynamic_cast<PetCard*>(cardNode);
					if (!dest || !petCard) {
						auto comp = DragComponent::getComp(cardNode);
						onDragBack(petCard);
					}
					else {
						bool dragOnActive = board->isActiveBoard(dest);
						bool check = board->checkCanAddBasicPetCard(petCard, dest) &&
							btlScn->onPlayerPetCard(pid, handIdx, dragOnActive ? BattleScene::PlaceType::Active : BattleScene::PlaceType::Bench);
						if (!check)
							onDragBack(petCard);
					}
				});
		/*else
			petCard->setDragEnabled(false);*/
	}
	else {
		// Dich den cua Basic Card la Active hoac Bench
		vector<Node*> dests = board->getAllHolders();
		if (!player->actionExceedsLimit(BSPlayer::TurnAction::EvolvePet))
			petCard->setDragHandler(dests,
				[this, btlScn](Node * cardNode, Node *dest) { // Drag Begin Callback
					btlScn->hideCardDetail();
				},
				[this, board, btlScn, handIdx](Node *cardNode, Node *dest) { // Drag End Callback
					auto petCard = dynamic_cast<PetCard*>(cardNode);
					auto holder = dynamic_cast<CardHolder*>(dest);
					if (!holder || !petCard) {
						auto comp = DragComponent::getComp(cardNode);
						onDragBack(petCard);
					}
					else {
						bool isActiveHolder = holder->isType(CardHolder::HolderType::Active);
						bool check = holder->canEvolveTo(petCard); // Kiem tra ben phia Scene
						if (check) { 
							auto benchIdx = holder->getHolderIdx();
							auto pAction = make_shared<MGame::PA_EvPetCard>(pid, handIdx, isActiveHolder, benchIdx);
							check = btlScn->onPlayerDoAction(pAction); // Kiem tra ben phia GameState, Neu OK thi lam
						}
						if (!check)
							onDragBack(petCard);
					}
				});
		/*else
			petCard->setDragEnabled(false);*/
	}
}

void BSHand::setDragForEnergyCard(EnergyCard *energyCard, unsigned int handIdx) {
	if (!energyCard)
		return;

	auto btlScn = BattleScene::getScene();
	auto board = btlScn->getBoard(pid);

	auto player = btlScn->getBSPlayer(pid);
	vector<Node*> dests;

	if (player->actionExceedsLimit(BSPlayer::TurnAction::AttachEnergy)) // Khong the attach
	{
		//energyCard->setDragEnabled(false);
		return;
	}
	
	dests = board->getAllHolders();
	// Them Drag component
	energyCard->setDragHandler(dests,
		[this, btlScn](Node * cardNode, Node *dest) { // Drag Begin Callback
			btlScn->hideCardDetail();
		},
		[this, board, btlScn, handIdx](Node *cardNode, Node *dest) { // Drag End Callback
			auto eCard = dynamic_cast<EnergyCard*>(cardNode);
			auto holder = dynamic_cast<CardHolder*>(dest);
			if (!holder) {
				auto comp = DragComponent::getComp(cardNode);
				onDragBack(eCard);
			}
			else {
				bool isActiveHolder = holder->isType(CardHolder::HolderType::Active);
				bool check = holder->hasPetCard();
				
				if (check) {
					auto placeType = isActiveHolder ? MGame::PA_AttachEnergy::PlaceType::Active : MGame::PA_AttachEnergy::PlaceType::Bench;
					auto benchIdx = holder->getHolderIdx();
					auto pAction = make_shared<MGame::PA_AttachEnergy>(pid, handIdx, placeType, benchIdx);
					check = btlScn->onPlayerDoAction(pAction);
				}

				if (!check)
					onDragBack(eCard);
			}
		}
		);
}

void BSHand::setDragForSupporterCard() {

}

void BSHand::disableDragAll() {
	for (auto &card : cards) {
		card->setDragEnabled(false);
	}
}


bool BSHand::playPetCardFromHandToActive(unsigned int handIdx, const function<void()> &onDone) {
	auto card = cards.at(handIdx);
	
	bool suc = false;
	if (card->getType() == BSCard::Type::Pet) {
		auto btlScn = BattleScene::getScn();
		auto board = btlScn->getBoard(pid);
		auto petCard = dynamic_cast<PetCard*>(card);

		suc = board->addPetOnActive(petCard, onDone);
		if (suc) { // Add thanh cong petCard
			removeCardAt(handIdx);
			updateCardPositions();
		}

	}
	return suc;
}

bool BSHand::playPetCardFromHandToBench(unsigned int handIdx, const function<void()> &onDone) {
	auto card = cards.at(handIdx);

	bool suc = false;
	if (card->getType() == BSCard::Type::Pet) {
		auto btlScn = BattleScene::getScn();
		auto board = btlScn->getBoard(pid);
		auto petCard = dynamic_cast<PetCard*>(card);
		suc = board->addPetOnBoard(petCard, onDone);
		if (suc) { // Add thanh cong petCard
			removeCardAt(handIdx);
			updateCardPositions();
		}
	}
	return suc;
}

bool BSHand::playEnergyCardFromHandToPet(unsigned int handIdx, bool isActive, unsigned int benchIdx, const function<void()> &onDone) {
	auto card = cards.at(handIdx);

	bool suc = false;
	if (card->getType() == BSCard::Type::Energy) {
		auto btlScn = BattleScene::getScn();
		auto board = btlScn->getBoard(pid);
		auto holder = isActive ? board->getActiveHolder() : board->getBenchHolder(benchIdx);
		auto eCard = dynamic_cast<EnergyCard*>(card);
		suc = holder->tryAddEnergyCard(eCard, onDone);
		if (suc) { // Add thanh cong petCard
			removeCardAt(handIdx);
			updateCardPositions();
		}
	}
	return suc;
}

bool BSHand::playEvCardFromHandToPet(unsigned int handIdx, bool isActive, unsigned int benchIdx, const function<void()> &onDone) {
	auto card = cards.at(handIdx);

	bool suc = false;
	if (card->getType() == BSCard::Type::Pet) {
		auto btlScn = BattleScene::getScn();
		auto board = btlScn->getBoard(pid);
		auto holder = isActive ? board->getActiveHolder() : board->getBenchHolder(benchIdx);
		auto pCard = dynamic_cast<PetCard*>(card);
		suc = holder->tryEvolveTo(pCard, onDone);
		if (suc) { // Add thanh cong petCard
			removeCardAt(handIdx);
			updateCardPositions();
		}
	}
	return suc;
}

void BSHand::removeCardAt(unsigned int handIdx) {
	if (!checkIdxValid(handIdx))
		return;

	cards.erase(cards.cbegin() + handIdx);
}



BATTLE_SCENE_NS_END