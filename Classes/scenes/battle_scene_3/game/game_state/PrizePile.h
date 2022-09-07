#pragma once

#include "../GameDefine.h"

#include <deque>
#include <vector>

class CardData;

NS_GAME_BEGIN

class GameState;
class Card;

class PrizePile {
public:
	using CardVector = std::vector<std::shared_ptr<Card>>;
	using PrizeIdx = unsigned int;
	using PrizePtr = std::shared_ptr<PrizePile>;

	static PrizePtr createShPtr(const PlayerIdType &id);

	PrizePile(const PlayerIdType &id);
	virtual ~PrizePile();
	bool init();

	void pushCards(const CardVector &pushedCards);
	void pushCard(const std::shared_ptr<Card> &pushedCard);

	void popCards(std::initializer_list<PrizeIdx> idxs, CardVector &cVec);

	void clear();
	bool empty() const;

	PrizePtr clone() const;
protected:
	PlayerIdType pid;
	CardVector cardVec;
};

NS_GAME_END