#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "cocos2d.h"
#include <memory>

namespace ResourceLoaderNS {
	class ResourceLoader;
}

class ResourcePool;

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
	std::shared_ptr<ResLoader> loader; // = nullptr
	std::shared_ptr<ResourcePool> pool;
	// Non-static members
	// Ctors

	// Data Members

	// Private functions
	bool init();
};

#endif // __GAME_MANAGER_H__
