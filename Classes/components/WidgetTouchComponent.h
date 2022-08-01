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
};


class DragComponent : public MComponent {
public:
	friend class WidgetTouchComponent;
	friend class HelloWorld;
	define_DragHandlerFunc_Type;

	DragHandlerFunc dragBeginCallback; // Function duoc goi khi bat dau drag object
	DragHandlerFunc dragEndCallback; // Function duoc goi khi release Touch luc dang drag object
	DragHandlerFunc dragInCallback; // Function duoc goi khi drag object den 1 dest node
	DragHandlerFunc dragOutCallback; // Function duoc goi khi drag object ra khoi 1 dest node
	std::vector<cocos2d::Node*> destinations; // Danh sach cac node muc tieu(dest node) khi drag object
	cocos2d::Node* dragContainer = nullptr; // Khi node duoc drag, node se duoc removeFromParent va addChild vao dragContainer
	cocos2d::Node* draggedObj = nullptr;
	/* 
	useCenter la T -> Thi center point cua object duoc dung kiem tra object drag den 1 dest node 
	useCenter la F -> Thi touch point cua object duoc dung kiem tra object drag den 1 dest node
	*/
	bool useCenter = false;

	DragComponent() = default;
	~DragComponent();
	bool isDroppable() { return hitDestNode != nullptr; }
protected:
	bool isDragging = false;
	cocos2d::Vec2 orgPos;
	float orgZ;
	cocos2d::Vec2 orgWorldPos;
	cocos2d::Vec2 offset;
	cocos2d::Node* orgParent = nullptr;
	cocos2d::Node* hitDestNode = nullptr;
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

void setDragComponent(cocos2d::ui::Widget* widget, DragComponent *comp);
void removeDragComponent(cocos2d::ui::Widget* widget);

WIDGET_TOUCH_NS_END

#endif // __WIDGET_TOUCH_COMPONENT_H__