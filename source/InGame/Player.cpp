#include "Player.h"
#include "../Use/MTRandom.h"
#include "../Use/Parameter.h"
#include "../Card.h"
#include <cmath>
#include "../ResourceServer.h"


Player::Player(bool is1P) {   //スマートポインタとメンバ変数の初期化

	_pIsInput = nullptr;
	_pSoundManager = nullptr;

	_pDeck = nullptr;  //とりあえず
	_pField = nullptr;
	_pFormula = nullptr;
	_pAnswer = nullptr;
	_pMagicNumber = nullptr;
	_pHp = nullptr;
	_pTimer = nullptr;
	_pAttack = nullptr;
	_pCutInVertical = nullptr;
	_pCutInHorizontal = nullptr;

	_isPlayingConfom = GameState::Playing;
	_cutInType = CutIn::Type::Max;

	_resultLT = { Parameter::_ScreenHalfWidth, Parameter::_ScreenHalfHeight };
	_resultRB = _resultLT;

	_magicNumber = 0;
	_cutInWaitCount = 0;
	_resultHandle = -1;

	_is1P = is1P;
	_isFirstAttack = false;
	_isResult = false;
}

Player::~Player() {
}

bool Player::Init(std::function<bool(Input::INPUT, bool)> isInput, std::shared_ptr<SoundManager>& soundManager) {

	_pIsInput = isInput;

	if (isInput == nullptr) {
		return false;
	}

	if (soundManager == nullptr) {
		return false;
	}

	_pSoundManager = soundManager;

	_pDeck.reset(new Deck);   //リソースの所有権を放棄し、新たなリソースの所有権を設定する
	_pField.reset(new Field(_is1P));
	_pFormula.reset(new Formula(_is1P));   //boolで自分が1Pかどうかセットしている
	_pAnswer.reset(new Answer(_is1P));
	_pMagicNumber.reset(new MagicNumber(_is1P));
	_pHp.reset(new Hp(_is1P));
	_pTimer.reset(new Timer(_is1P));
	_pAttack.reset(new Attack(_is1P));
	_pCutInVertical.reset(new CutIn(_is1P));
	_pCutInHorizontal.reset(new CutIn(_is1P));

	const TCHAR* resultImage = _is1P ? _T("png/result/result2P.png") : _T("png/result/result1P.png");

	_resultHandle = ResourceServer::LoadGraph(resultImage);

	bool magicCheck = _pMagicNumber->Init();
	bool fieldCheck = _pField->Init(isInput, soundManager);
	bool answerCheck = _pAnswer->Init();
	bool hpCheck = _pHp->Init();
	bool timerInit = _pTimer->Init();
	bool attackInit = _pAttack->Init(soundManager);
	bool cutInVerticalInit = _pCutInVertical->Init(CutIn::Type::Vertical, soundManager);
	bool cutInHorizontalInit = _pCutInHorizontal->Init(CutIn::Type::Horizontal, soundManager);

	if (_resultHandle == -1 || !magicCheck || !fieldCheck || !answerCheck || !hpCheck || !timerInit || !attackInit ||
		!cutInVerticalInit || !cutInHorizontalInit) {
		return false;
	}

	return true;
}

