#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "base_classes/EventsHandler/IEventsHandler.h"

#include "define/CommonDefine.h"
#include "cocos2d.h"
#include "ui/UIButton.h"
#include "ui/UIImageView.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"
#include "ui/UIListView.h"

#include <string>


class UnselectedCardHolder : public cocos2d::ui::Layout {
public:
	using CardId = std::string;
	static UnselectedCardHolder* create();
	UnselectedCardHolder();
	virtual ~UnselectedCardHolder();

	cocos2d::Node* getRoot() const { return root; }
	void renderWith(const CardId &cid, unsigned int stackedNum);

protected:
	virtual bool init() override;

	cocos2d::Node *root = nullptr;
	cocos2d::ui::Layout *container = nullptr;
	cocos2d::ui::ImageView *numPanel = nullptr;
	cocos2d::ui::Text *numLabel = nullptr;

	CardId cid;
	unsigned int stackedNum = 0;
};

class SelectedCardHolder : public cocos2d::ui::Layout {
public:
	using CardId = std::string;
	static SelectedCardHolder* create();
	SelectedCardHolder();
	virtual ~SelectedCardHolder();

	cocos2d::Node* getRoot() const { return root; }
protected:
	virtual bool init() override;

	cocos2d::Node *root = nullptr;
	cocos2d::ui::Layout *container = nullptr;
	cocos2d::ui::ImageView *numPanel = nullptr;
	cocos2d::ui::Text *numLabel = nullptr;

	CardId cid;
	unsigned int stackedNum = 0;
};

class CardSelector : public IEventsHandler
{
public:
	static const std::string UPDATE_SCHEDULER;
	static const std::string UPDATE_SCHEDULER2;


	friend class HelloWorld;
	using CardId = std::string;
	using SelectType = GConfig::SelectType;
	using SelectMap = std::map<SelectType, unsigned int>;

	/// All Events ///
	static const std::string ON_DONE_SELECT; // Event Data is Null

	using CardId = std::string;
	static std::shared_ptr<CardSelector> create(cocos2d::Node *root);
	CardSelector(cocos2d::Node *root);
	virtual ~CardSelector();

	void showToSelect(const std::vector<CardId> &cidVec, const SelectMap &selMap);
	void hide();
	void clearUI();
	void update(float t);


	cocos2d::Node* getRoot() const { return root; }
protected:

	virtual bool init();
	bool initUnselListView();
	bool initSelListView();

	cocos2d::Node *root = nullptr;

	// Unselected Panel
	cocos2d::ui::Layout *unselectedPanel = nullptr;
	cocos2d::ui::ListView* unselectedListView = nullptr;
	cocos2d::ui::Button *unselectedPrevBtn = nullptr;
	cocos2d::ui::Button *unselectedNextBtn = nullptr;
	std::vector<UnselectedCardHolder*> unSelHolderVec;

	cocos2d::ui::Layout *selectedPanel = nullptr;
	cocos2d::ui::ListView* selectedListView = nullptr;
	cocos2d::ui::Button *selectedPrevBtn = nullptr;
	cocos2d::ui::Button *selectedNextBtn = nullptr;
	std::vector<SelectedCardHolder*> selHolderVec;
};


class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void onEnter() override;
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	cocos2d::Node *selectorNode = nullptr;
	cocos2d::ui::Button *doBtn = nullptr;
	std::shared_ptr<CardSelector> selector;
	void doFunc1();
	void doFunc2();

};

#endif // __HELLOWORLD_SCENE_H__
