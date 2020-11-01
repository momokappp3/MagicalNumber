#pragma once
#include <array>
#include <vector>
#include <tchar.h>
#include "Types.h"

class DrawNumber {
public:
	DrawNumber();
	virtual ~DrawNumber();

	static constexpr int ImageHandlNum = 10;
	static constexpr int DigitMax = 5;

	enum class Anchor {
		LeftUp, CenterUp ,RightUp,
		LeftCenter, CenterCenter, RightCenter,
		LeftDown, CenterDown, RightDown
	};

	struct Info {
		unsigned long id;   //DrawInfoを消したいとき探して消したりする
		std::array<int, DigitMax> aDigit;
		std::array<Point, DigitMax> aPoint;
		bool isDraw;
	};

	//インスタンスごとに変更できる
	bool Init(std::array<const TCHAR*, ImageHandlNum> imageNames,float overlapRate = 0.0f);

	unsigned long SetNumber(int x, int y, int num, Anchor anchor = Anchor::LeftUp, unsigned long id = 0);
	bool DeleteNumber(unsigned long id);   //SetNumberの戻り値
	void Draw();
	bool SetIsDraw(unsigned long id,bool isDraw);
	

private:

	//&をつけると参照渡し引数のままc++の新たな仕様
	void ConvertDigit(int num, std::array<int, DigitMax>& digit);
	void ConvertAnchor(int x, int y, int width, int height, Anchor anchor, Info& info);
	void Draw(Info info);

	std::array<int, ImageHandlNum> _aImageHandles;   //画像の識別番号
	std::vector<Info> _vDrawInfo;

	float _overlapRate;
	bool _minus;

};
