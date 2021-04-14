#include "DeckSelect.h"
#include "../Use/Parameter.h"
#include "../ResourceServer.h"

DeckSelect::DeckSelect(bool is1p) : ObjectBase(TYPE::DeckSelect){

	_pIsInput = nullptr;
	_pSoundManager = nullptr;
	_normal = nullptr;
	_premium = nullptr;
	_lDeck.clear();

	_selected = Selected::NormalCard;

	_selectDeckIndex = 0;

	_isEnd = false;
	_is1P = is1p;
}

DeckSelect::~DeckSelect() {
}


bool DeckSelect::Init(std::function<bool(Input::INPUT, bool)> isInput, std::shared_ptr<SoundManager>& soundManager) {

	if (isInput == nullptr) {
		return false;
	}

	_pIsInput = isInput;

	if (soundManager == nullptr) {
		return false;
	}

	_pSoundManager = soundManager;

	_normal.reset(new ChoiceCard(_is1P));
	_premium.reset(new ChoiceCard(_is1P));

	bool normal = _normal->Init(isInput, soundManager, false);
	bool premium = _premium->Init(isInput, soundManager, true);

	if (!normal || !premium) {
		return false;
	}

	_selected = Selected::NormalCard;
	_normal->SetSelect(true);

	return InitGraph();
}

bool DeckSelect::InitGraph() {

	int handle0 = ResourceServer::LoadGraph(_T("png/deck/waku_s.png"));
	int handle1 = ResourceServer::LoadGraph(_T("png/back/wait.png"));

	if (handle0 == -1 || handle1 == -1) {
		return false;
	}

	DrawInfo info0;
	DrawInfo info1;

	info0.handle = handle0;
	info0.xy = _is1P ? Parameter::_SelectedDeckFrame1P : Parameter::_SelectedDeckFrame2P;
	info0.isDraw = false;

	Point point1P = { 0,0 };
	Point point2P = { 960,0 };

	info1.handle = handle1;
	info1.xy = _is1P ? point1P : point2P;
	info1.isDraw = false;

	_vDrawInfo.push_back(info0);
	_vDrawInfo.push_back(info1);

	return true;
}

GameState DeckSelect::Process(GameState state) {

	if (_isEnd) {
		return GameState::DeckSelectFinish;
	}

	if (_normal != nullptr) {
		_normal->Process();

		int choice = _normal->GetSelectCardNumber();

		if (choice > 0) {
			SetDeck(choice,false);
			_normal->ResetSelectCardNumber();
		}
	}

	if (_premium != nullptr) {
		_premium->Process();

		int choice = _premium->GetSelectCardNumber();

		if (choice > 0) {
			SetDeck(choice, true);
			_premium->ResetSelectCardNumber();
		}
	}

	ProcessSelect();
	ProcessDeckSelect();
	ProcessDeckCancel();

	if (_pIsInput(Input::INPUT::X, _is1P)) {
		SoundManager::SECommon se = SoundManager::SECommon::Cancel;

		if (_lDeck.size() == Parameter::_deckNum) {
			_isEnd = true;
			se = SoundManager::SECommon::OK;
		}

		if (_pSoundManager != nullptr) {
			_pSoundManager->PlaySECommon(se);
		}
	}

	return _isEnd ? GameState::DeckSelectFinish : GameState::DeckSelect;
}

void DeckSelect::SetDraw(bool isDraw) {

	if (_vDrawInfo.size() == 0) {
		return;
	}

	_vDrawInfo[0].isDraw = isDraw;
}

void DeckSelect::ProcessDeckCancel() {

	if (_selected != Selected::DeckUp && _selected != Selected::DeckDown) {
		return;
	}

	if (_pIsInput(Input::INPUT::A, _is1P)) {

		int count = 0;
		Card* pCard = nullptr;

		for (auto& card : _lDeck) {
			if (count == _selectDeckIndex) {
				pCard = card;

				card->SetSelect(false);
				int number = card->GetValue();

				if (card->IsPremium()) {
					_premium->SetCancelCard(number);
				}
				else {
					_normal->SetCancelCard(number);
				}
				
				ShiftDeck(count, card->GetXY());
				_lDeck.remove(card);
				
				break;
			}
			count++;
		}

		if (count == _lDeck.size()) {
			if (_selectDeckIndex > 0) {
				_selectDeckIndex--;
			}
		}

		if (pCard != nullptr) {
			delete pCard;
		}

		if (_pSoundManager != nullptr) {
			_pSoundManager->PlaySECommon(SoundManager::SECommon::AButton);
		}
	}
}

void DeckSelect::ShiftDeck(int cancelIndex,Point cancelXY) {

	int count = 0;
	Point lastPoint = cancelXY;

	for (auto& card : _lDeck) {
		Point point = card->GetXY();

		if (count > cancelIndex) {
			card->SetXY(lastPoint.x, lastPoint.y);
		}

		lastPoint = point;
		count++;
	}
}

