#include "GameManager.h"
#include "HelloWorldScene.h"
#include "scenes/title_scene/TitleScene.h"

USING_NS_CC;
RESOURCE_LOADER_USE_NS;
using namespace std;

std::shared_ptr<GameManager> GameManager::instance;

std::shared_ptr<GameManager> GameManager::getInstance() {
	if (!instance) { // instance == nullptr
		instance = std::make_shared<GameManager>();
		if (instance && !instance->init()) {
			instance.reset();
		}
	}
	return instance;
}

GameManager::GameManager() : 
	loader(new ResourceLoader()), 
	pool(new ResourcePool()),
	lang(new LangSys()),
	dataMgr(new DataManager()),
	evMgr(new EventManager())
{
}

GameManager::~GameManager()
{
	CCLOG("GameManager::dtor called");
}


bool GameManager::init() {
	// Load du lieu ve card

	// Load du lieu cua user

	// Chay scene co logo



	return true;
}

void GameManager::destroyInstance() {
	if (instance) 
		instance.reset();
}

bool GameManager::startGame() const {
	CCLOG("GM: Start the game");
	return true; // Indicate Success
}

void GameManager::exitGame() const {
	CCLOG("GM: Exit the game");
}

void GameManager::changeScene(Scene *scene) const {
	Director* director = Director::getInstance();
	if (director->getRunningScene()) // != nullptr
		director->replaceScene(scene);
	else
		director->runWithScene(scene);
}

void GameManager::changeSceneFade(Scene *scene) const {
	Director* director = Director::getInstance();
	if (director->getRunningScene()) // != nullptr
		director->replaceScene(TransitionFade::create(0.5, scene, Color3B(220, 110, 85)));
	else
		director->runWithScene(scene);
}


void GameManager::playTitleScene() const {
	changeScene(HelloWorld::createScene());
}

void GameManager::playTitleSceneFade() const {
	//changeSceneFade(HelloWorld::createScene());
	changeSceneFade(TitleScene::create());
}
