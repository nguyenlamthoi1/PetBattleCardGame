#include "SpriteAnimatorComponent.h"
//#include "cocos2d/cocos/2d/CCAction.h"

using namespace cocos2d;
using namespace std;
MY_COMPONENT_USE_NS;

SA_Animation::SA_Animation() {
}

SA_Animation::~SA_Animation() {
	CCLOG("SA_Animation::dtor called");
	if (ccAnimation) {
		ccAnimation->release();
		ccAnimation = nullptr;
	}
}

SA_Animation* SA_Animation::create() {
	SA_Animation* ret = new(std::nothrow) SA_Animation();
	if (!ret) {
		ret = nullptr;
		delete ret;
		return nullptr;
	}
	ret->autorelease();
	return ret;
}

bool SA_Animation::isValid() {
	return ccAnimation;
}

bool SA_Animation::initWithSpriteFrames(const Vector<SpriteFrame*> &frames, float delay, unsigned int loops) {
	ccAnimation = Animation::createWithSpriteFrames(frames, delay, loops);
	ccAnimation->retain();
	return true;
}


/*
	Class SpriteAnimatorComponent
*/

SpriteAnimatorComponent::SpriteAnimatorComponent(Sprite *s) : sprite(s) {
	CCLOG("SpriteAnimatorComponent::Ctor");
}

SpriteAnimatorComponent::~SpriteAnimatorComponent() {
	CCLOG("SpriteAnimatorComponent::Destructor");
	cleanup();
}

SpriteAnimatorComponent* SpriteAnimatorComponent::getComponent(cocos2d::Node* node) {
	return MyComponentNS::getComponent<SpriteAnimatorComponent>(node, COMPONENT_KEY::SPRITE_ANIMATOR);
}

SpriteAnimatorComponent* SpriteAnimatorComponent::setComponent(cocos2d::Node* node) {
	auto sprite = dynamic_cast<Sprite*>(node);
	CCASSERT(sprite != nullptr, "SpriteAnimatorComponent::setComponent: Sprite == nullptr ");
	SpriteAnimatorComponent* spriteAnimatorComponent = new (std::nothrow) SpriteAnimatorComponent(sprite);
	MyComponentNS::setComponent(node, COMPONENT_KEY::SPRITE_ANIMATOR, spriteAnimatorComponent);
	return spriteAnimatorComponent;
}

void SpriteAnimatorComponent::unsetComponent(cocos2d::Node* node) {
	MyComponentNS::removeComponent(node, COMPONENT_KEY::SPRITE_ANIMATOR);
}

void SpriteAnimatorComponent::cleanup() {
	
	animationMap.clear();

	if(ccAnimateAction){
		sprite->stopAction(ccAnimateAction);
		ccAnimateAction->release();
		ccAnimateAction = nullptr;
	}
	if (sprite) 
		sprite = nullptr;
}

void SpriteAnimatorComponent::addAnimation(const AnimationID& animationId, std::initializer_list<std::string> arrayOfSpriteFrameNames, float delay, unsigned int loop) {
	cocos2d::Vector<cocos2d::SpriteFrame*> arrayFrames;
	auto sfCache = SpriteFrameCache::getInstance();

	for (const auto &frameName : arrayOfSpriteFrameNames) {
		auto frame = sfCache->getSpriteFrameByName(frameName.c_str());
		arrayFrames.pushBack(frame);
	}

	SA_Animation *saAnimation = new (std::nothrow) SA_Animation();
	saAnimation->initWithSpriteFrames(arrayFrames, delay, loop);
	saAnimation->autorelease();
	animationMap.insert(animationId, saAnimation);
}


void SpriteAnimatorComponent::addAnimation(const AnimationID& animationId, const Vector<std::string>& arrayOfSpriteFrameNames, float delay,unsigned int loop) {
	cocos2d::Vector<cocos2d::SpriteFrame*> arrayFrames;
	auto sfCache = SpriteFrameCache::getInstance();
	for (auto& frameName : arrayOfSpriteFrameNames) {
		auto frame = sfCache->getSpriteFrameByName(frameName.c_str());
		arrayFrames.pushBack(frame);
	}
	SA_Animation *saAnimation = new (std::nothrow) SA_Animation();
	saAnimation->initWithSpriteFrames(arrayFrames, delay, loop);
	saAnimation->autorelease();
	animationMap.insert(animationId, saAnimation);
}

