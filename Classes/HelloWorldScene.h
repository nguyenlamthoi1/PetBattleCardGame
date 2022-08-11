#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	bool init1();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	void beginCb() {
		CCLOG("Begin call back");
	}
	void dragIn(cocos2d::Node *node, cocos2d::Node* cnode) {
		cnode->setColor(cocos2d::Color3B::RED);
		CCLOG("%s go in %s", node->getName().c_str(), cnode->getName().c_str());
	}
	void dragOut(cocos2d::Node *node, cocos2d::Node* cnode) {
		cnode->setColor(cocos2d::Color3B::WHITE);
		CCLOG("%s go out %s", node->getName().c_str(), cnode->getName().c_str());
	}

	void dragEnd(cocos2d::Node *node, cocos2d::Node* cnode) {
		if(cnode)
			CCLOG("%s can be dropped at %s", node->getName().c_str(), cnode->getName().c_str());
		else {
			CCLOG("%s can't be dropped", node->getName().c_str());

		}
	}
};

#endif // __HELLOWORLD_SCENE_H__
