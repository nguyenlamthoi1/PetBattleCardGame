#include "BSCardSelector.h"
#include "UnSelectedCardHolder.h"
#include "SelectedCardHolder.h"

#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "common/ResourcePool.h"
#include "common/LangSys.h"
#include "common/DataManager.h"

#include "ui/UIHelper.h"

#include "scenes/battle_scene_3/BattleScene.h"
#include "scenes/battle_scene_3/BSCard.h"
#include "scenes/battle_scene_3/BSResources.h"
#include "scenes/battle_scene_3/game/player_actions/PlayerAction.h"


USING_NS_CC;
//BATTLE_SCENE_USE_NS;
using namespace std;

BATTLE_SCENE_NS_BEG

/*
	BSCardSelector Class
*/

shared_ptr<BSCardSelector> BSCardSelector::create(BattleScene *root) {
	auto ret = make_shared<BSCardSelector>(root);
	if (ret && ret->init())
		return ret;
	return nullptr;
}

BSCardSelector::BSCardSelector(BattleScene *scn) : btlScn(scn) {
}
BSCardSelector::~BSCardSelector() {
	clearUI();
}
const string BSCardSelector::UPDATE_SCHEDULER = "UPDATE_SCHEDULER";
const string BSCardSelector::SELECT_DONE = "SELECT_DONE";

bool BSCardSelector::init() {
	root = btlScn->getScnRoot()->getChildByName("Card_Selector");
	
	if (!root)
		return false;
	
	root->setPosition(Vec2(0, 0));

	unselectedPanel = dynamic_cast<ui::Layout*>(root->getChildByName("Unselected_Panel"));
	unselectedPrevBtn = dynamic_cast<ui::Button*>(unselectedPanel->getChildByName("Unsel_Prev_Button"));
	unselectedNextBtn = dynamic_cast<ui::Button*>(unselectedPanel->getChildByName("Unsel_Next_Button"));
	unselectedListView = dynamic_cast<ui::ListView*>(unselectedPanel->getChildByName("UnSel_ListView"));
	initUnselListView();


	selectedPanel = dynamic_cast<ui::Layout*>(root->getChildByName("Selected_Panel"));
	selectedPrevBtn = dynamic_cast<ui::Button*>(selectedPanel->getChildByName("Prev_Button"));
	selectedNextBtn = dynamic_cast<ui::Button*>(selectedPanel->getChildByName("Next_Button"));
	selectedListView = dynamic_cast<ui::ListView*>(selectedPanel->getChildByName("ListView"));
	initSelListView();

	msgBoard = dynamic_cast<ui::Layout*>(root->getChildByName("Msg_Board"));
	doneBtn = dynamic_cast<ui::Button*>(root->getChildByName("DoneButton"));
	return true;
}

void BSCardSelector::update(float t) {
	if (!root->isVisible())
		return;
	if (unselectedListView && !unSelHolderVec.empty()) {
		auto last = unselectedListView->getRightmostItemInCurrentView();
		auto first = unselectedListView->getLeftmostItemInCurrentView();
		if (last == unSelHolderVec[unSelHolderVec.size() - 1]) {
			unselectedNextBtn->setVisible(false);
		}
		else
			unselectedNextBtn->setVisible(true);

		if (first == unSelHolderVec[0]) {
			unselectedPrevBtn->setVisible(false);
		}
		else
			unselectedPrevBtn->setVisible(true);
	}

	if (selectedListView && !selHolderVec.empty()) {
		auto last = selectedListView->getRightmostItemInCurrentView();
		auto first = selectedListView->getLeftmostItemInCurrentView();
		if (last == selHolderVec[selHolderVec.size() - 1]) {
			selectedNextBtn->setVisible(false);
		}
		else
			selectedNextBtn->setVisible(true);

		if (first == selHolderVec[0]) {
			selectedPrevBtn->setVisible(false);
		}
		else
			selectedPrevBtn->setVisible(true);
	}
}


