#ifndef __BS_NOTIFIER_H__
#define __BS_NOTIFIER_H__

#include "BSDefine.h"
#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"
#include "ui/UIButton.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>

BATTLE_SCENE_NS_BEG

class BattleScene;

class BSNotifier final {
public:
	friend class BattleScene;

	using DoneFunc = std::function<void()>;
	using CheckDoneFunc = std::function<bool()>;
	using OnCheckError = std::function<void()>;


	static BSNotifier* create(BattleScene *scn);
	BSNotifier(BattleScene *scn);
	~BSNotifier();

	cocos2d::Node* getNode() const { return root; }


private:
	bool init();

	BattleScene *btlScn = nullptr;
	cocos2d::Node *root = nullptr;
	cocos2d::ui::Layout *msgBoard = nullptr;
	cocos2d::ui::Button *doneBtn = nullptr;


public:
	void showMsg(const std::string &msg);
	void hideMsg(float afterT = 0);

	void showMsgAndHideAfter(const std::string &msg, float t);
	void showMsgWithDone(const std::string &msg, DoneFunc f, bool hideDoneOnClick = true);
	void showMsgWithDone(const std::string &msg, CheckDoneFunc checkFunc, DoneFunc doneFunc, OnCheckError errorFunc, bool hideOnSuccess = true);



	// Utilize
	void setMsg(const std::string &msg);
};

BATTLE_SCENE_NS_END

#endif // __BS_NOTIFIER_H__