void DeckSelect::Draw() {

	if (_normal != nullptr) {
		_normal->Draw();
	}

	if (_premium != nullptr) {
		_premium->Draw();
	}

	int count = 0;

	for (auto deck : _lDeck) {
		deck->SetSelect(_selectDeckIndex == count);   //同じときだけ枠を作る
		deck->Draw();
		count++;
	}

	ObjectBase::Draw();

	if (_isEnd) {
		if (_vDrawInfo.size() >= 1) {
			if (!_vDrawInfo[1].isDraw) {
				_vDrawInfo[1].isDraw = true;
			}
		}
	}
}

void DeckSelect::DeleteImage() {

	if (_normal != nullptr) {
		_normal->DeleteImage();
	}

	if (_premium != nullptr) {
		_premium->DeleteImage();
	}

	if (_vDrawInfo.size() > 0) {
		for (int i = 0; i < _vDrawInfo.size(); i++) {
			DrawInfo drawInfo = _vDrawInfo[i];

			ResourceServer::DeleteGraph(drawInfo.handle);
		}
		_vDrawInfo.clear();
	}
}

void DeckSelect::ProcessSelect() {

	Selected preSelect = _selected;

	if (_pIsInput(Input::INPUT::Up, _is1P)) {
		if (_selected > Selected::NormalCard) {
			int next = static_cast<int>(_selected);

			next--;
			_selected = static_cast<Selected>(next);
		}
	}

	if (_pIsInput(Input::INPUT::Down, _is1P)) {
		if (_selected < Selected::DeckDown) {
			bool isDown = true;

			if (_selected == Selected::DeckUp) {

				isDown = _lDeck.size() > 9;
			}

			if (isDown) {
				int next = static_cast<int>(_selected);

				next++;
				_selected = static_cast<Selected>(next);
			}
		}
	}

	if (preSelect != _selected) {
		bool se = true;

		switch (_selected) {
		case Selected::NormalCard:
			_normal->SetSelect(true);
			_premium->SetSelect(false);
			break;

		case Selected::PremiumCard:
			_normal->SetSelect(false);
			_premium->SetSelect(true);

			if (preSelect == Selected::DeckUp) {
				SetDraw(false);
			}

			break;

		case Selected::DeckUp:
			_normal->SetSelect(false);
			_premium->SetSelect(false);

			if(preSelect == Selected::PremiumCard){
				SetDraw(true);
			}
			if (preSelect == Selected::DeckDown) {
				_selectDeckIndex -= 10;
			}
			break;

		case Selected::DeckDown:
			_normal->SetSelect(false);
			_premium->SetSelect(false);

			if (preSelect == Selected::DeckUp) {
				int size = _lDeck.size();

				if (size >= 10) {
					_selectDeckIndex += 10;
					if (_selectDeckIndex >= size) {
						_selectDeckIndex = size - 1;
					}
				}
			}
			break;

		default:
			se = false;
			break;
		}

		if (se && _pSoundManager != nullptr) {
			_pSoundManager->PlaySECommon(SoundManager::SECommon::Move);
		}
	}
}

void DeckSelect::ProcessDeckSelect() {

	if (_selected != Selected::DeckUp && _selected != Selected::DeckDown) {
		return;
	}

	bool se = false;

	if (_pIsInput(Input::INPUT::Right, _is1P)) {
		int maxIndex = _lDeck.size() - 1;

		if (_selected == Selected::DeckUp) {

			if (maxIndex >= 9) {
				maxIndex = 9;
			}
		}

		if (_selectDeckIndex < maxIndex) {
			_selectDeckIndex++;

			se = true;
		}
	}

	if (_pIsInput(Input::INPUT::Left, _is1P)) {

		int minIndex = 0;
		
		if (_selected == Selected::DeckDown) {

			minIndex = 10;
		}

		if (_selectDeckIndex > minIndex) {
			_selectDeckIndex--;

			se = true;
		}
	}

	if (se && _pSoundManager != nullptr) {
		_pSoundManager->PlaySECommon(SoundManager::SECommon::Move);
	}
}

bool DeckSelect::SetDeck(int cardNum, bool isPremium) {

	int size = _lDeck.size();

	if (size >= Parameter::_deckNum) {
		return false;
	}

	Card* card = new Card;

	if (!card->Init(cardNum, isPremium, false, true)) {
		return false;
	}

	Point point = _is1P ? Parameter::_SelectedDeckStartPosition1P : Parameter::_SelectedDeckStartPosition2P;

	if (size >= 10) {
		size = size % 10;
		point.y += Parameter::_SelectedDeckIntervalHeight;
	}

	point.x += size * Parameter::_SelectedDeckIntervalWidth;

	card->SetXY(point.x, point.y);
	card->SetDraw(true);
	card->SetSelect(false);

	_selectDeckIndex = _lDeck.size();

	_lDeck.push_back(card);

	bool isMaxDeck = _lDeck.size() >= Parameter::_deckNum;

	_normal->SetMaxDeck(isMaxDeck);
	_premium->SetMaxDeck(isMaxDeck);

	return true;
}
/*

#ifdef _DEBUG   //デバッグビルドの時だけ有効になる
void DeckSelect::SetDebugFinish() {

	int dbugCardList[] = { 9,9,9,2,2,2,3,3,3,4,4,4,5,5,5,6,6,6,7,7 };

	for (int i = 0; i < 20; i++) {
		SetDeck(dbugCardList[i], false);
	}

	_isEnd = true;
}

#endif // DEBUG

*/