#ifndef __WIDGET_TOUCH_COMPONENT_H__
#define __WIDGET_TOUCH_COMPONENT_H__

#include "MyComponent.h"
#include "cocos2d.h"
#include <functional>

#define WIDGET_TOUCH_NS_BEGIN namespace WidgetTouchNS {
#define WIDGET_TOUCH_NS_END }
#define WIDGET_TOUCH_USE_NS using namespace WidgetTouchNS

#define WIDGET_TOUCH_HANDLER_PARAMS cocos2d::ui::Widget*, cocos2d::Touch*, cocos2d::Event*
#define NODE_TOUCH_HANDLER_PARAMS cocos2d::Node*, cocos2d::Touch*, cocos2d::Event*
#define define_TouchHandlerFunc_Type using TouchHandlerFunc = std::function<void(WIDGET_TOUCH_HANDLER_PARAMS)>
#define define_NodeTouchHandlerFunc_Type using NodeTouchHandlerFunc = std::function<void(NODE_TOUCH_HANDLER_PARAMS)>

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


//class NodeTouchComponent : public MComponent {
//public:
//	define_NodeTouchHandlerFunc_Type;
//	NodeTouchComponent(cocos2d::Node *n,
//		const NodeTouchHandlerFunc& beganFunc = nullptr,
//		const NodeTouchHandlerFunc& endedFunc = nullptr,
//		const NodeTouchHandlerFunc& movedFunc = nullptr,
//		const NodeTouchHandlerFunc& holdFunc = nullptr);
//	virtual ~NodeTouchComponent();
//
//	virtual bool isValid() const override { return node != nullptr; }
//	void setTouchBeganCb(const NodeTouchHandlerFunc& cb) { beganCb = cb; }
//	void setTouchEndedCb(const NodeTouchHandlerFunc& cb) { endedCb = cb; }
//	void setTouchMovedCb(const NodeTouchHandlerFunc& cb) { movedCb = cb; }
//	void setTouchHoldCb(const NodeTouchHandlerFunc& cb) { holdCb = cb; }
//	void setTouchCb(const NodeTouchHandlerFunc& beganFunc = nullptr,
//		const NodeTouchHandlerFunc& endedFunc = nullptr,
//		const NodeTouchHandlerFunc& movedFunc = nullptr,
//		const NodeTouchHandlerFunc& holdFunc = nullptr) {
//		beganCb = beganFunc;
//		endedCb = endedFunc;
//		movedCb = movedFunc;
//		holdCb = holdFunc;
//	}
//	float longTouchDelayCheckTime = 0.0f;
//	static const float TOUCH_MOVE_DELTA;
//	static const std::string NODE_HOLD_TOUCH_SCHEDULE_KEY;
//protected:
//	EventListenerTouchOneByOne *eventListener;
//	bool swallowTouches = true;
//
//	cocos2d::Node *node = nullptr;
//	NodeTouchHandlerFunc beganCb;
//	NodeTouchHandlerFunc endedCb;
//	NodeTouchHandlerFunc movedCb;
//	NodeTouchHandlerFunc holdCb;
//	bool checkingLongTouch = false;
//	cocos2d::Vec2 beganTouchPos = cocos2d::Vec2::ZERO;
//
//	virtual void applyHandler();
//};


class DragComponent : public MComponent {
public:
	friend class WidgetTouchComponent;
	friend class HelloWorld;
	define_DragHandlerFunc_Type;

	static std::shared_ptr<DragComponent> getShComp(cocos2d::Node *node);
	static DragComponent* getComp(cocos2d::Node *node);
	static void setComp(cocos2d::Node *node, DragComponent* comp);


	DragHandlerFunc dragBeginCallback; // Function duoc goi khi bat dau drag object
	DragHandlerFunc dragEndCallback; // Function duoc goi khi release Touch luc dang drag object
	DragHandlerFunc dragInCallback; // Function duoc goi khi drag object den 1 dest node
	DragHandlerFunc dragOutCallback; // Function duoc goi khi drag object ra khoi 1 dest node
	std::vector<cocos2d::Node*> destinations; // Danh sach cac node muc tieu(dest node) khi drag object

	float zInContainer = 999; // Local z cua object khi duoc add child vao dragContainer

	cocos2d::Node* draggedObj = nullptr;
	/* 
	useCenter la T -> Thi center point cua object duoc dung kiem tra object drag den 1 dest node 
	useCenter la F -> Thi touch point cua object duoc dung kiem tra object drag den 1 dest node
	*/

	DragComponent() = default;
	~DragComponent();
	bool isDroppable() { return hitDestNode != nullptr; }

	// Setters
	void setUseCenter(bool use) { useCenter = use; }
	void setEnabled(bool e) { enabled = e; }
	void setDragContainer(cocos2d::Node *container) { dragContainer = container; }

	// Getters
	cocos2d::Vec2 getOrgWorldPos() { return orgWorldPos; }
	cocos2d::Node* getOrgParent() { return orgParent; }
	cocos2d::Node* getHitDestNode() { return hitDestNode; }
	float getOrgZ() { return orgZ; }
	bool isObjDragging() { return isDragging; }
	bool isEnabled() { return enabled; }
protected:
	bool useCenter = true;
	cocos2d::Node* dragContainer = nullptr; // Khi node duoc drag, node se duoc removeFromParent va addChild vao dragContainer
	bool isDragging = false;
	cocos2d::Vec2 orgPos;
	float orgZ;
	cocos2d::Vec2 orgWorldPos;
	cocos2d::Vec2 offset;
	cocos2d::Node* orgParent = nullptr;
	cocos2d::Node* hitDestNode = nullptr;

	bool enabled = true;
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
void enableDragComponent(cocos2d::ui::Widget* widget);

WIDGET_TOUCH_NS_END

#endif // __WIDGET_TOUCH_COMPONENT_H__