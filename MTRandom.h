#pragma once
#include <random>

class MTRandom {
public:
	MTRandom();
	virtual ~MTRandom();

	static int Get(int min, int max);  //�w�肵�����l�̒��Ń����_����Ԃ�(int)
	static bool Get() {

		return Get(1, 2) == 1 ? true : false;
	}

	static std::mt19937_64 GetRandom();

};
