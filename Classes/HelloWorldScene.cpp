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

bool HelloWorld::init() {
	if (!Layer::init())
		return false;
	auto dir = Director::getInstance();
	auto vSize = dir->getVisibleSize();
	auto pool = GM_POOL;

	root = pool->tryGetNodeCsb("ccstd_csb/battle_scene/msg_layer.csb");
	this->addChild(root);
	root->setPosition(Vec2(0, 0));

	ui::Helper::doLayout(root);

	std::string msg = "Tap ready button if you are ready to play the game and having fun \n dgsdg dgdg sdfdsf dfdg ";

	ui::Layout* msgBoard = dynamic_cast<ui::Layout*>(root->getChildByName("Msg_Board"));
	msgBoard->setVisible(true);

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
	//msgBoard->doLayout();

	//text->setAnchorPoint(Vec2(0.5, 0.5));
	//auto newBoardSize = msgBoard->getContentSize();
	//text->setPosition(Vec2(newBoardSize.width / 2, newBoardSize.height / 2)); // Text o vi tri center

	return true;

}

void HelloWorld::onEnter() {
	Layer::onEnter();
}

void HelloWorld::doFlipMulCoins() {

	auto comp = SpriteAnimatorComponent::getComponent(coinSprite);
	if (!comp) {
		comp = SpriteAnimatorComponent::setComponent(coinSprite);
	}
	comp->addAnimation("Flip_Pikachu", {
		"flip_coin_animations/yellow/flip_coin_yellow_pikachu.png",
		"flip_coin_animations/yellow/flip_coin_yellow_0.png",
		"flip_coin_animations/yellow/flip_coin_yellow_1.png",
		"flip_coin_animations/yellow/flip_coin_yellow_2.png",
		"flip_coin_animations/yellow/flip_coin_yellow_3.png",
		"flip_coin_animations/yellow/flip_coin_yellow_4.png",
		"flip_coin_animations/yellow/flip_coin_yellow_5.png",
		"flip_coin_animations/yellow/flip_coin_yellow_6.png"
		}, 1.0f / 8);
	comp->addAnimation("Flip_Pikachu_End", {
	"flip_coin_animations/yellow/flip_coin_yellow_pikachu.png",
	"flip_coin_animations/yellow/flip_coin_yellow_0.png",
	"flip_coin_animations/yellow/flip_coin_yellow_1.png",
	"flip_coin_animations/yellow/flip_coin_yellow_2.png",
	"flip_coin_animations/yellow/flip_coin_yellow_3.png",
	"flip_coin_animations/yellow/flip_coin_yellow_4.png",
	"flip_coin_animations/yellow/flip_coin_yellow_5.png",
	"flip_coin_animations/yellow/flip_coin_yellow_6.png",
	//"flip_coin_animations/yellow/flip_coin_yellow_pikachu.png"
	"flip_coin_animations/yellow/flip_coin_yellow_3.png"

		}, 1.0f / 9);
	auto resBoard = flip1Panel->getChildByName("Result_Flip_Board");
	resBoard->setVisible(true);
	resBoard->setOpacity(0);

	comp->playAnimation("Flip_Pikachu", true);
	coinSprite->setPosition(Vec2(0, 0));
	coinSprite->runAction(Sequence::create(
		MoveBy::create(0.5f, Vec2(0, 45)),
		MoveBy::create(0.5f, Vec2(0, -45)),
		CallFunc::create([this]() {
			auto comp = SpriteAnimatorComponent::getComponent(coinSprite);
			comp->playAnimation("Flip_Pikachu_End", false, [this](std::string anim) {
				auto gotHead = cocos2d::RandomHelper::random_int(0, 1) == 1;
				auto preStr = gotHead ? "Head : " : "Tails : ";

				auto subStr = gotHead ? std::to_string(++numHeads) : std::to_string(++numTails);
				if (gotHead) {
					auto headsText = dynamic_cast<ui::Text*>(flipMulPanel->getChildByName("Num_Heads_Board")->getChildByName("Num_Heads_Text"));
					headsText->setString(preStr + subStr);
				}
				else {
					auto tailsText = dynamic_cast<ui::Text*>(flipMulPanel->getChildByName("Num_Tails_Board")->getChildByName("Num_Tails_Text"));
					tailsText->setString(preStr + subStr);
				}
				--numFlipLeft;
				if (numFlipLeft > 0)
					doFlipMulCoins();
				});
			}),
		nullptr));
}


