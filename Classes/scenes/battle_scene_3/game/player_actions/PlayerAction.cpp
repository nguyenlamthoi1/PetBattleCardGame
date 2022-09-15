#include "PlayerAction.h"

using namespace std;

NS_GAME_BEGIN

/*
	Player lua chon
*/

PA_SelectPrizeCards::PA_SelectPrizeCards(const PlayerIdType &id, initializer_list<unsigned int> vec) : 
	pid(id),
	idxVec(vec.begin(), vec.end())
{
}

PA_SelectPrizeCards::PA_SelectPrizeCards(const PlayerIdType &id, const vector<unsigned int> &vec) :
	pid(id),
	idxVec(vec.begin(), vec.end())
{
}

NS_GAME_END