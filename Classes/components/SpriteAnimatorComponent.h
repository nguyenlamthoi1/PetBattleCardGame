#pragma once

#include "MyComponent.h"
#include "cocos2d.h"

/*
	- Component gan vao 1 Sprite
	- Dieu khien viec thay doi sprite frame -> tao thanh animation
	- Co the chuyen doi qua lai nhieu animation
*/


typedef std::string AnimationID;
typedef std::function<void(AnimationID)> AnimationFinishedCallback;

class SpriteAnimatorComponent;

class SA_Animation : public cocos2d::Ref {
protected:
	friend class SpriteAnimatorComponent;

	cocos2d::Animation  *ccAnimation;
	AnimationID animation; // Ten(Id) cua animation
	bool loopForever = false;
	AnimationFinishedCallback finishedCallback;

	SA_Animation();
	virtual ~SA_Animation();
public:
	bool initWithSpriteFrames(const cocos2d::Vector<cocos2d::SpriteFrame*>& frames, float delay = 0.0f, unsigned int loops = 1);
	static SA_Animation* create();
	bool isValid();
	AnimationID getAnimationName() { return animation; }
	cocos2d::Animation* getCCAnimation() { return ccAnimation; }
protected:
};


class SpriteAnimatorComponent : public MComponent{
protected:
	cocos2d::Sprite* sprite = nullptr;
	cocos2d::Map<AnimationID, SA_Animation*> animationMap;
	AnimationID currentAnimation;
	cocos2d::Action* ccAnimateAction;
public:
	SpriteAnimatorComponent(cocos2d::Sprite *s);
	virtual ~SpriteAnimatorComponent();

	static SpriteAnimatorComponent* setComponent(cocos2d::Node* node);
	static void unsetComponent(cocos2d::Node* node);
	static SpriteAnimatorComponent* getComponent(cocos2d::Node* node);

	virtual void cleanup();

	void addAnimation(const AnimationID& animationId, const cocos2d::Vector<std::string*>& arrayOfSpriteFrameNames, float delay = 0.0f, unsigned int loop = 1);
	void addAnimation(const AnimationID& animationId, const std::string& plistPath, const char* format, int startCount, int lastCount, float delay = 0.0f, unsigned int loop = 1);
	void playAnimation(const AnimationID& animation, bool loopForever = false, AnimationFinishedCallback finishedCallback = nullptr); // Play 1 animation voi id(string)
	void setAnimationFinishedCallback(const AnimationID& animationId, const AnimationFinishedCallback& finishedCallback);
	void stopAnimation(bool callFinished = false); // Dung animation(action) hien tai
	AnimationID getCurrentAnimationId() { return currentAnimation; }
	bool isRunning();
	bool isAnimationLoopForever(const AnimationID& animationId);
protected:
	void onAnimationFinished();
protected:
	bool checkAnimationExists(const AnimationID& animationId);

};

