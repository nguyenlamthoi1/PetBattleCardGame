#ifndef __WIDGET_TOUCH_COMPONENT_H__
#define __WIDGET_TOUCH_COMPONENT_H__

#include "MyComponent.h"
#include "cocos2d.h"
#include <functional>

#define WIDGET_TOUCH_NS_BEGIN namespace WidgetTouchNS {
#define WIDGET_TOUCH_NS_END }
#define WIDGET_TOUCH_USE_NS using namespace WidgetTouchNS

#define WIDGET_TOUCH_HANDLER_PARAMS cocos2d::ui::Widget*, cocos2d::Touch*, cocos2d::Event*
#define define_TouchHandlerFunc_Type using TouchHandlerFunc = std::function<void(WIDGET_TOUCH_HANDLER_PARAMS)>

#define WIDGET_DRAG_HANDLER_PARAMS cocos2d::Node*, cocos2d::Node*
#define define_DragHandlerFunc_Type using DragHandlerFunc = std::function<void(WIDGET_DRAG_HANDLER_PARAMS)>

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
	float longTouchDelayCheckTime = 0.0f;

protected:
	cocos2d::ui::Widget *widget = nullptr;
	TouchHandlerFunc beganCb;
	TouchHandlerFunc endedCb;
	TouchHandlerFunc movedCb;
	
	TouchHandlerFunc holdCb;
	bool checkingLongTouch = false;
	static const float TOUCH_MOVE_DELTA;
	static const std::string HOLD_TOUCH_SCHEDULE_KEY;

	cocos2d::Vec2 beganTouchPos = cocos2d::Vec2::ZERO;

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


class DragComponent : public MComponent {
public:
	define_DragHandlerFunc_Type;

	bool isDragging;
	float orgX, orgY, orgZ;
	float orgWorldX, orgWorldY;
	float offX, offY;
	cocos2d::Node* orgParent;
	std::string tag;
	bool snap;
	std::vector 
	/*
		true: Co the drop khi dang drag, false: Chi co the drop khi release touch
	*/
	//bool dropOnMove;

	DragHandlerFunc dropCallback;
	DragHandlerFunc dragBeginCallback;
	DragHandlerFunc dragEndCallback;

	DragComponent(const std::string& _tag, 
		const DragHandlerFunc& _dropCallback, 
		const DragHandlerFunc& _dragBeginCallback, 
		const DragHandlerFunc& _dragEndCallback, 
		bool _dropOnMove, bool _snap) : 
		tag(_tag), dragBeginCallback(_dragBeginCallback), 
		dragEndCallback(_dragEndCallback), 
		dropCallback(_dropCallback), 
		//dropOnMove(_dropOnMove), 
		snap(_snap) {}
};




WIDGET_TOUCH_NS_BEGIN

define_TouchHandlerFunc_Type;

void setWidgetTouchBegan(cocos2d::ui::Widget *w, const TouchHandlerFunc &f);
void setWidgetTouchEnded(cocos2d::ui::Widget *w, const TouchHandlerFunc &f);
void setWidgetTouchMoved(cocos2d::ui::Widget *w, const TouchHandlerFunc &f);
void setWidgetTouchHold(cocos2d::ui::Widget *w, const TouchHandlerFunc &f, float delayCheckTime = 0.0f);

void setWidgetTouchCb(cocos2d::ui::Widget *w, 
	const TouchHandlerFunc &beganCb = nullptr,
	const TouchHandlerFunc &endedCb = nullptr,
	const TouchHandlerFunc &movedCb = nullptr,
	const TouchHandlerFunc &holdCb = nullptr);

define_DragHandlerFunc_Type;

void enableDrag(cocos2d::ui::Widget* widget,
	const std::string& tag,
	const DragHandlerFunc &dropCallback = nullptr,
	const DragHandlerFunc& dragBeginCallback = nullptr,
	const DragHandlerFunc& dragEndCallback = nullptr,
	bool dropOnMove = false,
	bool snap = true);

WIDGET_TOUCH_NS_END

#endif // __WIDGET_TOUCH_COMPONENT_H__