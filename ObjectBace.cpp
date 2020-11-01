#include "ObjectBace.h"
#include "DxLib.h"

ObjectBase::ObjectBase(TYPE type) {
	
	_type = type;   //継承したクラスでしかタイプはわからないのでコンストラクタで渡す

	_value = InvalidNum;   //-1
	_vDrawInfo.clear();
}

ObjectBase::~ObjectBase() {
}

void ObjectBase::SetDraw(bool isDraw) {

	for (DrawInfo& info : _vDrawInfo) {    //c++の新しいfor　ベクターの中身がinfoに入って最後まで処理
		info.isDraw = isDraw;  //代入する場合は型を参照
	}
}

void ObjectBase::Process() {
} 

bool ObjectBase::SetXY(int x, int y) {

	if (_vDrawInfo.size() == 0) {
		return false;
	}

	for (int i = 0; i < _vDrawInfo.size(); i++) {
		_vDrawInfo[i].xy = { x,y };
	}
	return true;
}

bool ObjectBase::GetDrawInfo(int index, DrawInfo& info) {

	if (_vDrawInfo.size() == 0 || index < 0 || index >= _vDrawInfo.size()) {
		return false;
	}
	
	info = _vDrawInfo[index];
	return true;
}

void ObjectBase::Draw() {

	for (DrawInfo info : _vDrawInfo) {    //c++の新しいfor　ベクターの中身がinfoに入って最後まで処理
		if (!info.isDraw) {
			continue;
		}

		DrawGraph(info.xy.x, info.xy.y, info.handle, TRUE);
	}
}

Point ObjectBase::GetXY() {

	if (_vDrawInfo.size() == 0) {
		return { 0,0 };
	}

	return _vDrawInfo[0].xy;
}