#include "TitleCharacter.h"
#include "Tchar.h"
#include "../ResourceServer.h"
#include "../Use/Parameter.h"
#include "../Use/MTRandom.h"
#include "DxLib.h"

TitleCharacter::TitleCharacter() : ObjectBase(TYPE::TitleCharcter) {

	_point = { 0,0 };
	_delay = -1;
	_alpha = 0;

	_start = false;
	_isFloat = false;
	_floatUp = false;
	_isEnd = false;
}

TitleCharacter::~TitleCharacter(){
}

bool TitleCharacter::Init(CharKind charkind, int delay) {

	const TCHAR* fileName = nullptr;

	switch (charkind){
	case TitleCharacter::CharKind::MA:
		fileName = _T("png/back/title_ma.png");
		//_point = Parameter::_TitleMA;
		break;

	case TitleCharacter::CharKind::JI:
		fileName = _T("png/back/title_ji.png");
		//_point = Parameter::_TitleJI;
		break;

	case TitleCharacter::CharKind::KA:
		fileName = _T("png/back/title_ka.png");
		//_point = Parameter::_TitleKA;
		break;

	case TitleCharacter::CharKind::RU:
		fileName = _T("png/back/title_ru.png");
		//_point = Parameter::_TitleRU;
		break;

	case TitleCharacter::CharKind::NA:
		fileName = _T("png/back/title_na.png");
		//_point = Parameter::_TitleNA;
		break;

	case TitleCharacter::CharKind::NN:
		fileName = _T("png/back/title_nn.png");
		//_point = Parameter::_TitleNN;
		break;

	case TitleCharacter::CharKind::BA:
		fileName = _T("png/back/title_ba.png");
		//_point = Parameter::_TitleBA;
		break;

	case TitleCharacter::CharKind::AA:
		fileName = _T("png/back/title_aa.png");
		//_point = Parameter::_TitleAA;
		break;

	default:
		return false;
	}

	int handle = ResourceServer::LoadGraph(fileName);   //ロードして識別番号をゲット

	if (handle == -1) {   //ハンドルに入っていないか確かめる
		return false;
	}

	DrawInfo info = { handle,{_point.x,Parameter::_ScreenHeight},false };

	_vDrawInfo.push_back(info);

	_delay = delay;

	return true;
}

void TitleCharacter::Process() {

	if (_isEnd) {
		ProcessEnding();
	}
	else if (_isFloat) {
		ProcessFloating();
	}
	else {
		ProcessDelay();
	}
}

void TitleCharacter::SetFloatMode() {

	if (_isFloat) {
		return;
	}

	_isFloat = true;

	_floatUp = MTRandom::Get();

	SetFloating();
}

void TitleCharacter::SetEndMode() {

	if (_isEnd) {
		return;
	}

	_isEnd = true;

	SetEnding();
}

void TitleCharacter::ProcessDelay() {

	if (_delay < 0) {
		return;
	}

	if (_delay <= 0) {
		if (!_start) {
			_start = SetStart();
		}

		_tween.Process();

		if (_tween.IsStart()) {
			Point xy = _tween.GetPosition();

			SetXY(xy.x, xy.y);

			float alpha = 255.0f * _tween.GetRate();

			_alpha = static_cast<int>(alpha);

		}
	}
	else {
		_delay--;
	}
}

void TitleCharacter::ProcessFloating() {

	_tween.Process();

	if (_tween.IsStart()) {
		Point xy = _tween.GetPosition();

		SetXY(xy.x, xy.y);
	}

	if (_tween.IsEnd()) {
		SetFloating();
	}
}

void TitleCharacter::ProcessEnding() {

	_tween.Process();

	if (_tween.IsStart()) {
		Point xy = _tween.GetPosition();

		SetXY(xy.x, xy.y);
	}
}

void TitleCharacter::SetFloating() {

	int randam = MTRandom::Get(30, 70);  //縦の揺れる幅
	Point start = GetXY();
	Point end = _point;

	end.y += _floatUp ? -randam : randam;
	_floatUp = !_floatUp;

	int frame = MTRandom::Get(200, 240);   //時間動くスピード
	Tween::Type type = MTRandom::Get() ? Tween::Type::SineStart : Tween::Type::SineEnd;
	//Tween::Type type = Tween::Type::SineStart;

	_tween.Reset();
	_tween.SetTween(start, end, frame, type);

}

void TitleCharacter::SetEnding() {

	Point start = GetXY();
	Point end = {start.x,Parameter::_ScreenHeight};

	_tween.Reset();
	_tween.SetTween(start, end, Parameter::_TitleEndFrame, Tween::Type::SineEnd);
}

bool TitleCharacter::SetStart() {

	SetDraw(true);

	Point point = GetXY();
	_tween.SetTween(point, _point, Parameter::_TitleCharcterProcessFrame, Tween::Type::SineStart);

	return true;
}

void TitleCharacter::DeleteImage() {

	if (_vDrawInfo.size() == 0) {
		return;
	}

	DrawInfo drawInfo = _vDrawInfo.front();

	ResourceServer::DeleteGraph(drawInfo.handle);

	_vDrawInfo.clear();
}