void SpriteAnimatorComponent::addAnimation(const AnimationID& animationId, const std::string& plistPath, const char* format, int startCount, int lastCount, float delay, unsigned int loop) {
	auto sfCache = SpriteFrameCache::getInstance();

	// Load file plist (Neu da load roi thi cocos se khong load lan nua)
	sfCache->addSpriteFramesWithFile(plistPath);
	char str[100];
	cocos2d::Vector<cocos2d::SpriteFrame*> arrayFrames;
	for (int i = startCount; i <= lastCount; i++)
	{
		sprintf(str, format, i);
		auto frame = sfCache->getSpriteFrameByName(str);
		if(frame)
			arrayFrames.pushBack(frame);
		else {
			CCASSERT(frame != nullptr, "SA_Animation::addAnimation error: frame is not found");
			return;
		}
	}

	SA_Animation *saAnimation = new (std::nothrow) SA_Animation();
	saAnimation->initWithSpriteFrames(arrayFrames, delay, loop);
	saAnimation->autorelease();
	animationMap.insert(animationId, saAnimation);
	return;
}


void SpriteAnimatorComponent::setAnimationFinishedCallback(const AnimationID& animationId, const AnimationFinishedCallback& finishedCallback) {
	bool foundAnimation = checkAnimationExists(animationId);
	CCASSERT(foundAnimation, "SpriteAnimatorComponent::setAnimationFinishedCallback error: not found animation");
	auto saAnimation = animationMap.at(animationId);
	saAnimation->finishedCallback = finishedCallback;
}

void SpriteAnimatorComponent::playAnimation(const AnimationID& animationId, bool loopForever, AnimationFinishedCallback finishedCallback) {
	stopAnimation(false); // Stop animation nhung khong goi finshed callback
	auto saAnimation = animationMap.at(animationId);
	Action* animateAction = nullptr;
	if (loopForever)
		animateAction = RepeatForever::create(Animate::create(saAnimation->getCCAnimation()));
	else
		animateAction = Sequence::create(
			Animate::create(saAnimation->getCCAnimation()), 
			CallFunc::create([this]() {onAnimationFinished(); }), 
			nullptr);

	setAnimationFinishedCallback(animationId, finishedCallback);
	saAnimation->loopForever = loopForever;
	currentAnimation = animationId;
	ccAnimateAction = animateAction;
	ccAnimateAction->retain();
	sprite->runAction(animateAction);
}

void SpriteAnimatorComponent::stopAnimation(bool callFinished) {
	if (ccAnimateAction) {
		sprite->stopAction(ccAnimateAction);
		if(callFinished)
			onAnimationFinished();
		else {
			if (ccAnimateAction) {
				ccAnimateAction->release();
				ccAnimateAction = nullptr;
			}
			currentAnimation = "";
		}
	}
}

void SpriteAnimatorComponent::onAnimationFinished() {
	if (ccAnimateAction) {
		ccAnimateAction->release();
		ccAnimateAction = nullptr;
	}
	auto saAnimation = animationMap.at(currentAnimation);
	if (saAnimation->finishedCallback)
		saAnimation->finishedCallback(currentAnimation);
	
	currentAnimation = "";
}

bool SpriteAnimatorComponent::isRunning() {
	return !currentAnimation.empty() && ccAnimateAction != nullptr;
}

bool SpriteAnimatorComponent::isAnimationLoopForever(const AnimationID& animationId) {
	return animationMap.at(animationId)->loopForever;
}

bool SpriteAnimatorComponent::checkAnimationExists(const AnimationID& animationId) {
	//bool foundAnimation = _animationMap.find(animationId) != _animationMap.end();
	//CCASSERT(foundAnimation, "SpriteAnimatorComponent::setAnimationFinishedCallback error: not found animation");
	auto saAnimation = animationMap.at(animationId);
	return saAnimation != nullptr;
}
