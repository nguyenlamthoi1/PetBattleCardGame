#pragma once

#include "../GameDefine.h"

NS_GAME_BEGIN

class GameState;
class Card;

class DiscardPile {
public:
	using CardVector = std::vector<std::shared_ptr<const Card>>;
	using DiscardIdx = unsigned int;
	using DiscardPtr = std::shared_ptr<DiscardPile>;

	static DiscardPtr createShPtr(const PlayerIdType &id);

	DiscardPile(const PlayerIdType &id);
	virtual ~DiscardPile();
	bool init();

	void pushCards(const CardVector &pushedCards);
	void pushCard(const std::shared_ptr<Card> &pushedCard);

	void popCards(std::initializer_list<DiscardIdx> idxs, CardVector &cVec);

	void clear();
	bool empty() const;

	DiscardPtr clone() const;
protected:
	PlayerIdType pid;
	CardVector cardVec;
};

NS_GAME_END
