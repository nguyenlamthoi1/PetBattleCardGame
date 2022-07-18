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


void WidgetTouchComponent::applyHandler() {
	CCASSERT(widget != nullptr, "WidgetTouchComponent::applyHandler: widget is NULL");

	auto func = [this](Ref *sender, ui::Widget::TouchEventType eventType)->void {
		switch (eventType) {
		case ui::Widget::TouchEventType::BEGAN:
		{
			if (beganCb)
				beganCb(widget, nullptr, nullptr);

			if (holdCb) {
				//_isCheckingLongTouch = false;
				//beganTouchPos.x = widget->getTouchBeganPosition().x;
				//beganTouchPos.y = widget->getTouchBeganPosition().y;
				//cocos2d::Director::getInstance()->getScheduler()->schedule([this, widget](float dt)
				//	{
				//		_isCheckingLongTouch = true;

				//		Vec2 touchStartLoc = beganTouchPos;
				//		Vec2 touchCurLoc = widget->getTouchBeganPosition();
				//		float distance = touchStartLoc.distance(touchCurLoc);
				//		if (distance < COMPONENT_CONST::TOUCH_MOVE_DELTA) { // Touch is not determined being moved 
				//			if (_touchHoldCallback)
				//				_touchHoldCallback(widget, nullptr, nullptr);
				//		}
				//	}, this, 0, 0, _longTouchDelayCheck, false, COMPONENT_CONST::HOLD_TOUCH_SCHEDULE_KEY);
			}
			break;
		}
		case ui::Widget::TouchEventType::ENDED:
		case ui::Widget::TouchEventType::CANCELED:
		{
			/*if (_touchHoldCallback && !_isCheckingLongTouch)
				cocos2d::Director::getInstance()->getScheduler()->unschedule(COMPONENT_CONST::HOLD_TOUCH_SCHEDULE_KEY, this);*/

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

void setWidgetTouchHold(cocos2d::ui::Widget *w, const TouchHandlerFunc &cb) {
	auto comp = getComponent<WidgetTouchComponent>(w, COMPONENT_KEY::WIDGET_TOUCH);
	if (!comp) {
		comp = new WidgetTouchComponent(w);
		setComponent(w, COMPONENT_KEY::WIDGET_TOUCH, comp);
	}
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
