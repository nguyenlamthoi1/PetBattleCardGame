#include "WidgetTouchComponent.h"
#include "ui/UIWidget.h"
#include "common/Utilize.h"

USING_NS_CC;
MY_COMPONENT_USE_NS;
UTILIZE_USE_NS;
using namespace std;

/*
	DragComponent Class
*/

DragComponent::~DragComponent() {
	CCLOG("DragComponent::dtor");
}


/*
	WidgetTouchComponent Class
*/


WidgetTouchComponent::WidgetTouchComponent(ui::Widget *w,
	const TouchHandlerFunc& beganFunc,
	const TouchHandlerFunc& endedFunc,
	const TouchHandlerFunc& movedFunc,
	const TouchHandlerFunc& holdFunc) :
widget(w), beganCb(beganFunc), endedCb(endedFunc), movedCb(movedFunc), holdCb(holdFunc)
{
	applyHandler();
}

WidgetTouchComponent::~WidgetTouchComponent() {
	CCLOG("WidgetTouchComponent::dtor called");
}

const float WidgetTouchComponent::TOUCH_MOVE_DELTA = 24.0f;
const string WidgetTouchComponent::HOLD_TOUCH_SCHEDULE_KEY = "HOLD_TOUCH_SCHEDULE_KEY";

void WidgetTouchComponent::applyHandler() {
	CCASSERT(widget != nullptr, "WidgetTouchComponent::applyHandler: widget is NULL");

	auto func = [this](Ref *sender, ui::Widget::TouchEventType eventType)->void {
		switch (eventType) {
		case ui::Widget::TouchEventType::BEGAN:
		{
			if (beganCb)
				beganCb(widget, nullptr, nullptr);

			if (holdCb) {
				checkingLongTouch = false;
				beganTouchPos = widget->getTouchBeganPosition();
				Director::getInstance()->getScheduler()->schedule([this](float dt)
					{
						checkingLongTouch = true;
						Vec2 touchStartLoc = beganTouchPos;
						Vec2 touchCurLoc = widget->getTouchBeganPosition();
						float distance = touchStartLoc.distance(touchCurLoc);
						if (distance < TOUCH_MOVE_DELTA) { // Touch duoc xem la khong bi di chuyen
							if (holdCb)
								holdCb(widget, nullptr, nullptr);
						}
					}, this, 0, 0, longTouchDelayCheckTime, false, HOLD_TOUCH_SCHEDULE_KEY);
			}

			// Drag Comp
			auto comp = MyComponentNS::getComponent<DragComponent>(widget, COMPONENT_KEY::DRAG);
			if (comp && comp->isEnabled()) {
				comp->isDragging = false;
				comp->orgPos = widget->getPosition();
				comp->orgZ = widget->getLocalZOrder();
				comp->orgWorldPos = widget->getWorldPosition();
				comp->offset = widget->getTouchBeganPosition() - comp->orgWorldPos;
				comp->orgParent = widget->getParent();

			}
			// --

			break;
		}
		case ui::Widget::TouchEventType::ENDED:
		case ui::Widget::TouchEventType::CANCELED:
		{
			if (holdCb && !checkingLongTouch)
				cocos2d::Director::getInstance()->getScheduler()->unschedule(HOLD_TOUCH_SCHEDULE_KEY, this);

			if (endedCb) 
				endedCb(widget, nullptr, nullptr);

			// Drag Comp
			auto comp = MyComponentNS::getComponent<DragComponent>(widget, COMPONENT_KEY::DRAG);
			if (comp && comp->isEnabled() && comp->isDragging) {
				comp->isDragging = false;
				if(comp->dragEndCallback)
					comp->dragEndCallback(widget, comp->hitDestNode);
				comp->hitDestNode = nullptr;
			}
			//--

			break;
		}
		case ui::Widget::TouchEventType::MOVED: {
			if (movedCb) 
				movedCb(widget, nullptr, nullptr);


			// Drag comp
			auto comp = MyComponentNS::getComponent<DragComponent>(widget, COMPONENT_KEY::DRAG);
			if (comp && comp->isEnabled()) {
				cocos2d::Director::getInstance()->getScheduler()->unschedule(HOLD_TOUCH_SCHEDULE_KEY, this);

				if (comp->isDragging) {
					// Set position cho obj tuong doi voi drag container
					Vec2 wPos;
					if (!comp->useCenter)
						wPos = widget->getTouchMovePosition() - comp->offset;
					else
						wPos = widget->getTouchMovePosition();
					auto lPos = widget->getParent()->convertToNodeSpaceAR(wPos);
					widget->setPosition(comp->dragContainer == Director::getInstance()->getRunningScene() ? wPos : lPos);
					bool useCenter = false;
					auto wCheckPos = widget->getTouchMovePosition();
					for (const auto &dNode : comp->destinations) {
						auto s = dNode->getContentSize();

						// C1
						//auto wP = Utilize::mnode::getWorldPos(dNode);
						//auto rect = Rect(wP.x, wP.y, s.width, s.height);
						//bool check = rect.containsPoint(wCheckPos);
						// C2
						auto lPos = dNode->convertToNodeSpaceAR(wCheckPos);
						auto rect = Rect(0, 0, s.width, s.height);
						bool check = rect.containsPoint(lPos);

						if (check) {

							if (comp->hitDestNode && comp->hitDestNode != dNode) {
								// DRAG OUT CALLBACK
								if(comp->dragOutCallback)
									comp->dragOutCallback(widget, comp->hitDestNode);
								comp->hitDestNode = nullptr;
							}

							if (!comp->hitDestNode || comp->hitDestNode != dNode) {
								// DRAG IN CALLBACK
								if(comp->dragInCallback)
									comp->dragInCallback(widget, dNode);
								comp->hitDestNode = dNode;
							}

							break;
						}
						else {
							if (comp->hitDestNode && comp->hitDestNode == dNode) {
								// DRAG OUT CALLBACK
								if(comp->dragOutCallback)
									comp->dragOutCallback(widget, dNode);
								comp->hitDestNode = nullptr;
							}
						}
					}
				}
				else {
					// bat dau drag
					auto d = widget->getTouchBeganPosition().distance(widget->getTouchMovePosition());
					if (d > TOUCH_MOVE_DELTA) {
						comp->isDragging = true;

						// add child vao drag container
						widget->removeFromParent();
						if (!comp->dragContainer) 
							comp->dragContainer = Director::getInstance()->getRunningScene();
						comp->dragContainer->addChild(widget, comp->zInContainer);

						// Set position cho obj tuong doi voi drag container
						Vec2 wPos;
						if (!comp->useCenter) 
							wPos = widget->getTouchMovePosition() - comp->offset;
						else 
							wPos = widget->getTouchMovePosition();
						auto lPos = widget->getParent()->convertToNodeSpaceAR(wPos);
						widget->setPosition(comp->dragContainer == Director::getInstance()->getRunningScene() ? wPos : lPos);
						if(comp->dragBeginCallback)
							comp->dragBeginCallback(widget, nullptr); // DRAG BEGIN CALLBACK
					}
				}
			}
			//--

			break;
		}
		}
	};

	widget->addTouchEventListener(func);
}


