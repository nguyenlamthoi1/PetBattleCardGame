#ifndef __CARD_DATA_H__
#define __PLAYER_DATA_H__

#include <string>
#include <unordered_map>
#include <vector>

class CardData{
public:
	using CardId = std::string;

	enum class Type {
		None,
		Pet,
		Energy,
		Supporter,
		Item
	};
	CardData(CardId id, Type t);
	virtual ~CardData();
	bool isValid() { return !id.empty(); }

	CardId id;
	Type type = Type::None;
};

class PetCardData final : public CardData{
public:
	PetCardData(CardId id);
	~PetCardData();

	std::string name;
	size_t hp = 0;
	std::string petImg;
};

#endif // __PLAYER_DATA_H__