GameState Player::Process(GameState state) {

	GameState ret = state;

	switch (state){
	case GameState::PreStart:
		InitHp();
		ProcessField(state);
		ret = GameState::Start;  //仮ですぐスタートする
		break;

	case GameState::ReStart: // 2ターン目以降の開始ステート
		ProcessField(state);
		ret = GameState::Start;
		break;

	case GameState::Start:
		ProcessField(state);
		ret = GameState::SetMagicNumber;
		break;

	case GameState::SetMagicNumber:
		ProcessField(state);
		ret = GameState::SetHandCard;
		break;

	case GameState::SetHandCard:
		SetHandCard();
		ProcessField(state);
		ret = GameState::Playing;
		break;

	case GameState::Playing:
		ProcessField(state);
		CheckTimeUp();
		ret = _isPlayingConfom;   //プレイか待ちか
		break;

	case GameState::Waiting:
		ProcessField(state);
		break;

	case GameState::PreAttackNumberFirst:
		ProcessField(state);

		if (_isFirstAttack) {
			ret = ProcessPreAttack(ret, true);
		}
		else {
			ret = GameState::PreAttackNumberFirst;
		}
		break;

	case GameState::StartCutInFirst:
		ProcessField(state);

		if (_isFirstAttack) {
			ret = ProcessCutIn(ret, true);
		}
		else {
			ret = GameState::StartCutInFirst;
		}
		break;

	case GameState::PreAttackNumberSecond:
		ProcessField(state);

		if (!_isFirstAttack) {
			ret = ProcessPreAttack(ret, false);
		}
		else {
			ret = GameState::PreAttackNumberSecond;
		}
		break;

	case GameState::StartCutInSecond:
		ProcessField(state);

		if (!_isFirstAttack) {
			ret = ProcessCutIn(ret, false);
		}
		else {
			ret = GameState::StartCutInSecond;
		}
		break;

	case GameState::Judgment:
		ProcessField(state);
		break;

	case GameState::PreResult:
		ProcessField(state);

		if (_pHp->GetHp() <= 0) {
			_isResult = true;

			Point end = { Parameter::_ScreenWidth, Parameter::_ScreenHeight };

			_resulTween.SetTween(_resultRB, end, 90, Tween::Type::SineEnd);

			ret = GameState::Result;
		}
		break;

	case GameState::Result:
		ProcessField(state);

		if (!ProcesResult()) {
			ret = GameState::GameEnd;
		}
		break;

	default:
		break;
	}
	return ret;
}

void Player::ProcessField(GameState state) {

	if (_pField != nullptr) {   //continueが使えないから
		_pField->Process();
		
		ProcessFieldPlaying(state);
	}

	if (_pHp != nullptr) {
		_pHp->Process();
	}

	if (_pTimer != nullptr) {
		_pTimer->Process();
	}
}

GameState Player::ProcessPreAttack(GameState state, bool first) {

	if (_pAttack != nullptr) {
		_pAttack->Process();

		if (_pAttack->IsEnd()) {
			return first ? GameState::StartCutInFirst : GameState::StartCutInSecond;
		}
	}

	return state;
}

GameState Player::ProcessCutIn(GameState state, bool first) {

	if (_pAttack != nullptr) {
		_pAttack->Process();
	}

	if (_pCutInVertical != nullptr) {
		_pCutInVertical->Process();

		if (_pCutInVertical->IsWait()) {
			if (_cutInWaitCount > 120 && !_pCutInVertical->IsFinish()) {
				_pCutInVertical->Finish();
			}

			_cutInWaitCount++;
		}

		if (_pCutInVertical->IsComplete()) {
			return first ? GameState::PreAttackNumberSecond : GameState::Judgment;
		}
	}

	if (_pCutInHorizontal != nullptr) {
		_pCutInHorizontal->Process();

		if (_pCutInHorizontal->IsWait()) {

			if (_cutInWaitCount > 120 && !_pCutInHorizontal->IsFinish()) {
				_pCutInHorizontal->Finish();
			}

			_cutInWaitCount++;
		}

		if (_pCutInHorizontal->IsComplete()) {
			return first ? GameState::PreAttackNumberSecond : GameState::Judgment;
		}
	}

	return state;
}


