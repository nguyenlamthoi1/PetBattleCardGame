#ifndef __CARD_DATA_H__
#define __CARD_DATA_H__

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
	virtual ~CardData();
	bool isValid() const { return !id.empty(); }

	CardId id;
	Type type = Type::None;
protected:
	CardData(CardId id, Type t);
};

class PetCardData final : public CardData{
public:
	PetCardData(CardId id = "");
	~PetCardData();

	std::string name;
	size_t hp = 0;
	std::string petImg;
};

#endif // __CARD_DATA_H__