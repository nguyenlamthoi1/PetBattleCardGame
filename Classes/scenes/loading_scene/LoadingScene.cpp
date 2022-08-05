#include "LoadingScene.h"
#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "common/LangSys.h"
#include "common/DataManager.h"
#include "define/CommonDefine.h"

USING_NS_CC;
RESOURCE_LOADER_USE_NS;
using namespace std;

const string LoadingScene::LOGO_NAME = "LogoSprite";
const string LoadingScene::CIRCLE_NAME = "CircleSprite";
const string LoadingScene::SCENE_SCHEDULER = "LoadingScene::onLoadingFinished";


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
	auto scheduler = Director::getInstance()->getScheduler();
	scheduler->unschedule(SCENE_SCHEDULER, this);
}

void LoadingScene::onEnter() {
	Scene::onEnter();
	playScene();
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
	//loader->addPlistFile("");
	loader->addLoadedObj(LoadedObject("data\\localization\\en.txt", []()->bool {
		return GameManager::getInstance()->getLang()->load("data\\localization\\en.txt");
		}));

	loader->addLoadedObj(LoadedObject("data\\energy_cards.json", []()->bool {
		return GameManager::getInstance()->getDataManager()->loadEnergyCards();
		}));

	loader->addLoadedObj(LoadedObject("data\\pet_cards.json", []()->bool {
		return GameManager::getInstance()->getDataManager()->loadPetCards();
		}));

	loader->addLoadedObj(LoadedObject("data\\player_data.json", []()->bool {
		return GameManager::getInstance()->getDataManager()->loadPlayerData();
		}));

	loader->addLoadedObj(LoadedObject("data\\energy_images_x3.plist", []()->bool {
		return GameManager::getInstance()->getLoader()->loadPlistFile("ccstd_assets/energy_images_x3.plist");
		}));
	
	//--

	auto fileCount = loader->stepCount();
	if (fileCount > 0) {
		auto evListener = EventListenerCustom::create(EVENT_CUSTOM::RES_LOADING_STEP_FINISHED,
			[this](EventCustom* event) {
				auto data = static_cast<EVENT_CUSTOM::EC_LoadStepFinishedData*>(event->getUserData());
				onFileLoaded(data);
			});
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		dispatcher->addEventListenerWithSceneGraphPriority(evListener, this);
	}

	auto evListener = EventListenerCustom::create(EVENT_CUSTOM::RES_LOADING_FINISHED,
		[this](EventCustom* event) {
			auto data = static_cast<EVENT_CUSTOM::EC_LoadingFinishedData*>(event->getUserData());
			onLoadingFinished(data);
		});
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(evListener, this);
	loader->startLoading();
}

void LoadingScene::onFileLoaded(EVENT_CUSTOM::EC_LoadStepFinishedData *data) {
	CCLOG("LoadingScene::onFileLoaded: %s: %s at %d / %d", data->fName.c_str(), data->suc ? "succeeded" : "failed", data->cur + 1, data->total);
}

void LoadingScene::onLoadingFinished(EVENT_CUSTOM::EC_LoadingFinishedData *data) {
	CCLOG("LoadingScene::onLoadingFinished: %d / %d", data->loaded, data->total);
	auto scheduler = Director::getInstance()->getScheduler();
	scheduler->schedule([this](float) {	
		auto gm = GameManager::getInstance();
		gm->playTitleSceneFade(); 
		}, this, 0, 0, 1.5, false, SCENE_SCHEDULER);
}



