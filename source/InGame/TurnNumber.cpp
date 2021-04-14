#include "TurnNumber.h"
#include "../Use/Parameter.h"

TurnNumber::TurnNumber() {

	_pDrawNumber = nullptr;
	_point = { 0,0 };
	_handle = -1;
}

TurnNumber::~TurnNumber() {
}

bool TurnNumber::Init() {

	std::array<const TCHAR*, DrawNumber::ImageHandlNum> imageNames = {
	_T("png/number/turnNumber0.png"), _T("png/number/turnNumber1.png") ,
	_T("png/number/turnNumber2.png"), _T("png/number/turnNumber3.png") ,
	_T("png/number/turnNumber4.png"), _T("png/number/turnNumber5.png") ,
	_T("png/number/turnNumber6.png"), _T("png/number/turnNumber7.png") ,
	_T("png/number/turnNumber8.png"), _T("png/number/turnNumber9.png")
	};

	_pDrawNumber.reset(new DrawNumber);

	_point = Parameter::_TurnNumberPosition;

	if (!_pDrawNumber->Init(imageNames)) {
		return false;
	}

	return true;
}

bool TurnNumber::SetNumber(int num) {

	if (_pDrawNumber == nullptr) {
		return false;
	}

	_handle = _pDrawNumber->SetNumber(_point.x, _point.y, num, DrawNumber::Anchor::LeftUp, _handle);

	return true;
}

void TurnNumber::Draw() {

	if (_pDrawNumber != nullptr) {
		_pDrawNumber->Draw();
	}
}