#include "WidgetTouchComponent.h"
#include "ui/UIWidget.h"

USING_NS_CC;
MY_COMPONENT_USE_NS;
using namespace std;

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
			break;
		}
		case ui::Widget::TouchEventType::ENDED:
		case ui::Widget::TouchEventType::CANCELED:
		{
			if (holdCb && !checkingLongTouch)
				cocos2d::Director::getInstance()->getScheduler()->unschedule(HOLD_TOUCH_SCHEDULE_KEY, this);

			if (endedCb) 
				endedCb(widget, nullptr, nullptr);
			break;
		}
		case ui::Widget::TouchEventType::MOVED: {
			if (movedCb) 
				movedCb(widget, nullptr, nullptr);
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

WIDGET_TOUCH_NS_END
