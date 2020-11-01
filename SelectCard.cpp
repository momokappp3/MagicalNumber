#include "SelectCard.h"
#include "Parameter.h"

SelectCard::SelectCard() : Card() {
	
	_pDrawNumber = nullptr;
	_selectCount = 0;

	_handle = 0;
}

SelectCard::~SelectCard() {
}

bool SelectCard::Init(int cardNum, bool isPremium, bool isChoice, bool isSmall) {

	if (!Card::Init(cardNum, isPremium, isChoice, isSmall)) {
		return false;
	}

	_pDrawNumber.reset(new DrawNumber);

	std::array<const TCHAR*, DrawNumber::ImageHandlNum> imageNames = {
		_T("png/number/deckNumber0.png"), _T("png/number/deckNumber1.png") ,
		_T("png/number/deckNumber2.png"), _T("png/number/deckNumber3.png") ,
		_T("png/number/deckNumber4.png"), _T("png/number/deckNumber5.png") ,
		_T("png/number/deckNumber6.png"), _T("png/number/deckNumber7.png") ,
		_T("png/number/deckNumber8.png"), _T("png/number/deckNumber9.png")
	};


	if (!_pDrawNumber->Init(imageNames)) {
		return false;
	}

	return true;
}

bool SelectCard::ProcessSelectCount() {

	if (_pDrawNumber == nullptr) {
		return false;
	}

	DrawInfo info = { 0,{0,0},false};

	GetDrawInfo(0,info);

	_handle = _pDrawNumber->SetNumber(info.xy.x + 100, info.xy.y + 270,
									  Parameter::_someNumberInDeck - _selectCount,
									  DrawNumber::Anchor::LeftUp, _handle);

	return true;
}

void SelectCard::Process() {

	Card::Process();
	ProcessSelectCount();

}

void SelectCard::Draw() {

	Card::Draw();
	if (_pDrawNumber != nullptr) {
		_pDrawNumber->Draw();
	}
}