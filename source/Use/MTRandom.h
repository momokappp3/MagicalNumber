#pragma once
#include <random>

class MTRandom {
public:
	MTRandom();
	virtual ~MTRandom();

	static int Get(int min, int max);  //指定した数値の中でランダムを返す(int)
	static bool Get() {

		return Get(1, 2) == 1 ? true : false;
	}

	static std::mt19937_64 GetRandom();

};
