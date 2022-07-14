#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "cocos2d.h"

class GameManager // Singleton
{
public:
	static GameManager* getInstance();
	static void destroyInstance();

	GameManager(GameManager &other) = delete; // shouldn't be cloneable
	void operator=(const GameManager &other) = delete; // shouldn't be assignable

	bool startGame() const; // start the scene
	void exitGame() const;
	void changeScene(cocos2d::Scene *scn) const;
	
protected:
	// Static members
	static GameManager *instance; // = nullptr
	

	// Non-static members
	// Ctors
	GameManager(){}

	// Data Members

	// Private functions
	bool init();
};

#endif // __GAME_MANAGER_H__