bool BSCardSelector::initUnselListView() {
	if (!unselectedListView)
		return false;
	auto &listView = unselectedListView;

	auto vSize = Director::getInstance()->getVisibleSize();
	constexpr float Margin = 20.0f;
	constexpr unsigned int MaxPerRow = 4;
	constexpr float ItemWidth = 195;
	auto listViewSize = listView->getContentSize();
	auto newlistViewSize = Size(ItemWidth * MaxPerRow + Margin * (MaxPerRow - 1), listViewSize.height);
	listView->setContentSize(newlistViewSize);
	listView->setClippingEnabled(true);
	listView->setItemsMargin(Margin);
	listView->setPositionX(vSize.width / 2.0);
	listView->setScrollBarEnabled(false);

	auto lvLeftPos = listView->getLeftBoundary();
	auto lvRightPos = listView->getRightBoundary();
	unselectedPrevBtn->setPosition(Vec2(lvLeftPos - 50.0f, listView->getPositionY()));
	unselectedNextBtn->setPosition(Vec2(lvRightPos + 50.0f, listView->getPositionY()));
	//listView->addEventListener([this](Ref *r, ui::ScrollView::EventType type) {
	//	if (type == ui::ScrollView::EventType::SCROLL_TO_RIGHT) {// SCROLL_TO_TOP
	//	  // do what you need to do 
	//		unselectedNextBtn->setVisible(false);
	//	}
	//	else if (type == ui::ScrollView::EventType::SCROLL_TO_LEFT) {
	//		unselectedPrevBtn->setVisible(false);

	//	}
	//	else if(type == ui::ScrollView::EventType::SCROLLING){
	//		unselectedNextBtn->setVisible(true);
	//		unselectedPrevBtn->setVisible(true);
	//	}
	//	});
	return true;
}

bool BSCardSelector::initSelListView() {
	if (!selectedListView)
		return false;
	auto &listView = selectedListView;

	auto vSize = Director::getInstance()->getVisibleSize();
	constexpr float Margin = 20.0f;
	constexpr unsigned int MaxPerRow = 4;
	constexpr float ItemWidth = 195;
	auto listViewSize = listView->getContentSize();
	auto newlistViewSize = Size(ItemWidth * MaxPerRow + Margin * (MaxPerRow - 1), listViewSize.height);
	listView->setContentSize(newlistViewSize);
	listView->setClippingEnabled(true);
	listView->setItemsMargin(Margin);
	listView->setPositionX(vSize.width / 2.0);
	listView->setScrollBarEnabled(false);

	auto lvLeftPos = listView->getLeftBoundary();
	auto lvRightPos = listView->getRightBoundary();
	selectedPrevBtn->setPosition(Vec2(lvLeftPos - 50.0f, listView->getPositionY()));
	selectedNextBtn->setPosition(Vec2(lvRightPos + 50.0f, listView->getPositionY()));

	return true;
}


void BSCardSelector::clearUI() {
	if (unselectedListView)
		unselectedListView->removeAllItems();
	unSelHolderVec.clear();

	if (selectedListView)
		selectedListView->removeAllItems();
	selHolderVec.clear();

	for (const auto &card : cardVec) {
		card->returnToPool();
		card->release();
	}
	cardVec.clear();

	Director::getInstance()->getScheduler()->unschedule(UPDATE_SCHEDULER, this);
}