/*
	NodeTouchComponent Class
*/

//const float NodeTouchComponent::TOUCH_MOVE_DELTA = 24.0f;
//const string NodeTouchComponent::NODE_HOLD_TOUCH_SCHEDULE_KEY = "NODE_HOLD_TOUCH_SCHEDULE_KEY";
//
//NodeTouchComponent::NodeTouchComponent(Node *n,
//	const NodeTouchHandlerFunc& beganFunc,
//	const NodeTouchHandlerFunc& endedFunc,
//	const NodeTouchHandlerFunc& movedFunc,
//	const NodeTouchHandlerFunc& holdFunc) :
//	node(n), beganCb(beganFunc), endedCb(endedFunc), movedCb(movedFunc), holdCb(holdFunc)
//{
//	applyHandler();
//}
//
//NodeTouchComponent::~NodeTouchComponent() {
//	CCLOG("WidgetTouchComponent::dtor called");
//}
//
//void NodeTouchComponent::applyHandler() {
//	CCASSERT(node != nullptr, "WidgetTouchComponent::applyHandler: widget is NULL");
//
//	eventListener = EventListenerTouchOneByOne::create();
//	eventListener->setSwallowTouches(swallowTouches);
//	eventListener->onTouchBegan = [this](Touch* touch, Event* event)->bool {
//		Node* target = event->getCurrentTarget();
//
//		CCASSERT(node != nullptr, "WidgetTouchComponent::applyHandler: widget is NULL");
//
//		Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
//		Size s = target->getContentSize();
//		Rect rect = Rect(0, 0, s.width * target->getScaleX(), s.height * target->getScaleY());
//		bool checkTouchBeganCond = target->isVisible() && rect.containsPoint(locationInNode);
//
//		if (checkTouchBeganCond) {
//			CCLOG("TouchHandlerComponent:: onTouchBegan: %s", target->getName().c_str());
//			if (beganCb)
//				beganCb(node, nullptr, nullptr);
//				
//			if (!holdCb)
//				return swallowTouches; // not handle long touch -> swallow touch 
//
//			if (holdCb) {
//				checkingLongTouch = false;
//				cocos2d::Director::getInstance()->getScheduler()->schedule([this, target, touch](float dt)
//					{
//						checkingLongTouch = true;
//						Vec2 touchStartLoc = touch->getStartLocation();
//						Vec2 touchCurLoc = touch->getLocation();
//						float distance = touchStartLoc.distance(touchCurLoc);
//						if (distance < TOUCH_MOVE_DELTA) { // Touch is not determined being moved 
//							if (holdCb)
//								holdCb(target, nullptr, nullptr);
//						}
//					}, this, 0, 0, longTouchDelayCheckTime, false, NODE_HOLD_TOUCH_SCHEDULE_KEY);
//			}
//		}
//		return swallowTouches;
//
//		return false; // -> not swallow -> pass event
//	};
//
//	eventListener->onTouchEnded = [this](Touch* touch, Event* event)->void {
//		Node* node = event->getCurrentTarget();
//		CCLOG("On Touch ended %s", node->getName().c_str());
//
//		// Handle long touch
//		if (holdCb && !checkingLongTouch)
//			cocos2d::Director::getInstance()->getScheduler()->unschedule(NODE_HOLD_TOUCH_SCHEDULE_KEY, this);
//
//		if (endedCb) {
//			Vec2 touchStartLoc = touch->getStartLocation();
//			Vec2 touchGetLocation = touch->getLocation();
//			float distance = touchStartLoc.distance(touchGetLocation);
//		}
//
//	};
//	eventListener->onTouchMoved = [this](Touch* touch, Event* event) ->void {
//		/*Node* target = event->getCurrentTarget();
//		Rect rect = target->getBoundingBox();
//		Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
//		bool checkTouchBeganCond = rect.containsPoint(locationInNode);
//
//		if (checkTouchBeganCond) {
//			CCLOG("On Touch moved inside %s", target->getName().c_str());
//
//		}
//		else {
//			CCLOG("On Touch moved outside %s", target->getName().c_str());
//		}*/
//	};
//
//	auto eventDispatcher = Director::getInstance()->getEventDispatcher();
//	eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, node);
//}



