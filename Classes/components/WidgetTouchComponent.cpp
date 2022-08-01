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
			if (comp) {
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
			if (comp && comp->isDragging) {
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
			if (comp) {
				if (comp->isDragging) {
					// Cap nhat lai thong tin cua Drag
					if (!comp->useCenter)
						widget->setPosition(widget->getTouchMovePosition() - comp->offset);
					else
						widget->setPosition(widget->getTouchMovePosition());

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
					if (d > 24) {
						comp->isDragging = true;
						widget->removeFromParent();
						if (!comp->dragContainer) {
							auto curScene = Director::getInstance()->getRunningScene();
							curScene->addChild(widget, 9999);
						}
						else {
							comp->dragContainer->addChild(widget);
						}
						if (!comp->useCenter)
							widget->setPosition(widget->getTouchMovePosition() - comp->offset);
						else
							widget->setPosition(widget->getTouchMovePosition());

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



WIDGET_TOUCH_NS_END