void BSCardSelector::showToSelect(const vector<CardId> &cidVec, const SelectMap &selMap, bool checkFull) {
	clearUI();
	root->setVisible(true);
	selectFull = checkFull;
	map<CardId, unsigned int> stackedMap;
	for (const auto &cid : cidVec) {
		stackedMap[cid] += 1;
	}
	auto dm = GM_DATA_MGR;
	for (const auto &itr : stackedMap) {
		const auto &cid = itr.first;
		const auto &num = itr.second;

		auto holder = BSUnselectedCardHolder::create();
		unselectedListView->addChild(holder);
		unSelHolderVec.push_back(holder);
		for (unsigned int i = 0; i < num; ++i) {
			auto cardData = dm->getCardData(cid);
			auto card = BSCard::createWithData(cardData);
			card->retain();
			cardVec.push_back(card);
			card->setFlip(false);
			holder->addCard(card);
		}
	}

	unselectedNextBtn->addClickEventListener([this](Ref *sender) {
		unselectedListView->jumpToRight();
		});

	unselectedPrevBtn->addClickEventListener([this](Ref *sender) {
		unselectedListView->jumpToLeft();
		});

	unselectedListView->addEventListener(CC_CALLBACK_2(BSCardSelector::onUnSelItemTouched, this));
	unselectedListView->requestDoLayout();
	//------------//

	for (const auto &itr : selMap) {
		auto selectType = itr.first;
		auto num = itr.second;

		for (unsigned int i = 0; i < num; ++i) {
			auto holder = BSSelectedCardHolder::create();
			holder->setSelType(selectType);
			selectedListView->addChild(holder);
			selHolderVec.push_back(holder);
		}
	}
	selectedPrevBtn->addClickEventListener([this](Ref *sender) {
		selectedListView->jumpToLeft();
		});

	selectedNextBtn->addClickEventListener([this](Ref *sender) {
		selectedListView->jumpToRight();
		});

	selectedListView->addEventListener(CC_CALLBACK_2(BSCardSelector::onSelItemTouched, this));
	selectedListView->requestDoLayout();
	//-------------//
	setMsg(createSelectMsg(selMap));
	
	doneBtn->setEnabled(false);
	doneBtn->addClickEventListener(CC_CALLBACK_1(BSCardSelector::onDoneBtnClicked, this));
	//-------------//
	Director::getInstance()->getScheduler()->schedule(std::bind(&BSCardSelector::update, this, placeholders::_1), this, 0, CC_REPEAT_FOREVER, 0, false, UPDATE_SCHEDULER);

}

void BSCardSelector::hide() {
	clearUI();
	root->setVisible(false);
}

void BSCardSelector::setMsg(const string &msg) {
	auto text = dynamic_cast<ui::Text*>(msgBoard->getChildByName("Msg_Text"));
	text->setString(msg);
	auto textSize = text->getContentSize();
	auto boardSize = msgBoard->getContentSize();

	constexpr float D_WIDTH = 20;
	constexpr float D_HEIGHT = 10;

	float newW = textSize.width + D_WIDTH * 2;
	float newH = textSize.height + D_HEIGHT * 2;
	msgBoard->setContentSize(Size(newW, newH));

	auto layPara = ui::RelativeLayoutParameter::create();
	//layPara->setMargin(ui::Margin(D_WIDTH, D_HEIGHT, D_WIDTH, D_HEIGHT));
	layPara->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
	text->setLayoutParameter(layPara);
	msgBoard->setLayoutType(ui::Layout::Type::RELATIVE);
	msgBoard->requestDoLayout();

	auto boardParent = msgBoard->getParent();
	auto parentSize = boardParent->getContentSize();
	//msgBoard->setPosition(Vec2(parentSize.width / 2.0f, parentSize.height / 2.0f) + Vec2(0, 80));
}

