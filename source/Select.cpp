#include "Select.h"

Select::Select(TYPE type):ObjectBase(type) {   //右から左に引数を受け流す

	_selectIndex = -1;
}

Select::~Select() {
}

void Select::SetDraw(bool isDraw) {

	for (int i = 0; i < _vDrawInfo.size();i++) {
		if (i == _selectIndex && isDraw) {   //select用の画像だった場合
			continue;
		}
		_vDrawInfo[i].isDraw = isDraw;
	}
}

bool Select::SetSelect(bool select) {

	if (_selectIndex == -1) {
		return false;
	}

	_vDrawInfo[_selectIndex].isDraw = select;

	return true;
}

bool Select::SetSelectXY(int x, int y) {

	if (_selectIndex == -1) {
		return false;
	}

	_vDrawInfo[_selectIndex].xy = {x,y};

	return true;
}

void Select::SetSelectDrawInfo(DrawInfo info) {

	_selectIndex = _vDrawInfo.size();   //protectedはそのまま使える

	_vDrawInfo.push_back(info);   //sizeでとれたところに追加
}
