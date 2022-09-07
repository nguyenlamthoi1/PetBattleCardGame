#ifndef __UTILIZE_H__
#define __UTILIZE_H__

#include "cocos2d.h"
#include "components/MyComponent.h"
#include <vector>
#include <memory>
#include <string>
#include <functional>

#define UTILIZE_NS_BEGIN namespace Utilize {
#define UTILIZE_NS_END }
#define UTILIZE_USE_NS using namespace Utilize

#define UTIL_STR_NS_BEGIN namespace mstr {
#define UTIL_STR_NS_END }

#define UTIL_VECTOR_BEGIN namespace mvector {
#define UTIL_VECTOR_END }

#define UTIL_NODE_BEGIN namespace mnode {
#define UTIL_NODE_END }

#define UTIL_RANDOM_BEGIN namespace mrandom {
#define UTIL_RANDOM_END }

//#define UTIL_NODE_NS_BEGIN namespace node {
//#define UTIL_NODE_NS_END }


UTILIZE_NS_BEGIN

UTIL_STR_NS_BEGIN

bool has_suffix(const std::string &s, const std::string &suffix);

UTIL_STR_NS_END


cocos2d::Node* getChildByMap(cocos2d::Node* root, const std::string &childName, const std::string &parentName);
cocos2d::Node* getChildByMap(cocos2d::Node* root, const std::string &name);
cocos2d::Node* getChildByName(cocos2d::Node* root, const std::string &name);

UTIL_NODE_BEGIN

cocos2d::Vec2 getLocalPos(cocos2d::Node *node, cocos2d::Node *localNode = nullptr);
cocos2d::Vec2 getWorldPos(cocos2d::Node *node);


UTIL_NODE_END

UTIL_VECTOR_BEGIN

// vector
template <typename T>
bool removeElement(std::vector<T>& vector, const T& element)
{
	auto it = std::find(vector.begin(), vector.end(), element);
	if (it != vector.end())
	{
		vector.erase(it);
		return true;
	}
	return false;
};

UTIL_VECTOR_END

UTIL_RANDOM_BEGIN

/**
 * @class RandomHelper
 * @brief A helper class for creating random number.
 */
	class CC_DLL RandomHelper {
	public:
		template<typename T>
		static inline T random_real(T min, T max) {
			std::uniform_real_distribution<T> dist(min, max);
			auto &mt = RandomHelper::getEngine();
			return dist(mt);
		}

		template<typename T>
		static inline T random_int(T min, T max) {
			std::uniform_int_distribution<T> dist(min, max);
			auto &mt = RandomHelper::getEngine();
			return dist(mt);
		}
	private:
		static std::mt19937 &getEngine();
};

/**
 * Returns a random value between `min` and `max`.
 */
template<typename T>
inline T random(T min, T max) {
	return RandomHelper::random_int<T>(min, max);
}

template<>
inline float random(float min, float max) {
	return RandomHelper::random_real(min, max);
}

template<>
inline long double random(long double min, long double max) {
	return RandomHelper::random_real(min, max);
}

template<>
inline double random(double min, double max) {
	return RandomHelper::random_real(min, max);
}

/**
 * Returns a random int between 0 and RAND_MAX.
 */
inline int random() {
	return cocos2d::random(0, RAND_MAX);
};

/**
 * Returns a random float between -1 and 1.
 * It can be seeded using std::srand(seed);
 */
inline float rand_minus1_1() {
	return ((std::rand() / (float)RAND_MAX) * 2) - 1;
	//    return cocos2d::random(-1.f, 1.f);
};

/**
 * Returns a random float between 0 and 1.
 * It can be seeded using std::srand(seed);
 */
inline float rand_0_1() {
	return std::rand() / (float)RAND_MAX;
	//    return cocos2d::random(0.f, 1.f);
};


UTIL_RANDOM_END

UTILIZE_NS_END
#endif // __UTILIZE_H__