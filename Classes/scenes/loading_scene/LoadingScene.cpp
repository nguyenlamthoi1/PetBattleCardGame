#include "LoadingScene.h"
#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "define/CommonDefine.h"

USING_NS_CC;

using namespace std;

const string LoadingScene::LOGO_NAME = "LogoSprite";
const string LoadingScene::CIRCLE_NAME = "CircleSprite";


LoadingScene* LoadingScene::create()
{
	LoadingScene *scn = new LoadingScene();
	if (scn->init())
	{
		scn->autorelease();
		return scn;
	}
	else
	{
		delete scn;
		scn = nullptr;
		return nullptr;
	}
}

bool LoadingScene::init() {
	if (!Scene::init()) 
		return false;

	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();

	layout = ui::Layout::create();
	layout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	layout->setBackGroundColorOpacity(255);
	layout->setBackGroundColor(Color3B(250, 214, 184));
	layout->setContentSize(visibleSize); // Xet bang kich thuoc man hinh

	auto logoSprite = Sprite::create("single_images/logo_with_shadow.png");
	Size logoSize = logoSprite->getContentSize();
	logoSprite->setName(LOGO_NAME);
	logoSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	logoSprite->setScale(1.5f);
	layout->addChild(logoSprite);

	auto circleSprite = Sprite::create("single_images/hud_loading_circle.png");
	Size circleSize = circleSprite->getContentSize();
	circleSprite->setName(CIRCLE_NAME);
	circleSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 200));
	layout->addChild(circleSprite);

	bottomLabel = ui::Text::create("", "fonts\Pokemon_Pixel_Font", 36);
	bottomLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 300));
	bottomLabel->setTextColor(Color4B::WHITE);
	bottomLabel->setString("");
	bottomLabel->setScale(1.0 / 2);
	layout->addChild(bottomLabel);

	this->addChild(layout);

	return true;
}

LoadingScene::LoadingScene() {
}

LoadingScene::~LoadingScene() {

}

void LoadingScene::onEnter() {
	Scene::onEnter();
	playScene();

	auto scheduler = Director::getInstance()->getScheduler();
	startLoading();
}

void LoadingScene::onExit() {
	Scene::onExit();
}

void LoadingScene::playScene() {
	layout->setOpacity(0);
	layout->runAction(FadeIn::create(0.5));

	Node *logoSprite = layout->getChildByName(LOGO_NAME);
	logoSprite->runAction(RepeatForever::create(
		Sequence::create(
			FadeTo::create(1.0, 255),
			FadeTo::create(1.0, 127),
			nullptr)));
	
	Node *circleSprite = layout->getChildByName(CIRCLE_NAME);
	circleSprite->runAction(RepeatForever::create(::RotateBy::create(1.0, 360)));
	
}

void LoadingScene::startLoading() {
	auto loader = GameManager::getInstance()->getLoader();

	// Them file can load

	//--
	auto fileCount = loader->stepCount();
	if (fileCount > 0) {
		auto evListener = EventListenerCustom::create(EVENT_CUSTOM::RES_LOADING_STEP_FINISHED,
			[this](EventCustom* eventData) {
				auto data = dynamic_cast<EVENT_CUSTOM::EC_LoadStepFinishedData*>(eventData);
				onFileLoaded(data);
			});
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		dispatcher->addEventListenerWithSceneGraphPriority(evListener, this);
	}

	auto evListener = EventListenerCustom::create(EVENT_CUSTOM::RES_LOADING_FINISHED,
		[this](EventCustom* eventData) {
			auto data = dynamic_cast<EVENT_CUSTOM::EC_LoadingFinishedData*>(eventData);
			onLoadingFinished(data);
		});
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(evListener, this);
	
	loader->startLoading();
}

void LoadingScene::onFileLoaded(EVENT_CUSTOM::EC_LoadStepFinishedData *data) {

}

void LoadingScene::onLoadingFinished(EVENT_CUSTOM::EC_LoadingFinishedData *data) {
	CCLOG("LoadingScene::onLoadingFinished");
	auto gm = GameManager::getInstance();
	gm->playTitleSceneFade();
}



