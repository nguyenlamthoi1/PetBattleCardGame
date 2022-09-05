#pragma once

class ClassB{
};

//namespace MGame {
//	class ClassA {};
//}
namespace MGame {
	class ClassA;
}

class MGame::ClassA {

};



//namespace MGame {
//	/*
//		- Class luu tru game state: deck, hand, board,...
//		- Dieu khien qua trinh choi
//		* Luu y khong ke thua class nay
//	*/
//	class BattleManager final {
//	//public:
//	//	BattleManager();
//	//	virtual ~BattleManager();
//
//	//	/*
//	//	- Khoi tao tran dau giua 2 nguoi choi
//	//	* P1, P2 khong phan biet thu tu
//	//	*/
//	//	bool initGame(const PlayerIdType &p1, const PlayerIdType &p2);
//	//private:
//	//	unsigned int curTurn = 0;
//	//	PlayerIdType curTurnId = PlayerIdInvalid;
//
//	};
//}
