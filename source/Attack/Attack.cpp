#include "Attack.h"
#include "../Use/Parameter.h"
#include "DxLib.h"

Attack::Attack(bool is1P) {

	_pDrawNumber = nullptr;
	_pVertical = nullptr;
	_pHorizontal = nullptr;

	_point = { 0,0 };

	_handle = 0;

	_ovalWidth = 0.0f;
	_ovalHeight = 0.0f;

	_waitStartCount = 0;
	_waitEndCount = 0;

	_isStart = false;
	_isAttack = false;
	_isDrawNumber = false;
	_is1P = is1P;
}

Attack::~Attack() {
}

bool Attack::Init(std::shared_ptr<SoundManager>& soundManager) {

	std::array<const TCHAR*, DrawNumber::ImageHandlNum> imageNames = {
	_T("png/number/answer0.png"), _T("png/number/answer1.png") ,
	_T("png/number/answer2.png"), _T("png/number/answer3.png") ,
	_T("png/number/answer4.png"), _T("png/number/answer5.png") ,
	_T("png/number/answer6.png"), _T("png/number/answer7.png") ,
	_T("png/number/answer8.png"), _T("png/number/answer9.png")
	};

	_pDrawNumber.reset(new DrawNumber);

	_point = _is1P ? Parameter::_attackNumberPotion1P : Parameter::_attackNumberPotion2P;

	bool drawNumberInit = _pDrawNumber->Init(imageNames);

	_pVertical.reset(new CutIn(_is1P));
	_pHorizontal.reset(new CutIn(_is1P));

	bool verticalInit = _pVertical->Init(CutIn::Type::Vertical, soundManager);
	bool horizontalInit = _pHorizontal->Init(CutIn::Type::Horizontal, soundManager);

	if (!drawNumberInit || !verticalInit || !horizontalInit) {
		return false;
	}

	return true;
}

void Attack::Process() {

	_tween.Process();

	if (_tween.IsStart()) {

		Point point = _tween.GetPosition();
		_ovalWidth = static_cast<float>(point.x);
		_ovalHeight = _ovalWidth / 2.0f;
	}

	bool tweenIsEnd = _tween.IsEnd();

	if (tweenIsEnd && !_isDrawNumber) {
		_isDrawNumber = true;

		int attack = _isAttack ? Parameter::_attackNumberBace : 0;

		_handle = _pDrawNumber->SetNumber(_point.x, _point.y, attack, DrawNumber::Anchor::CenterCenter, _handle);
	}

	if (tweenIsEnd) {
		_waitEndCount++;
	}

	if (_isStart) {
		if (_waitStartCount < Parameter::_attackWaitStartFrame) {
			_waitStartCount++;

			if (_waitStartCount >= Parameter::_attackWaitStartFrame) {
				_tween.SetTween({ 0,0 }, { 200,0 }, 30, Tween::Type::SineStart);
			}
		}
	}
}

void Attack::Draw() {

	if (!_isStart) {
		return;
	}

	if (_ovalWidth >= 0.0f && _ovalHeight > 0.0f) {
		DrawOvalAA(static_cast<float>(_point.x), static_cast<float>(_point.y), _ovalWidth + 10.0f, _ovalHeight + 10.0f, 64, GetColor(255, 69, 0), TRUE);
		DrawOvalAA(static_cast<float>(_point.x), static_cast<float>(_point.y), _ovalWidth, _ovalHeight, 32, GetColor(255, 177, 80), TRUE);
	}

	if (_isDrawNumber && _pDrawNumber != nullptr) {
		_pDrawNumber->Draw();
	}
}

void Attack::TurnInit() {

	_isStart = false;
	_isAttack = false;
	_isDrawNumber = false;

	_ovalWidth = 0.0f;
	_ovalHeight = 0.0f;

	_waitStartCount = 0;
	_waitEndCount = 0;

	_tween.Reset();
}

void Attack::SetAttackBace(bool isAttack) {

	_isStart = true;
	_isAttack = isAttack;
	_isDrawNumber = false;

	_ovalWidth = 0.0f;
	_ovalHeight = 0.0f;

	_waitStartCount = 0;
	_waitEndCount = 0;

	_tween.Reset();
}

bool Attack::IsEnd() {

	return _waitEndCount >= Parameter::_attackWaitEndFrame;
}