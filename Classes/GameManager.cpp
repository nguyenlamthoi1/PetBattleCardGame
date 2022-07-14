#include "GameManager.h"

USING_NS_CC;

GameManager *GameManager::instance = nullptr;

GameManager* GameManager::getInstance() {
	if (instance == nullptr) {
		instance = new GameManager();
		if (!instance->init()) {
			delete instance;
			instance == nullptr;
		}
	}
	return instance;
}

bool GameManager::init() {
	// Load du lieu ve card

	// Load du lieu cua user

	// Chay scene co logo



	return true;
}

void GameManager::destroyInstance() {
	if (instance != nullptr) {
		delete instance;
		instance = nullptr;
	}
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
