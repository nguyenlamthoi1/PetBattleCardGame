#include "BSBoard.h"
#include "CardHolder.h"
#include "BattleScene.h"

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

BSBoard::BSBoard(BattleScene *scn, PlayerIdType id) : btlScn(scn), ownerId(id){

}

BSBoard::~BSBoard() {
	CCLOG("BSBoard::dtor %d", ownerId);
}

bool BSBoard::init() {
	if (!btlScn)
		return false;
	auto ui = btlScn->getScnRoot();
	string prefix = ownerId == PLAYER ? "P1" : "P2";
	boardPanel = dynamic_cast<ui::Layout*>(ui->getChildByName(prefix + "_Panel")); // P1_Panel or P2_Panel
	if (!boardPanel)
		return false;

	activeBoard = dynamic_cast<ui::Layout*>(boardPanel->getChildByName("Active_Board"));
	// Them holder vao active board
	activeHolder = CardHolder::createActive(btlScn, ownerId);
	activeBoard->addChild(activeHolder);
	activeHolder->setPosition(Vec2(6, 6));

	benchBoard = dynamic_cast<ui::Layout*>(boardPanel->getChildByName("Bench_Board"));
	auto benchSize = benchBoard->getContentSize();
	auto cardH = benchSize.height - 12;
	
	while (benchHolders.size() < maxBenchCapacity) {
		auto holder = CardHolder::createBench(btlScn, ownerId);
		holder->setHolderSizeH(cardH);
		benchBoard->addChild(holder);
		benchHolders.push_back(holder);
	}
	alignHoldersOnBenchBoard(true); // Can chinh vi tri cac holder trong bench
	
	return true;
}

void BSBoard::alignHoldersOnBenchBoard(bool forceDoLayout) {
	auto layoutType = benchBoard->getLayoutType();
	auto &layout = benchBoard;
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

bool BSBoard::hasActivePet() {
	return activeHolder->hasPetCard();
}

/*
	Getters
*/

const std::vector<CardHolder*> BSBoard::getBenchHolders() const {
	return benchHolders;
}


BATTLE_SCENE_NS_END