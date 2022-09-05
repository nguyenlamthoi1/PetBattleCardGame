//#include "Holder.h"
//
//using namespace std;
//
//NS_GAME_BEGIN
//
//Holder::HolderPtr Holder::createShPtr(const PlayerIdType &id) {
//	auto ret = make_shared<Holder>(id);
//	if (!ret || !ret->init())
//		ret.reset(); // nullptr
//	return ret;
//}
//
//Holder::Holder(const PlayerIdType &id) : pid(id) {}
//
//Holder::~Holder(){}
//
//bool Holder::init() {
//	return true;
//}
//
//NS_GAME_END