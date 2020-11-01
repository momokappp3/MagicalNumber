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
		unsigned long id;   //DrawInfo�����������Ƃ��T���ď������肷��
		std::array<int, DigitMax> aDigit;
		std::array<Point, DigitMax> aPoint;
		bool isDraw;
	};

	//�C���X�^���X���ƂɕύX�ł���
	bool Init(std::array<const TCHAR*, ImageHandlNum> imageNames,float overlapRate = 0.0f);

	unsigned long SetNumber(int x, int y, int num, Anchor anchor = Anchor::LeftUp, unsigned long id = 0);
	bool DeleteNumber(unsigned long id);   //SetNumber�̖߂�l
	void Draw();
	bool SetIsDraw(unsigned long id,bool isDraw);
	

private:

	//&������ƎQ�Ɠn�������̂܂�c++�̐V���Ȏd�l
	void ConvertDigit(int num, std::array<int, DigitMax>& digit);
	void ConvertAnchor(int x, int y, int width, int height, Anchor anchor, Info& info);
	void Draw(Info info);

	std::array<int, ImageHandlNum> _aImageHandles;   //�摜�̎��ʔԍ�
	std::vector<Info> _vDrawInfo;

	float _overlapRate;
	bool _minus;

};
