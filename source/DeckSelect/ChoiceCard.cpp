#include "ChoiceCard.h"
#include "../ObjectBace.h"
#include "../Use/Parameter.h"
#include "../ResourceServer.h"

namespace {
	constexpr int CardSelectPosition = 5;   //リストの5番目
}

ChoiceCard::ChoiceCard(bool is1P):ObjectBase(TYPE::ChoiceCard) {

	_plCard.clear();
	_pIsInput = nullptr;
	_pSoundManager = nullptr;

	_animation = Animation::None;

	_is1P = is1P;

	_animationBase = {0,0};

	_selectCardNumber = 0;

	_isSelect = false;   //外からセットできるように
	_isPremium = false;
	_isMaxDeck = false;
}

ChoiceCard::~ChoiceCard(){  

	for (auto&& card : _plCard) {   //&& = 実体をデリートしないといけない
		delete card;
	}
	_plCard.clear();
}

bool ChoiceCard::Init(std::function<bool(Input::INPUT, bool)> isInput, std::shared_ptr<SoundManager>& soundManager, bool isPremium) {

	if (isInput == nullptr) {
		return false;
	}

	_pIsInput = isInput;

	if (soundManager == nullptr) {
		return false;
	}

	_pSoundManager = soundManager;

	_isPremium = isPremium;

	Point position = { 0, 0 };

	if (_isPremium) {
		position = _is1P ? Parameter::_ChoicePremiumCardStartPotistion1P : Parameter::_ChoicePremiumCardStartPotistion2P;
	}
	else {
		position = _is1P ? Parameter::_ChoiceNormalCardStartPotistion1P : Parameter::_ChoiceNormalCardStartPotistion2P;
	}

	//テーブル
	//int cardIndex[] = { 7,8,9,10,1,2,3,4,5,6 };
	int cardIndex[] = { 6,7,8,9,10,1,2,3,4,5 };

	for (int i = 0; i < Card::CardMax; i++) {
		SelectCard* card = new SelectCard;   //インスタンス10個

		card->Init(cardIndex[i],_isPremium, true);
		card->SetXY(position.x, position.y);
		card->SetSelect(false);

		position.x += Parameter::_ChoiceCardIntervalWidth;

		_plCard.push_back(card);
	}
	
	return InitGraph();
}

bool ChoiceCard::InitGraph() {

	int handle0 = ResourceServer::LoadGraph(_T("png/deck/waku_b.png"));
	int handle1 = ResourceServer::LoadGraph(_T("png/deck/Dwaku.png"));
	int handle2 = ResourceServer::LoadGraph(_T("png/deck/filter.png"));

	if (handle0 == -1 || handle1 == -1 || handle2 == -1) {
		return false;
	}

	DrawInfo info0;
	DrawInfo info1;
	DrawInfo info2;

	info0.handle = handle0;
	info1.handle = handle1;
	info2.handle = handle2;
	

	if (_isPremium) {

		info0.xy = _is1P ? Parameter::_ChoicePremiumWaku1P : Parameter::_ChoicePremiumWaku2P;
		info1.xy = _is1P ? Parameter::_ChoicePremiumBackWaku1P : Parameter::_ChoicePremiumBackWaku2P;
		info2.xy = _is1P ? Parameter::_ChoicePremiumFilter1P : Parameter::_ChoicePremiumFilter2P;
	}
	else {
		info0.xy = _is1P ? Parameter::_ChoiceNormalWaku1P : Parameter::_ChoiceNormalWaku2P;
		info1.xy = _is1P ? Parameter::_ChoiceNormalBackWaku1P : Parameter::_ChoiceNormalBackWaku2P;
		info2.xy = _is1P ? Parameter::_ChoiceNormalFilter1P : Parameter::_ChoiceNormalFilter2P;
	}


	info0.isDraw = false;
	info1.isDraw = true;
	info2.isDraw = true;

	_vDrawInfo.push_back(info0);
	_vDrawInfo.push_back(info1);
	_vDrawInfo.push_back(info2);

	return true;
}

void ChoiceCard::Process(){

	_tween.Process();

	if (IsAnimation()) {
		ProcessAnimation();
	}
	else if (_isSelect) {
		CheckInput();
	}

	for (auto& card : _plCard) {
		card->Process();
	}
}

void ChoiceCard::Draw(){

	// 選択するカードの表示エリアを設定

	Point start = { 0, 0 };
	Point end = { 0,0 };

	if (_isPremium) {
		if (_is1P) {
			start = Parameter::_ChoicePremiumStartPosition1P;
			end = Parameter::_ChoicePremiumEndPosition1P;
		}
		else {
			start = Parameter::_ChoicePremiumStartPosition2P;
			end = Parameter::_ChoicePremiumEndPosition2P;
		}
	}
	else {
		if (_is1P) {
			start = Parameter::_ChoiceNormalStartPosition1P;
			end = Parameter::_ChoiceNormalEndPosition1P;
		}
		else {
			start = Parameter::_ChoiceNormalStartPosition2P;
			end = Parameter::_ChoiceNormalEndPosition2P;
		}
	}

	SetDrawArea(start.x,start.y,end.x,end.y);

	for (auto& card : _plCard) {
		card->Draw();
	}

	// 描画エリアを全画面に戻す
	SetDrawArea(0, 0, Parameter::_ScreenWidth, Parameter::_ScreenHeight);

	ObjectBase::Draw();
}

