#ifndef __WIDGET_TOUCH_COMPONENT_H__
#define __WIDGET_TOUCH_COMPONENT_H__

#include "MyComponent.h"
#include "cocos2d.h"
#include <functional>

#define WIDGET_TOUCH_NS_BEGIN namespace WidgetTouchNS {
#define WIDGET_TOUCH_NS_END }
#define WIDGET_TOUCH_USE_NS using namespace WidgetTouchNS
#define define_TouchHandlerFunc_Type using TouchHandlerFunc = std::function<void(cocos2d::ui::Widget*, cocos2d::Touch*, cocos2d::Event*)>
#define WIDGET_TOUCH_HANDLER_PARAMS cocos2d::ui::Widget*, cocos2d::Touch*, cocos2d::Event*
class WidgetTouchComponent : public MComponent {
public:
	define_TouchHandlerFunc_Type;
	WidgetTouchComponent(cocos2d::ui::Widget *w, 
		const TouchHandlerFunc& beganFunc = nullptr, 
		const TouchHandlerFunc& endedFunc = nullptr, 
		const TouchHandlerFunc& movedFunc = nullptr, 
		const TouchHandlerFunc& holdFunc = nullptr);
	virtual ~WidgetTouchComponent();
	virtual bool isValid() const override { return widget != nullptr; }
	void setTouchBeganCb(const TouchHandlerFunc& cb) { beganCb = cb; }
	void setTouchEndedCb(const TouchHandlerFunc& cb) { endedCb = cb; }
	void setTouchMovedCb(const TouchHandlerFunc& cb) { movedCb = cb; }
	void setTouchHoldCb(const TouchHandlerFunc& cb) { holdCb = cb; }
	void setTouchCb(const TouchHandlerFunc& beganFunc = nullptr,
		const TouchHandlerFunc& endedFunc = nullptr,
		const TouchHandlerFunc& movedFunc = nullptr,
		const TouchHandlerFunc& holdFunc = nullptr) {
		beganCb = beganFunc;
		endedCb = endedFunc;
		movedCb = movedFunc;
		holdCb = holdFunc;
	}
protected:
	cocos2d::ui::Widget *widget = nullptr;
	TouchHandlerFunc beganCb;
	TouchHandlerFunc endedCb;
	TouchHandlerFunc movedCb;
	TouchHandlerFunc holdCb;

	virtual void applyHandler();
//	//Special touch callbacks
//	bool _isCheckingLongTouch = false;
//	float _longTouchDelayCheck = 0.0f;
//	cocos2d::Vec2 beganTouchPos = cocos2d::Vec2::ZERO;
//	CommonTouchCallback _touchHoldCallback = nullptr;
//
//	bool _swallowTouches = true;
//	bool _forceTouchEnd = false;
//	bool _checkEndedInsideNode = true;
//
//public:
//	OneTouchHandlerComponent(
//		const TouchBeganCallback& touchBeganCallback,
//		const CommonTouchCallback& touchEndedCallback,
//		const CommonTouchCallback& touchMovedCallback,
//		/*const CommonTouchCallback& touchCancelledCallback,*/
//		const CommonTouchCallback& longTouchCallback = nullptr,
//		bool swallowTouches = true
//	);
//	void apply(cocos2d::Node* node);
//	void apply(cocos2d::ui::Widget* widget);
//
//	void cleanup();
//
//	void setLongTouchDelay(float longTouchDelayCheck) { _longTouchDelayCheck = cocos2d::clampf(longTouchDelayCheck, 0.0f, COMPONENT_CONST::HOLD_TOUCH_DELAY); }
//
//	void setTouchBeganCallback(const TouchBeganCallback& touchBeganCallback) { _touchBeganCallback = touchBeganCallback; }
//	void setTouchEndedCallback(const CommonTouchCallback& touchEndedCallback) { _touchEndedCallback = touchEndedCallback; }
//	void setTouchMovedCallback(const CommonTouchCallback& touchMovedCallback) { _touchMovedCallback = touchMovedCallback; }
//	void setTouchHoldCallback(const CommonTouchCallback& touchHoldCallback) { _touchHoldCallback = touchHoldCallback; }
//
//	void setSwallowTouches(bool swallowTouches);
//	bool isSwallowTouches();
//	static void cleanupTouchHandler(cocos2d::Node* node);

};

WIDGET_TOUCH_NS_BEGIN
define_TouchHandlerFunc_Type;
void setWidgetTouchBegan(cocos2d::ui::Widget *w, const TouchHandlerFunc &f);
void setWidgetTouchEnded(cocos2d::ui::Widget *w, const TouchHandlerFunc &f);
void setWidgetTouchMoved(cocos2d::ui::Widget *w, const TouchHandlerFunc &f);
void setWidgetTouchHold(cocos2d::ui::Widget *w, const TouchHandlerFunc &f);

void setWidgetTouchCb(cocos2d::ui::Widget *w, 
	const TouchHandlerFunc &beganCb = nullptr,
	const TouchHandlerFunc &endedCb = nullptr,
	const TouchHandlerFunc &movedCb = nullptr,
	const TouchHandlerFunc &holdCb = nullptr);

WIDGET_TOUCH_NS_END

#endif // __WIDGET_TOUCH_COMPONENT_H__