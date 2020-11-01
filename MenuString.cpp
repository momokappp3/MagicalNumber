#include "MenuString.h"
#include <tchar.h>
#include "ResourceServer.h"
#include "DxLib.h"
#include "Parameter.h"
#include <cmath>

MenuString::MenuString(){

	_center = { 0,0 };

	_handle = -1;
	_delay = 0;
	_imageWidth = 0.0f;
	_imageHeight = 0.0f;
	_scale = 0.0f;
}

MenuString::~MenuString() {
}

bool MenuString::Init(Kind kind, int delay) {

	if (kind >= Kind::Max || delay < 0) {
		return false;
	}

	const TCHAR* fileName = nullptr;

	switch (kind){
	case MenuString::Kind::Match:
		fileName = _T("png/menu/taisen.png");
		_center = Parameter::_MenuMatch;
		break;

	case MenuString::Kind::Help:
		fileName = _T("png/menu/herupu.png");
		_center = Parameter::_MenuHelp;
		break;

	case MenuString::Kind::EndGame:
		fileName = _T("png/menu/syuryo.png");
		_center = Parameter::_MenuEndGame;
		break;

	default:
		return false;
	}

	int handle = ResourceServer::LoadGraph(fileName);

	if (handle == -1) {
		return false;
	}

	_handle = handle;

	int width = 0, height = 0;

	if (GetGraphSize(_handle, &width, &height) == -1) {
		return false;
	}

	_imageWidth = static_cast<float>(width);
	_imageHeight = static_cast<float>(height);
	
	_delay = delay;

	return true;
}

void MenuString::Process() {

	if (_delay == 0) {
		_tween.Process();

		if (_tween.IsStart()) {
			Point xy = _tween.GetPosition();

			_scale = static_cast<float>(xy.x) / 100.0f;

		}

	}
	else {
		_delay--;

		if (_delay == 0) {
			SetTweenScale(0, 100, 20, Tween::Type::SineEnd);
		}
	}
}

void MenuString::Draw() {

	DrawScale(_scale);
}

void MenuString::DrawScale(float scale) {

	if(_handle == -1 || _imageWidth == 0.0f || _imageHeight == 0.0f || scale <= 0.0f){
		return;
	}

	float width = _imageWidth * scale;
	float height = _imageHeight * scale;
	float halfWidthF = std::round(width / 2.0f);
	float halfHeightF = std::round(height / 2.0f);
	int halfWidth = static_cast<int>(halfWidthF);
	int halfHeight = static_cast<int>(halfHeightF);

	DrawExtendGraph(_center.x - halfWidth, _center.y - halfHeight, _center.x + halfWidth, _center.y + halfHeight, _handle, TRUE);

}

void MenuString::SetTweenScale(int start, int end, int frame, Tween::Type type) {

	_tween.Reset();
	_tween.SetTween({ start, 0 }, { end, 0 }, frame, type);

}

void MenuString::DeleteImage() {

	if (_handle == -1) {
		return;
	}

	ResourceServer::DeleteGraph(_handle);

	_handle = -1;
}