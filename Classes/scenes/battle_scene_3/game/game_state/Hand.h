#pragma once

#include "../GameDefine.h"

#include <vector>

class CardData;

NS_GAME_BEGIN

class Card;

class Hand {
public:
	using CardVector = std::vector<std::shared_ptr<Card>>;
	using HandPtr = std::shared_ptr<Hand>;
	
	static HandPtr createShPtr(const PlayerIdType &id);
	Hand(const PlayerIdType &id);
	virtual ~Hand();
	bool init();
	
	void clear();
	bool isEmpty() const;
	void pushCards(const CardVector &pushedCards);
	void pushCard(const std::shared_ptr<Card> &pushedCard);

	HandPtr clone() const;
protected:
	PlayerIdType pid = PlayerIdInvalid;
	CardVector cardVec;
};

NS_GAME_END