#include "Hp.h"
#include <array>
#include "Tchar.h"
#include "../Use/Parameter.h"
#include "DxLib.h"
#include "../ResourceServer.h"
#include <cmath>

Hp::Hp(bool is1P):ObjectBase(TYPE::HP) {

	_point = { 0,0 };
	_leftUp = { 0,0 };
	_rightDown = { 0,0 };

	_hp = 0;
	_nowHp = 0;
	_nextHp = 0;
	_handle = -1;
	_imageWidth = 0;
	_imageHeight = 0;

	_is1P = is1P;
}

Hp::~Hp() {
}

bool Hp::Init() {

	int handle = ResourceServer::LoadGraph(_T("png/hp/hp.png"));
	
	_handle = ResourceServer::LoadGraph(_T("png/hp/hpInner.png"));

	if (handle == -1 || _handle == -1) {
		return false;
	}

	if (GetGraphSize(_handle, &_imageWidth , &_imageHeight) == -1) {
		return false;
	}

	_point = _is1P ? Parameter::_HpPosition1P : Parameter::_HpPosition2P;

	_leftUp = _point;
	_leftUp.x += Parameter::_HpImageMarginLeft;

	_rightDown = _leftUp;
	_rightDown.x += _imageWidth - (Parameter::_HpImageMarginLeft + Parameter::_HpImageMarginRight);
	_rightDown.y += _imageHeight;

	DrawInfo drawInfo;

	drawInfo.handle = handle;
	drawInfo.xy = _point;
	drawInfo.isDraw = false;

	_vDrawInfo.push_back(drawInfo);

	return true;
}

void Hp::Process() {

	_tween.Process();

	if (_tween.IsStart()) {
		_rightDown = _tween.GetPosition();
	}

	if (_tween.IsEnd()) {
		_nowHp = _nextHp;
	}
}

void Hp::Draw() {

	ObjectBase::Draw();

	if (_handle == -1) {
		return;
	}

	SetDrawArea(_leftUp.x, _leftUp.y, _rightDown.x, _rightDown.y);
	DrawGraph(_point.x, _point.y, _handle, TRUE);
	SetDrawArea(0, 0, Parameter::_ScreenWidth, Parameter::_ScreenHeight);
}

void Hp::SetHp(int hp) {

	if (hp == _nowHp) {
		return;
	}

	_nextHp = hp;

	Point next = { 0,0 };

	GetHpArea(_nextHp, next);

	Point now = { 0,0 };

	GetHpArea(_nowHp, now);

	int diff = std::abs(_nowHp - _nextHp);

	_tween.Reset();
	_tween.SetTween(now, next, diff, Tween::Type::SineStart);
}

void Hp::GetHpArea(int hp, Point& rightDown) {

	int hpPixel = _imageWidth - Parameter::_HpImageMarginLeft - Parameter::_HpImageMarginRight;
	float rate = static_cast<float>(hp) / static_cast<float>(_hp);
	float hpRate = std::round(static_cast<float>(hpPixel) * rate);
	int nowHp = static_cast<int>(hpRate);

	rightDown = _leftUp;
	rightDown.x += nowHp;
	rightDown.y += _imageHeight;

}