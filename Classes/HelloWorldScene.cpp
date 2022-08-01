#include "HelloWorldScene.h"
#include "ui/UILayout.h"
#include "ui/UIWidget.h"
#include "components/WidgetTouchComponent.h"
#include "common/Utilize.h"


USING_NS_CC;
UTILIZE_USE_NS;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
/*bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
	//sprite->autorelease();
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
	test();

    return true;
}*/

bool HelloWorld::init() {
	if (!Layer::init())
		return false;
	auto dir = Director::getInstance();
	auto vSize = dir->getVisibleSize();

	auto parentLayout = ui::Layout::create();
	parentLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	parentLayout->setBackGroundColor(Color3B::WHITE);
	parentLayout->setContentSize(Size(800, 800));
	parentLayout->setPosition(Vec2::ZERO);
	this->addChild(parentLayout);

	// L1
	auto l1 = ui::Layout::create();
	l1->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	l1->setBackGroundColor(Color3B::RED);
	l1->setBackGroundColorOpacity(255 / 2);
	l1->setContentSize(Size(400, 400));
	l1->setPosition(Vec2(0, 400));
	parentLayout->addChild(l1);

	// OBJ
	auto obj = ui::Layout::create();
	obj->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	obj->setBackGroundColor(Color3B::YELLOW);
	obj->setBackGroundColorOpacity(255 / 2);
	obj->setContentSize(Size(200, 200));
	obj->setPosition(Vec2(100, 100));
	obj->setAnchorPoint(Vec2(0.5, 0.5));
	obj->setName("Object");
	l1->addChild(obj);

	// L2
	auto l2 = ui::Layout::create();
	l2->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	l2->setBackGroundColor(Color3B::BLUE);
	l2->setBackGroundColorOpacity(255 / 2);
	l2->setContentSize(Size(400, 400));
	l2->setPosition(Vec2(400, 0));
	l2->setName("L2");
	parentLayout->addChild(l2);

	// L3
	auto l3 = ui::Layout::create();
	l3->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	l3->setBackGroundColor(Color3B::GREEN);
	l3->setBackGroundColorOpacity(255 / 2);
	l3->setContentSize(Size(400, 400));
	l3->setPosition(Vec2(400, 400));
	l3->setName("L3");
	parentLayout->addChild(l3);

	auto dragComp = new DragComponent();
	dragComp->destinations.push_back(l2);
	dragComp->destinations.push_back(l3);
	dragComp->useCenter = false;

	MyComponentNS::setComponent(obj, COMPONENT_KEY::DRAG, dragComp);
	obj->setTouchEnabled(true);
	obj->addTouchEventListener([this](Ref *sender, ui::Widget::TouchEventType eventType) {
		auto widget = dynamic_cast<ui::Widget*>(sender);
		switch (eventType) {
		case ui::Widget::TouchEventType::BEGAN:
		{
			// Drag mode
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
			auto comp = MyComponentNS::getComponent<DragComponent>(widget, COMPONENT_KEY::DRAG);
			if (comp && comp->isDragging) {
				comp->isDragging = false;
				dragEnd(widget, comp->hitDestNode);
				comp->hitDestNode = nullptr;
			}

			break;
		}
		case ui::Widget::TouchEventType::MOVED: {

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
								dragOut(widget, comp->hitDestNode);
								comp->hitDestNode = nullptr;
							}

							if (!comp->hitDestNode || comp->hitDestNode != dNode) {
								// DRAG IN CALLBACK
								dragIn(widget, dNode);
								comp->hitDestNode = dNode;
							}

							break;
						}
						else {
							if (comp->hitDestNode && comp->hitDestNode == dNode) {
								// DRAG OUT CALLBACK
								dragOut(widget, dNode);
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

						beginCb(); // DRAG BEGIN CALLBACK
					}
				}
			}
			break;
		}
		}
		});


	return true;

}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

class A : public Ref {
public:
	virtual ~A() {
		CCLOG("A: dtor called");
	}
};
