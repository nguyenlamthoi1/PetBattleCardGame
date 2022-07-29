#ifndef __CARD_DATA_H__
#define __PLAYER_DATA_H__

#include <string>
#include <unordered_map>
#include <vector>

class CardData{
public:
	enum class Type {
		None,
		Pet,
		Energy,
		Supporter,
		Item
	};

	Type type = Type::None;
};

#endif // __PLAYER_DATA_H__