void Player::ProcessFieldPlaying(GameState state) {

	if (_isResult) {
		return;
	}

	if (state == GameState::Playing && _pFormula != nullptr && _pIsInput != nullptr && _pAnswer != nullptr) {
		int onSelect = _pField->GetOnSelect();
		bool cancel = _pIsInput(Input::INPUT::A, _is1P);
		bool confirm = _pIsInput(Input::INPUT::LB, _is1P);   //決定ボタン

		if (onSelect == -1 && !cancel && !confirm) {  //入力なし
			return;   //処理しない
		}

		bool onSelectAndCansel = onSelect != -1 && cancel;
		bool onSelectAndConfirm = onSelect != -1 && confirm;
		bool canselAndConfirm = cancel && confirm;

		if (onSelectAndCansel || onSelectAndConfirm || canselAndConfirm) {  //同時押し
			return;   //処理しない
		}

		if (onSelect != -1) {
			Select* select = _pField->GetSelect(onSelect);   //B押したときのindexでSelectクラスを取得

			if (select != nullptr) {
				if (_pFormula->IsAdd(select)) {
					bool isSign = _pFormula->IsSign(select);

					select->SetSelect(false);   //falese選択が外れる

					if (isSign) {
						Sign* sign = dynamic_cast<Sign*>(select);

						if (sign != nullptr) {
							select = new Sign(*sign);   //記号はコピーされる新しく作る
						}
					}

					if (_pFormula->Add(select, onSelect)) {
						if (!isSign) {
							_pField->SetSelect(onSelect, nullptr);
							_pField->SetAddCount();
						}
					}
				}
			}
		}
		else if (cancel) {
			Select* cancelSelect = nullptr;
			int cancelIndex = -1;

			if (_pFormula->GetCancel(cancelSelect, cancelIndex)) {
				Card* card = dynamic_cast<Card*>(cancelSelect);

				if (card != nullptr) {
					card->ClearSetXY();   //カードの時だけ戻す
					_pField->SetSelect(cancelIndex, card);
					_pField->SetSubCount();
				}
				else {
					delete cancelSelect;   //newしたSignクラスのデリート
				}

				if (_pSoundManager != nullptr) {
					_pSoundManager->PlaySECommon(SoundManager::SECommon::AButton);
				}
			}
		}
		else if (confirm) {

			SoundManager::SECommon se = SoundManager::SECommon::Cancel;

			if (_pFormula->IsCheck()) {
				_isPlayingConfom = GameState::Waiting;

				se = SoundManager::SECommon::OK;

				_pField->SetFinish(true);
				_pTimer->SetStop(true);
			}

			if (_pSoundManager != nullptr) {
				_pSoundManager->PlaySECommon(se);
			}
		}

		if (_pFormula->IsCheck()) {
			_pAnswer->SetNoAnswer(false);
			_pAnswer->SetNumber(_pFormula->GetAnswer());
		}
		else {
			_pAnswer->SetNoAnswer(true);
		}
	}
}

void Player::Draw() {

	if (_pTimer != nullptr) {
		_pTimer->Draw();
	}

	if (_pField != nullptr) {
		_pField->Draw();
	}

	if (_isPlayingConfom >= GameState::Waiting) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 96);

		Point leftUp = { 0,0 };
		Point rightDown = { Parameter::_ScreenWidth,Parameter::_ScreenHeight };

		if (_is1P) {
			rightDown.x = Parameter::_ScreenHalfWidth;
		}
		else {
			leftUp.x = Parameter::_ScreenHalfWidth;
		}

		DrawBox(leftUp.x, leftUp.y, rightDown.x, rightDown.y, GetColor(238, 130, 238), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	}

	if (_pFormula != nullptr) {
		_pFormula->Draw();
	}

	if (_pAnswer != nullptr) {
		_pAnswer->Draw();
	}

	if (_pMagicNumber != nullptr) {
		_pMagicNumber->Draw();
	}

	if (_pHp != nullptr) {
		_pHp->Draw();
	}

	if (_pAttack != nullptr) {
		_pAttack->Draw();
	}

}

void Player::DrawFront() {

	if (_pCutInVertical != nullptr) {
		_pCutInVertical->Draw();
	}

	if (_pCutInHorizontal != nullptr) {
		_pCutInHorizontal->Draw();
	}

	DrawResult();
}

bool Player::SetDeck(std::list<Card*>& deck) {

	//仮　　本来はデッキ選択画面から情報を受け取るα版は決め打ちでやってく

	if (_pDeck == nullptr) {
		false;
	}


	for (auto&& card : deck) {
		Card* addCard = new Card;
		addCard->Init(card->GetValue(),card->IsPremium());
		_pDeck->AddCard(addCard);
	}

	_pDeck->Shuffle();

	return true;
}

