#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void onEnter() override;
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	cocos2d::Node *root = nullptr;
	cocos2d::Node *flip1Panel = nullptr;
	cocos2d::Node *flipMulPanel = nullptr;

	cocos2d::Sprite *coinSprite = nullptr;
	bool flipDone = false;

	
	int numHeads = 0;
	int numTails = 0;
	int numFlipLeft = 0;
	void flipMul();
	void doFlipMulCoins();

	void flip1();
	void doFlip1Coin();

};

#endif // __HELLOWORLD_SCENE_H__