WIDGET_TOUCH_NS_BEGIN

void setWidgetTouchBegan(cocos2d::ui::Widget *w, const TouchHandlerFunc &cb) {
	auto comp = getComponent<WidgetTouchComponent>(w, COMPONENT_KEY::WIDGET_TOUCH);
	if (!comp) {
		comp = new WidgetTouchComponent(w);
		setComponent(w, COMPONENT_KEY::WIDGET_TOUCH, comp);
	}
	comp->setTouchBeganCb(cb);
}

void setWidgetTouchEnded(cocos2d::ui::Widget *w, const TouchHandlerFunc &cb) {
	auto comp = getComponent<WidgetTouchComponent>(w, COMPONENT_KEY::WIDGET_TOUCH);
	if (!comp) {
		comp = new WidgetTouchComponent(w);
		setComponent(w, COMPONENT_KEY::WIDGET_TOUCH, comp);
	}
	comp->setTouchEndedCb(cb);
}

void setWidgetTouchMoved(cocos2d::ui::Widget *w, const TouchHandlerFunc &cb) {
	auto comp = getComponent<WidgetTouchComponent>(w, COMPONENT_KEY::WIDGET_TOUCH);
	if (!comp) {
		comp = new WidgetTouchComponent(w);
		setComponent(w, COMPONENT_KEY::WIDGET_TOUCH, comp);
	}
	comp->setTouchMovedCb(cb);
}

void setWidgetTouchHold(cocos2d::ui::Widget *w, const TouchHandlerFunc &cb, float delayCheckTime) {
	auto comp = getComponent<WidgetTouchComponent>(w, COMPONENT_KEY::WIDGET_TOUCH);
	if (!comp) {
		comp = new WidgetTouchComponent(w);
		setComponent(w, COMPONENT_KEY::WIDGET_TOUCH, comp);
	}
	comp->longTouchDelayCheckTime = delayCheckTime;
	comp->setTouchHoldCb(cb);
}

void setWidgetTouchCb(cocos2d::ui::Widget *w,
	const TouchHandlerFunc &beganCb,
	const TouchHandlerFunc &endedCb,
	const TouchHandlerFunc &movedCb,
	const TouchHandlerFunc &holdCb) 
{
	auto comp = getComponent<WidgetTouchComponent>(w, COMPONENT_KEY::WIDGET_TOUCH);
	if (!comp) {
		comp = new WidgetTouchComponent(w, beganCb, endedCb, movedCb, holdCb);
		setComponent(w, COMPONENT_KEY::WIDGET_TOUCH, comp);
		return;
	}
	comp->setTouchCb(beganCb, endedCb, movedCb, holdCb);
}

void setDragComponent(ui::Widget* widget, DragComponent *comp) {

	setComponent(widget, COMPONENT_KEY::DRAG, comp);

	// Them touch handler neu chua ton tai
	auto touchComp = getComponent<WidgetTouchComponent>(widget, COMPONENT_KEY::WIDGET_TOUCH);
	if (!touchComp)
	{
		touchComp = new WidgetTouchComponent(widget);
		setComponent(widget, COMPONENT_KEY::WIDGET_TOUCH, touchComp);
	}
}

void removeDragComponent(ui::Widget* widget) {
	removeComponent(widget, COMPONENT_KEY::DRAG);
}

void enableDragComponent(cocos2d::ui::Widget* widget) {
	auto comp = getComponent<DragComponent>(widget, COMPONENT_KEY::DRAG);
	if (comp) 
		comp->setEnabled(true);
}




WIDGET_TOUCH_NS_END
