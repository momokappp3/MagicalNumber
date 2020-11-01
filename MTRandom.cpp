#include "MTRandom.h"

namespace {                        //プログラムの実行するたび同じ値が表示されるのを防ぐために speed_gen
	std::random_device seed_gen;   //初期シード　乱数　時刻やCPUの温度でランダムを引っ張るのと同じ
	std::mt19937_64 random(seed_gen());   //ランダム作る　メルセンヌツイスター
}

MTRandom::MTRandom(){
}

MTRandom::~MTRandom(){
}

int MTRandom::Get(int min, int max) {

	//下限aおよび上限bを受け取るコンストラクタ。a以上b以下(範囲[a, b])の値が生成される
	std::uniform_int_distribution<int>  distr(min,max);

	return distr(random);   //minからmaxまでのランダムが取れる
}

std::mt19937_64 MTRandom::GetRandom() {

	return random;
}

