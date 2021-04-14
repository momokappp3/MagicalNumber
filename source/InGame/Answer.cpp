#include "Answer.h"
#include "../Use/Parameter.h"

Answer::Answer(bool is1P):ObjectBase(ObjectBase::TYPE::Answer) {

	_pDrawNumber = nullptr;

	_point = { 0,0 };
	_handle = 0;
	_is1P = is1P;
	_isNoAnswer = false;
}

Answer::~Answer() {
}

bool Answer::Init() {

	std::array<const TCHAR*, DrawNumber::ImageHandlNum> imageNames = {
		_T("png/number/answer0.png"), _T("png/number/answer1.png") ,
		_T("png/number/answer2.png"), _T("png/number/answer3.png") ,
		_T("png/number/answer4.png"), _T("png/number/answer5.png") ,
		_T("png/number/answer6.png"), _T("png/number/answer7.png") ,
		_T("png/number/answer8.png"), _T("png/number/answer9.png")
	};

	_pDrawNumber.reset(new DrawNumber);

	_point = _is1P ? Parameter::_AnswerPosition1P : Parameter::_AnswerPosition2P;

	if (!_pDrawNumber->Init(imageNames)) {
		return false;
	}	

	return true;
}

bool Answer::SetNumber(int num) {

	if (_pDrawNumber == nullptr) {
		return false;
	}

	_handle = _pDrawNumber->SetNumber(_point.x, _point.y, num, DrawNumber:: Anchor::LeftUp, _handle);

	return true;
}

void Answer::Draw() {

	if (_isNoAnswer) {
		return;
	}

	if (_pDrawNumber != nullptr) {
		_pDrawNumber->Draw();
	}
}

void Answer::TurnInit() {

	if (_handle != 0) {
		_pDrawNumber->SetIsDraw(_handle,false);
	}
}