void HelloWorld::doFlip1Coin() {
	auto comp = SpriteAnimatorComponent::setComponent(coinSprite);
	comp->addAnimation("Flip_Pikachu", {
		"flip_coin_animations/yellow/flip_coin_yellow_pikachu.png",
		"flip_coin_animations/yellow/flip_coin_yellow_0.png",
		"flip_coin_animations/yellow/flip_coin_yellow_1.png",
		"flip_coin_animations/yellow/flip_coin_yellow_2.png",
		"flip_coin_animations/yellow/flip_coin_yellow_3.png",
		"flip_coin_animations/yellow/flip_coin_yellow_4.png",
		"flip_coin_animations/yellow/flip_coin_yellow_5.png",
		"flip_coin_animations/yellow/flip_coin_yellow_6.png"
		}, 1.0f / 8);
	comp->addAnimation("Flip_Pikachu_End", {
	"flip_coin_animations/yellow/flip_coin_yellow_pikachu.png",
	"flip_coin_animations/yellow/flip_coin_yellow_0.png",
	"flip_coin_animations/yellow/flip_coin_yellow_1.png",
	"flip_coin_animations/yellow/flip_coin_yellow_2.png",
	"flip_coin_animations/yellow/flip_coin_yellow_3.png",
	"flip_coin_animations/yellow/flip_coin_yellow_4.png",
	"flip_coin_animations/yellow/flip_coin_yellow_5.png",
	"flip_coin_animations/yellow/flip_coin_yellow_6.png",
	//"flip_coin_animations/yellow/flip_coin_yellow_pikachu.png"
	"flip_coin_animations/yellow/flip_coin_yellow_3.png"

		}, 1.0f / 9);
	auto resBoard = flip1Panel->getChildByName("Result_Flip_Board");
	resBoard->setVisible(true);
	resBoard->setOpacity(0);

	comp->playAnimation("Flip_Pikachu", true);
	coinSprite->setPosition(Vec2(0, 0));
	coinSprite->runAction(Sequence::create(
		MoveBy::create(0.5f, Vec2(0, 45)),
		MoveBy::create(0.5f, Vec2(0, -45)),
		CallFunc::create([this]() {
			auto comp = SpriteAnimatorComponent::getComponent(coinSprite);
			comp->playAnimation("Flip_Pikachu_End", false, [this](std::string anim) {
				auto resBoard = flip1Panel->getChildByName("Result_Flip_Board");
				auto resText = dynamic_cast<ui::Text*>(resBoard->getChildByName("Result_Flip_Text"));
				resText->setString("Tails");
				resBoard->runAction(FadeIn::create(0.5f));
				});
			}),
		nullptr));
}

void HelloWorld::flip1() {
	flip1Panel->setScale(0.75f);
	flip1Panel->setVisible(true);
	flip1Panel->setOpacity(0);
	flip1Panel->runAction(Sequence::create(
		Spawn::create(
			FadeIn::create(0.3f),
			ScaleTo::create(0.3f, 1.0f),
			nullptr),
		DelayTime::create(0.5f),
		CallFunc::create([this]() {
			doFlip1Coin();
			}),
		nullptr
		));
}

void HelloWorld::flipMul() {
	numFlipLeft = 5;
	flipMulPanel->setScale(0.75f);
	flipMulPanel->setVisible(true);
	flipMulPanel->setOpacity(0);
	flipMulPanel->runAction(Sequence::create(
		Spawn::create(
			FadeIn::create(0.3f),
			ScaleTo::create(0.3f, 1.0f),
			nullptr),
		DelayTime::create(0.5f),
		CallFunc::create([this]() {
			doFlipMulCoins();
			}),
		nullptr
				));
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
