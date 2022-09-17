#include "HelloWorldScene.h"

#include "ui/UILayout.h"
#include "ui/UIWidget.h"
#include "ui/UIText.h"
#include "ui/UIHelper.h"

#include "components/WidgetTouchComponent.h"
#include "components/SpriteAnimatorComponent.h"
#include "GameManager.h"
#include "common/ResourcePool.h"

#include "common/Utilize.h"


USING_NS_CC;
UTILIZE_USE_NS;
using namespace std;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
/*bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
	//sprite->autorelease();
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
	test();

    return true;
}*/




////////////////////////

SelectedCardHolder* SelectedCardHolder::create() {
	auto ret = new SelectedCardHolder();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	else {
		delete ret;
		ret = nullptr;
	}

	return ret;
}

SelectedCardHolder::SelectedCardHolder() {}

SelectedCardHolder::~SelectedCardHolder() {
	if (root) {
		root->removeFromParent();
		ResourcePool::returnNode(root);
	}
}

bool SelectedCardHolder::init() {
	if (!ui::Layout::init())
		return false;

	auto pool = GM_POOL;
	root = pool->tryGetNodeCsb("ccstd_csb/battle_scene/prefabs/selected_card_holder_1.csb");
	if (!root)
		return false;
	this->addChild(root);

	container = dynamic_cast<ui::Layout*>(root->getChildByName("Container"));
	numPanel = dynamic_cast<ui::ImageView*>(container->getChildByName("Num_Panel"));
	numLabel = dynamic_cast<ui::Text*>(numPanel->getChildByName("Num_Label"));

	this->setContentSize(root->getContentSize());

	return true;
}

////////////////////////

/*
	BSCardSelector Class
*/

shared_ptr<CardSelector> CardSelector::create(Node *root) {
	auto ret = make_shared<CardSelector>(root);
	if (ret && ret->init()) 
		return ret;
	return nullptr;
}

CardSelector::CardSelector(cocos2d::Node *rootnode) : root(rootnode) {}
CardSelector::~CardSelector(){
	clearUI();
}
const string CardSelector::UPDATE_SCHEDULER = "UPDATE_SCHEDULER";
const string CardSelector::UPDATE_SCHEDULER2 = "UPDATE_SCHEDULER2";

bool CardSelector::init() {
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

	return true;
}

void CardSelector::update(float t) {
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


bool CardSelector::initUnselListView() {
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

bool CardSelector::initSelListView() {
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


void CardSelector::clearUI() {
	if (unselectedListView) 
		unselectedListView->removeAllItems();
	unSelHolderVec.clear();

	if (selectedListView) 
		selectedListView->removeAllItems();
	selHolderVec.clear();

	Director::getInstance()->getScheduler()->unschedule(UPDATE_SCHEDULER, this);
}

void CardSelector::showToSelect(const vector<CardId> &cidVec, const SelectMap &selMap) {
	//clearUI();
	//root->setVisible(true);
	//
	//map<CardId, unsigned int> stackedMap;
	//for (const auto &cid : cidVec) {
	//	stackedMap[cid] += 1;
	//}

	//unsigned int i = 0;
	//for (const auto &itr : stackedMap) {
	//	const auto &cid = itr.first;
	//	const auto &num = itr.second;

	//	auto holder = UnselectedCardHolder::create();
	//	holder->renderWith(cid, num);
	//	unselectedListView->addChild(holder);
	//	unSelHolderVec.push_back(holder);
	//	++i;
	//}

	//unselectedNextBtn->addClickEventListener([this](Ref *sender) {
	//	unselectedListView->jumpToRight();
	//	});

	//unselectedPrevBtn->addClickEventListener([this](Ref *sender) {
	//	unselectedListView->jumpToLeft();
	//	});

	//unselectedListView->requestDoLayout();
	//

	////------------//

	//for (const auto &itr : selMap) {
	//	auto selectType = itr.first;
	//	auto num = itr.second;

	//	for (unsigned int i = 0; i < num; ++i) {
	//		auto holder = SelectedCardHolder::create();
	//		selectedListView->addChild(holder);
	//		selHolderVec.push_back(holder);
	//	}
	//}
	//selectedListView->requestDoLayout();
	////-------------//
	//Director::getInstance()->getScheduler()->schedule(std::bind(&CardSelector::update, this, placeholders::_1), this, 0, CC_REPEAT_FOREVER, 0, false, UPDATE_SCHEDULER);

}

void CardSelector::hide() {
	clearUI();
	root->setVisible(false);
}

bool HelloWorld::init() {
	if (!Layer::init())
		return false;
	auto dir = Director::getInstance();
	auto vSize = dir->getVisibleSize();
	auto pool = GM_POOL;

	selectorNode = pool->tryGetNodeCsb("ccstd_csb/battle_scene/select_card_layout.csb");
	this->addChild(selectorNode);
	ui::Helper::doLayout(selectorNode);
	selector = CardSelector::create(selectorNode);
	selectorNode->setVisible(false);

	auto square = ui::Layout::create();
	square->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	square->setBackGroundColor(Color3B::BLUE);
	square->setBackGroundColorOpacity(255);
	square->setContentSize(Size(200, 200));
	square->setPosition(Vec2(0, 0));
	square->setAnchorPoint(Vec2(0, 0));
	square->setTouchEnabled(true);
	square->addTouchEventListener([this](Ref *sender, ui::Widget::TouchEventType ev) {
		switch (ev) {
		case ui::Widget::TouchEventType::ENDED:
			doFunc1();
			break;
		}
		});
	this->addChild(square);

	square = ui::Layout::create();
	square->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	square->setBackGroundColor(Color3B::RED);
	square->setBackGroundColorOpacity(255);
	square->setContentSize(Size(200, 200));
	square->setPosition(Vec2(500, 0));
	square->setAnchorPoint(Vec2(0, 0));
	square->setTouchEnabled(true);
	square->addTouchEventListener([this](Ref *sender, ui::Widget::TouchEventType ev) {
		switch (ev) {
		case ui::Widget::TouchEventType::ENDED:
			doFunc2();
			break;
		}
		});
	this->addChild(square);

	return true;
}


void HelloWorld::doFunc1() {
	CCLOG("Show Selector");
	//vector<string> cids = { "E1", "P1", "P2", "P1", "E1", "P2", "P3", "E2",  "1", "2", "3", "4"};
	vector<string> cids = { "E1", "P1", "P2"};

	CardSelector::SelectMap selmap = {
		{GConfig::SelectType::Basic_Energy_Fire, 2},
	};

	selector->showToSelect(cids, selmap);
}

void HelloWorld::doFunc2() {
	//auto &lv = selector->unselectedListView;
	//selector->root->setVisible(true);
	////selector->clearUI();
	//auto holder = UnselectedCardHolder::create();
	////lv->removeAllItems();
	//lv->addChild(holder);
	//holder = UnselectedCardHolder::create();
	//lv->addChild(holder);
	//holder = UnselectedCardHolder::create();
	//lv->addChild(holder);

	//lv->requestDoLayout();
	selector->hide();

}

void HelloWorld::onEnter() {
	Layer::onEnter();
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
