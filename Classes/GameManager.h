#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "common/ResourceLoader.h"
#include "common/ResourcePool.h"
#include "common/LangSys.h"
#include "common/DataManager.h"
#include "common/event/EventManager.h"

#include "cocos2d.h"

#include <memory>

namespace ResourceLoaderNS {
	class ResourceLoader;
}

class ResourcePool;
class LangSys;
class DataManager;
class EventManager;

class GameManager // Singleton
{
public:
	using ResLoader = ResourceLoaderNS::ResourceLoader;

	static std::shared_ptr<GameManager> getInstance();
	static void destroyInstance();

	GameManager(GameManager &other) = delete; // shouldn't be cloneable
	void operator=(const GameManager &other) = delete; // shouldn't be assignable

	// Getters
	std::shared_ptr<ResLoader> getLoader() { return loader; }
	std::shared_ptr<ResourcePool> getPool() { return pool; }
	std::shared_ptr<LangSys> getLang() { return lang; }
	std::shared_ptr<DataManager> getDataManager() { return dataMgr; }
	std::shared_ptr<EventManager> getEventManager() { return evMgr; }


	bool startGame() const; // start the scene
	void exitGame() const;
	void changeScene(cocos2d::Scene *scn) const;
	void changeSceneFade(cocos2d::Scene *scn) const;
	void playTitleScene() const;
	void playTitleSceneFade() const;

	~GameManager();
	GameManager();

private:
	// Static members

	static std::shared_ptr<GameManager> instance; // = nullptr

	// Non-static members

	std::shared_ptr<ResLoader> loader;
	std::shared_ptr<ResourcePool> pool;
	std::shared_ptr<LangSys> lang;
	std::shared_ptr<DataManager> dataMgr;
	std::shared_ptr<EventManager> evMgr;

	// Ctors

	// Data Members

	// Private functions
	bool init();
};

#define GM_LANG GameManager::getInstance()->getLang()
#define GM_POOL GameManager::getInstance()->getPool()
#define GM_LOADER GameManager::getInstance()->getLoader()
#define GM_DATA_MGR GameManager::getInstance()->getDataManager()
#define GM_EV_MGR GameManager::getInstance()->getEventManager()

#endif // __GAME_MANAGER_H__
