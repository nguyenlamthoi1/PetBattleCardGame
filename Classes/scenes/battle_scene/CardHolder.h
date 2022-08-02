#ifndef __CARD_HOLDER_H__
#define __CARD_HOLDER_H__

#include "BSDefine.h"
#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>

BATTLE_SCENE_NS_BEG

class CardHolder : public cocos2d::ui::Layout {
public:
	static CardHolder* create();
	CardHolder();
	virtual ~CardHolder();
	virtual bool init() override;

protected:
	cococs2d::Node *node;
};

BATTLE_SCENE_NS_END


#endif // __CARD_HOLDER_H__
