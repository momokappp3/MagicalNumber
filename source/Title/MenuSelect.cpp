#include "MenuSelect.h"
#include "../ResourceServer.h"
#include "../Use/Parameter.h"
#include "DxLib.h"

MenuSelect::MenuSelect() : MenuString() {

}

MenuSelect::~MenuSelect() {
}

bool MenuSelect::Init(Kind kind, int delay) {

	if (delay < 0) {
		return false;
	}

	int handle = ResourceServer::LoadGraph(_T("png/menu/botan_waku.png"));

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

	_center = Parameter::_MenuMatch;

	return true;
}

void MenuSelect::Process() {

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
			SetSelect(MenuString::Kind::Match);

		}
	}
}

bool MenuSelect::SetSelect(MenuString::Kind kind) {

	switch (kind) {
	case MenuString::Kind::Match:
		_center = Parameter::_MenuMatch;
		break;

	case MenuString::Kind::Help:
		_center = Parameter::_MenuHelp;
		break;

	case MenuString::Kind::EndGame:
		_center = Parameter::_MenuEndGame;
		break;

	default:
		return false;
	}

	SetTweenScale(120, 100, 20, Tween::Type::SineStart);

	return true;
}