void ChoiceCard::DeleteImage() {

	for (auto&& card : _plCard) {
		card->DeleteImage();
	}

	if (_vDrawInfo.size() > 0) {
		for (int i = 0; i < _vDrawInfo.size(); i++) {
			DrawInfo drawInfo = _vDrawInfo[i];

			ResourceServer::DeleteGraph(drawInfo.handle);
		}
		_vDrawInfo.clear();
	}
}

void ChoiceCard::SetSelect(bool isSelect) {

	_isSelect = isSelect;

	_vDrawInfo[0].isDraw = isSelect;
}

void ChoiceCard::CheckInput() {

	if (!_isSelect || _pIsInput == nullptr) {
		return;
	}

	if (!IsAnimation()) {
		bool seMove = false;

		if (_pIsInput(Input::INPUT::Right, _is1P)) {
			_animation = Animation::Right;
			seMove = true;
		}

		if (_pIsInput(Input::INPUT::Left, _is1P)) {
			_animation = Animation::Left;
			seMove = true;
		}

		if (seMove && _pSoundManager != nullptr){
			_pSoundManager->PlaySEDeckSelect(SoundManager::SEDeckSelect::ChoiceMove);

		}

		if (_selectCardNumber <= 0 && !_isMaxDeck) {
			if (_pIsInput(Input::INPUT::B, _is1P)) {

				SelectCard* card = GetSelectCard();

				if (card != nullptr) {
					int selectCount = card->GetSelectCount();

					if (selectCount < Parameter::_someNumberInDeck) {

						_selectCardNumber = card->GetValue();

						if (card->IsPremium()) {
							int count = selectCount + 1;

							for (auto&& card : _plCard) {
								card->SetSelectCount(count);
							}
						}
						else {
							card->SetSelectCount(selectCount + 1);
						}

						if (_pSoundManager != nullptr) {
							_pSoundManager->PlaySECommon(SoundManager::SECommon::BButton);

						}
					}
				}
			}
		}
	}
}

void ChoiceCard::ProcessAnimation() {

	if (!_tween.IsStart()) {
		int x = 0;
		if (_animation == Animation::Left) {
			x = -Parameter::_ChoiceCardIntervalWidth;
		}
		else if (_animation == Animation::Right) {
			x = Parameter::_ChoiceCardIntervalWidth;
		}
		
		_tween.SetTween({ 0,0 }, { x,0 }, 15);

		auto& card = _plCard.front();

		 _animationBase = card->GetXY();
	}
	
	int deltaX = _tween.GetPosition().x;
	int count = 0;

	for (auto& card : _plCard) {
		Point position = { _animationBase.x + count * Parameter::_ChoiceCardIntervalWidth , _animationBase.y };

		card->SetXY(position.x + deltaX, position.y);
		count++;
	}

	if (_tween.IsEnd()) {
		if (_animation == Animation::Left) {
			Point position = { _animationBase.x + _plCard.size() * Parameter::_ChoiceCardIntervalWidth , _animationBase.y };
			SelectCard* front = _plCard.front();

			front->SetXY(position.x, position.y);

			_plCard.pop_front();
			_plCard.push_back(front);

		}
		else if (_animation == Animation::Right) {
			SelectCard* back = _plCard.back();

			back->SetXY(_animationBase.x, _animationBase.y);

			_plCard.pop_back();
			_plCard.push_front(back);
		}
		_animation = Animation::None;

		_tween.Reset();
		
	}
}

SelectCard* ChoiceCard::GetSelectCard() {

	int count = 0;

	for (auto&& card : _plCard) {
		if (count == CardSelectPosition) {
			return card;
		}
		count++;
	}

	return nullptr;
}

void ChoiceCard::SetCancelCard(int number) {

	auto result = std::find_if(_plCard.begin(), _plCard.end(), [number](SelectCard* card) { return card->GetValue() == number ; });
	SelectCard* selectCard = *result;

	if (selectCard != nullptr) {
		int selectCount = selectCard->GetSelectCount();

		if (selectCount > 0) {
			if (selectCard->IsPremium()) {
				int count = selectCount - 1;

				for (auto&& card : _plCard) {
					card->SetSelectCount(count);
				}
			}
			else {
				selectCard->SetSelectCount(selectCount - 1);
			}
		}
	}
}
