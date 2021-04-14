#include "Select.h"

Select::Select(TYPE type):ObjectBase(type) {   //‰E‚©‚ç¶‚Éˆø”‚ğó‚¯—¬‚·

	_selectIndex = -1;
}

Select::~Select() {
}

void Select::SetDraw(bool isDraw) {

	for (int i = 0; i < _vDrawInfo.size();i++) {
		if (i == _selectIndex && isDraw) {   //select—p‚Ì‰æ‘œ‚¾‚Á‚½ê‡
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

	_selectIndex = _vDrawInfo.size();   //protected‚Í‚»‚Ì‚Ü‚Üg‚¦‚é

	_vDrawInfo.push_back(info);   //size‚Å‚Æ‚ê‚½‚Æ‚±‚ë‚É’Ç‰Á
}
