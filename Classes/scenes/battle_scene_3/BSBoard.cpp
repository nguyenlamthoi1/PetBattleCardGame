#include "BSBoard.h"
#include "CardHolder.h"
#include "BattleScene.h"
#include "BSCard.h"
#include "GameManager.h"
#include "BSNotifier.h"

#include "scenes/battle_scene_3/game/player_actions/PlayerAction.h"

#include "common/Utilize.h"
#include <new>

USING_NS_CC;
UTILIZE_USE_NS;
using namespace std;

BATTLE_SCENE_NS_BEG

BSBoard* BSBoard::create(BattleScene *scn, PlayerIdType id) {
	auto board = new (nothrow) BSBoard(scn, id);

	if (board && board->init()) 
		return board;
	
	delete board;
	board = nullptr;
	return board;
}

BSBoard::BSBoard(BattleScene *scn, PlayerIdType id) : btlScn(scn), ownerId(id){}

BSBoard::~BSBoard() {}

bool BSBoard::init() {
	if (!btlScn)
		return false;
	auto ui = btlScn->getScnRoot();
	string prefix = ownerId == PLAYER ? "P1" : "P2";
	pPanel = dynamic_cast<ui::Layout*>(ui->getChildByName(prefix + "_Panel")); // P1_Panel or P2_Panel
	if (!pPanel)
		return false;

	doneBtn = ownerId == PLAYER ? dynamic_cast<ui::Button*>(pPanel->getChildByName("BoardBtn")) : nullptr;


	activeBoard = dynamic_cast<ui::Layout*>(pPanel->getChildByName("Active_Board"));
	// Them holder vao active board
	activeHolder = CardHolder::createActive(btlScn, ownerId);
	activeBoard->addChild(activeHolder);
	activeHolder->setPosition(Vec2(6, 6));

	benchBoard = dynamic_cast<ui::Layout*>(pPanel->getChildByName("Bench_Board"));
	benchContainer = dynamic_cast<ui::Layout*>(benchBoard->getChildByName("Container"));

	auto benchSize = benchContainer->getContentSize();
	auto cardH = benchSize.height - 12;
	
	unsigned int idx = 0;
	while (benchHolders.size() < maxBenchCapacity) {
		auto holder = CardHolder::createBench(btlScn, ownerId, idx);
		holder->setHolderSizeH(cardH);
		holder->setName("BenchHolder_" + to_string(benchHolders.size()));
		benchContainer->addChild(holder);
		benchHolders.push_back(holder);
		++idx;
	}
	alignHoldersOnBenchBoard(true); // Can chinh vi tri cac holder trong bench
	
	curBenchNum = 0;

	return true;
}

void BSBoard::alignHoldersOnBenchBoard(bool forceDoLayout) {
	auto layoutType = benchContainer->getLayoutType();
	auto &layout = benchContainer;
	if (layoutType != cocos2d::ui::Layout::Type::HORIZONTAL)
		layout->setLayoutType(cocos2d::ui::Layout::Type::HORIZONTAL);

	auto children = layout->getChildren();
	auto spaceX = 10;
	auto offsetPos = Vec2(6, 6);
	auto benchSize = benchBoard->getContentSize();
	for (auto& child : children)
	{
		cocos2d::ui::Widget* widget = dynamic_cast<cocos2d::ui::Widget*>(child);
		auto orgSize = widget->getContentSize();
		auto spaceLeft = (benchSize.width - (children.size() * orgSize.width)) / (children.size() + 1);

		float diffW = fabs(1.0 * orgSize.width * (1 - holderScale));
		if (widget)
		{
			auto layoutParameter = ui::LinearLayoutParameter::create();
			layoutParameter->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
			layoutParameter->setMargin(cocos2d::ui::Margin(spaceLeft, 0, 0, 0));
			widget->setLayoutComponentEnabled(true);
			widget->setLayoutParameter(layoutParameter);
		}
	}
	if (forceDoLayout)
		layout->forceDoLayout();
	else
		layout->requestDoLayout();
}

bool BSBoard::checkCanAddBasicPetCard(PetCard *card, cocos2d::Node* dest) const {
	if (!card || !card->isBasic())
		return false;
	return (activeBoard == dest && !hasActivePet()) || (benchBoard == dest && !isBenchFull());
}


bool BSBoard::hasActivePet() const {
	return activeHolder->hasPetCard();
}

bool BSBoard::isBenchFull() const {
	return curBenchNum >= maxBenchCapacity;
}

bool BSBoard::hasPetOnBoard() const {
	for (const auto &holder : benchHolders) {
		if (holder->hasPetCard())
			return true;
	}
	return false;
}


/*
	Getters
*/

const vector<Node*> BSBoard::getAllHolders() const {
	vector<Node*> ret;
	ret.push_back(activeHolder);
	ret.insert(ret.cend(), benchHolders.begin(), benchHolders.end());
	return ret;
}

const vector<CardHolder*>& BSBoard::getBenchHolders() const {
	return benchHolders;
}

CardHolder* BSBoard::getActiveHolder() const {
	return activeHolder;
}

CardHolder* BSBoard::getBenchHolder(unsigned int benchIdx) const {
	if (benchIdx >= benchHolders.size())
		return nullptr;

	return benchHolders.at(benchIdx);
}

const string BSBoard::ACTIVE_BOARD_NAME = "Active_Board";
const string BSBoard::BENCH_BOARD_NAME = "Bench_Board";

bool BSBoard::addPetOnActive(PetCard *card, const function<void()> &onDone) {
	return activeHolder->tryAddBasicPetCard(card, onDone);
}
bool BSBoard::addPetOnBoard(PetCard *card, const function<void()> &onDone) {
	for (const auto holder : benchHolders) {
		if (!holder->hasPetCard()) {
			return holder->tryAddBasicPetCard(card, onDone);
		}
	}
	return false;
}

void BSBoard::startSelectOneOfBench(const std::function<void(CardHolder*)> &onDone) {
	unsigned int holderIdx = 0;
	bool canStartSelect = false;
	for (auto &holder : benchHolders) {
		if (holder->hasPetCard()) {
			canStartSelect = true;
			holder->setSelectable(true);
			holder->setOnSelectCallback([this](CardHolder *h) {
				for (auto &holder : benchHolders) {
					if (holder == h) {
						holder->setSelected(true);
					}
					else {
						holder->setSelected(false);
					}
				}
				doneBtn->setVisible(true);
			});
		}
		else {
			holder->setSelectable(false);
			holder->setOnSelectCallback(nullptr);
		}
		holder->setSelected(false);
		
		++holderIdx;
	}
	if (canStartSelect) {
		auto lang = GM_LANG;
		auto notifier = btlScn->getNotifier();
		notifier->showMsg(StringUtils::format("Please choose 1 Pokemon on Bench"));
		btlScn->setEnableEndTurnButton(false);
		doneBtn->setVisible(false);
		doneBtn->addClickEventListener([this, onDone, notifier](Ref *sender) {
			unsigned int selectedNum = 0;
			CardHolder *found = nullptr;
			for (auto &holder : benchHolders) {
				if (holder->checkSelected()) {
					found = holder;
					break;
				}
			}
			if (found) {
				for (auto &holder : benchHolders) 
					holder->setSelectable(false);

				if (onDone)
					onDone(found);
				notifier->hideMsg();
				doneBtn->setVisible(false);
				doneBtn->addClickEventListener(nullptr);
			}
			});
	}
}


BATTLE_SCENE_NS_END