std::string BSCardSelector::createSelectMsg(const SelectMap &selMap) {
	string ret = "Select ";

	auto createStringFrom = [](unsigned int num, GConfig::SelectType type) {
		string ret;
		ret += to_string(num);
		switch (type) {
		case GConfig::SelectType::Pet_Any:
			ret += " Pokemon Card" + (num > 1 ? string("s") : string(""));
			break;
		case GConfig::SelectType::Basic_Energy_Any:
			ret += " Basic Energy Card" + (num > 1 ? string("s") : string(""));
			break;
		case GConfig::SelectType::Basic_Energy_Dark:
			ret += " Basic Dark Energy Card" + (num > 1 ? string("s") : string(""));
			break;
		case GConfig::SelectType::Basic_Energy_Dragon:
			ret += " Basic Dragon Energy Card" + (num > 1 ? string("s") : string(""));
			break;
		case GConfig::SelectType::Basic_Energy_Electric:
			ret += " Basic Electric Energy Card" + (num > 1 ? string("s") : string(""));
			break;
		case GConfig::SelectType::Basic_Energy_Fighting:
			ret += " Basic Fighting Energy Card" + (num > 1 ? string("s") : string(""));
			break;
		case GConfig::SelectType::Basic_Energy_Fire:
			ret += " Basic Fire Energy Card" + (num > 1 ? string("s") : string(""));
			break;
		case GConfig::SelectType::Basic_Energy_Grass:
			ret += " Basic Grass Energy Card" + (num > 1 ? string("s") : string(""));
			break;
		case GConfig::SelectType::Basic_Energy_Normal:
			ret += " Basic Normal Energy Card" + (num > 1 ? string("s") : string(""));
			break;
		case GConfig::SelectType::Basic_Energy_Psychic:
			ret += " Basic Psychic Energy Card" + (num > 1 ? string("s") : string(""));
			break;
		case GConfig::SelectType::Basic_Energy_Steel:
			ret += " Basic Normal Energy Card" + (num > 1 ? string("s") : string(""));
			break;
		case GConfig::SelectType::Basic_Energy_Water:
			ret += " Basic Water Energy Card" + (num > 1 ? string("s") : string(""));
			break;
		default:
			ret += " Card" + (num > 1 ? string("s") : string(""));
		}
		
		return ret;
	};

	for (auto itr = selMap.begin(); itr != selMap.end(); ++itr) {
		auto selectType = itr->first;
		auto num = itr->second;
		auto s = createStringFrom(num, selectType);
		ret += s;
		
		auto last = prev(selMap.end(), 1);
		if (itr != last)
			ret += ", ";
	}
	
	return ret;
}

void BSCardSelector::onSelItemTouched(cocos2d::Ref* sender, cocos2d::ui::ListView::EventType e) {
	auto &listView = selectedListView;
	if (e == ui::ListView::EventType::ON_SELECTED_ITEM_START) {
	}
	else if (e == ui::ListView::EventType::ON_SELECTED_ITEM_END) {
		auto selectedIndex = listView->getCurSelectedIndex();
		auto selWidget = listView->getItem(selectedIndex);
		auto selHolder = dynamic_cast<BSSelectedCardHolder*>(selWidget);
	
		auto fromHolder = selHolder->fromHolder;
		auto removedCard = selHolder->removeCard();
		if (removedCard) {
			bool suc = fromHolder->addCard(removedCard);
			if (!suc) {
				selHolder->addCard(removedCard, fromHolder);
			}

			doneBtn->setEnabled(suc && canDoneSelect());
		}

		
		
	}
}

void BSCardSelector::onUnSelItemTouched(cocos2d::Ref* sender, cocos2d::ui::ListView::EventType e) {
	auto &listView = unselectedListView;
	if (e == ui::ListView::EventType::ON_SELECTED_ITEM_START) {
	}
	else if (e == ui::ListView::EventType::ON_SELECTED_ITEM_END) {
		auto selectedIndex = listView->getCurSelectedIndex();
		auto selWidget = listView->getItem(selectedIndex);
		auto unselHolder = dynamic_cast<BSUnselectedCardHolder*>(selWidget);
		auto removedCard = unselHolder->removeCard();
		if (removedCard) {
			bool suc = false;
			for (const auto &holder : selHolderVec) {
				suc = holder->addCard(removedCard, unselHolder);
				if (suc)
					break;
			}
			if (!suc) {
				unselHolder->addCard(removedCard);
			}
			doneBtn->setEnabled(suc && canDoneSelect());
		}
	}
}

bool BSCardSelector::canDoneSelect() const {
	auto check = false;
	bool checkFull = selectFull;
	if (checkFull) {
		check = true;
		for (auto &holder : selHolderVec) {
			if (!holder->hasCard())
			{
				check = false;
				break;
			}
		}
	}
	else {
		check = false;
		for (auto &holder : selHolderVec) {
			if (holder->hasCard())
			{
				check = true;
				break;
			}
		}
	}
	return check;
}


void BSCardSelector::onDoneBtnClicked(cocos2d::Ref* sender) {
	if (canDoneSelect()) {
		hide();
	}
}


BATTLE_SCENE_NS_END