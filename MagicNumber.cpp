#include "MagicNumber.h"
#include "Parameter.h"

MagicNumber::MagicNumber(bool is1P) : ObjectBase (ObjectBase::TYPE::MagicNumber) {

	_pDrawNumber = nullptr;

	_point = { 0,0 };
	_handle = 0;
	_is1P = is1P;
}

MagicNumber::~MagicNumber() {
}

bool MagicNumber::Init() {

	std::array<const TCHAR*, DrawNumber::ImageHandlNum> imageNames = {
	_T("png/number/magic0.png"), _T("png/number/magic1.png") ,
	_T("png/number/magic2.png"), _T("png/number/magic3.png") ,
	_T("png/number/magic4.png"), _T("png/number/magic5.png") ,
	_T("png/number/magic6.png"), _T("png/number/magic7.png") ,
	_T("png/number/magic8.png"), _T("png/number/magic9.png")
	};

	_pDrawNumber.reset(new DrawNumber);

	_point = _is1P ? Parameter::_MagicNumberPosition1P : Parameter::_MagicNumberPosition2P;

	if (!_pDrawNumber->Init(imageNames)) {
		return false;
	}

	return true;
}

bool MagicNumber::SetNumber(int num) {

	if (_pDrawNumber == nullptr) {
		return false;
	}

	_handle = _pDrawNumber->SetNumber(_point.x, _point.y, num, DrawNumber::Anchor::LeftUp, _handle);

	return true;
}

void MagicNumber::Draw() {

	if (_pDrawNumber != nullptr) {
		_pDrawNumber->Draw();
	}
}