bool Player::SetHandCard() {

	if (_pDeck == nullptr || _pField == nullptr) {
		return false;
	}
	if (!_pDeck->IsDeck()) {
		return false;
	}

	int selectSize = _pField->GetSelectSize();

	if (selectSize <= Sign::SignMax) {
		return false;
	}

	for (int i = Sign::SignMax; i < selectSize; i++) {

		if (_pField->GetSelect(i) != nullptr) {  //カードがあったら何もしない
			continue;
		}
		Card* card = _pDeck->GetCard();

		if (card == nullptr) {   //先頭要素が無い = カードを使いきった
			_pDeck->ResetDiscard();   //デッキの補充
			card = _pDeck->GetCard();

			if (card == nullptr) {  //何かおかしいバグ
				return false;
			}
		}
		_pField->SetSelect(i, card);
	}

	return true;
}

bool Player::SetMagicNumber(int magicNumber) {

	if (_pMagicNumber == nullptr) {
		return false;
	}

	_pMagicNumber->SetNumber(magicNumber);

	_magicNumber = magicNumber;

	return true;
}

bool Player::InitHp() {

	if (_pHp == nullptr) {
		return false;
	}

	_pHp->InitHP(Parameter::_hp);
	_pHp->SetDraw(true);

	return true;
}

bool Player::SetTimer() {

	if (_pTimer == nullptr) {
		return false;
	}

	return _pTimer->SetStart(Parameter::_turnTime);
}

bool Player::SetHp(int hp) {

	if (_pHp == nullptr) {
		return false;
	}

	_pHp->SetHp(hp);

	return true;
}

int Player::GetHp() {

	if (_pHp == nullptr) {
		return 0;
	}

	return _pHp->GetHp();
}

int Player::GetAttack() {

	if (!_pFormula->IsCheck()) {
		return 0;
	}

	int attack = Parameter::_attackNumberBace;
	int answer = _pFormula->GetAnswer();
	int diff = std::abs(_magicNumber - answer);

	if (diff == 0) {
		attack += Parameter::_attackNumberPerfectBonus;
	}
	else if (diff < Parameter::_attackNumberDiffMax) {
		attack += Parameter::_attackNumberUnit * (Parameter::_attackNumberDiffMax - diff);
	}

	return attack;
}

void Player::SetAttack() {

	_pAttack->SetAttackBace(_pFormula->IsCheck());
}

void Player::TurnInit() {

	_pAnswer->TurnInit();

	std::vector<Card*> discardList;

	_pFormula->GetDiscard(discardList);

	for (int i = 0; i < discardList.size(); i++) {
		_pDeck->AddDiscard(discardList[i]);
		
	}

	_pFormula->TurnInit();
	_pField->TurnInit();

	_pAttack->TurnInit();

	_pCutInVertical->TurnInit();
	_pCutInHorizontal->TurnInit();

	_isPlayingConfom = GameState::Playing;
	_cutInWaitCount = 0;
}

void Player::CheckTimeUp() {

	if (_isPlayingConfom == GameState::Playing) {
		if (_pTimer == nullptr) {
			return;
		}

		if (!_pTimer->GetIsEnd()) {
			return;
		}

		_isPlayingConfom = GameState::Waiting;
	}
} 

void Player::StartCutIn(CutIn::Type type, bool leftUp) {

	switch (type){
	case CutIn::Type::Vertical:
		if (_pCutInVertical != nullptr) {
			_pCutInVertical->Start(leftUp);
		}
		break;

	case CutIn::Type::Horizontal:
		if (_pCutInHorizontal != nullptr) {
			_pCutInHorizontal->Start(leftUp);
		}
		break;

	default:
		return;
	}

	_cutInType = type;
}

bool Player::ProcesResult() {

	_resulTween.Process();

	Point rb = _resulTween.GetPosition();
	Point diff = { rb.x - _resultRB.x, rb.y - _resultRB.y };

	_resultLT.x -= diff.x;
	_resultLT.y -= diff.y;

	_resultRB = rb;

	if (_resulTween.IsEnd()) {
		if (_pIsInput(Input::INPUT::B, _is1P)) {
			return false;
		}
	}

	return true;
}

void Player::DrawResult() {

	if (!_isResult) {
		return;
	}

	DrawExtendGraph(_resultLT.x, _resultLT.y, _resultRB.x, _resultRB.y, _resultHandle, FALSE);
}
