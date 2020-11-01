/*
  最上位のクラス
  いろいろなクラスを呼び出す
*/

#include "Game.h"
#include "MTRandom.h"
#include "Fade.h"
#include "Parameter.h"

Game::Game() {

	_state1 = { GameState::Title, GameState::Title };
	_state2 = { GameState::Title, GameState::Title };

	_pInGameBG = nullptr;
	_pInput = nullptr;
	_pSoundManager = nullptr;

	_pTitle = nullptr;
	_pTitleMenu = nullptr;

	_pDeckSelectBG = nullptr;

	_pDeckSelect1 = nullptr;
	_pDeckSelect2 = nullptr;

	_pTurnNumber = nullptr;

	_pPlayer1 = nullptr;
	_pPlayer2 = nullptr;

	_pFirst = nullptr;
	_pSecond = nullptr;

	_isInput = nullptr;

	_fadeCount = 0;
	_turnNum = 0;
}

Game::~Game() {
}

bool Game::Init() {

	_state1 = { GameState::Title, GameState::Title }; 
	_state2 = { GameState::Title, GameState::Title };

	//_state1 = { GameState::DeckSelect, GameState::DeckSelect };   //仮処理
	//_state2 = { GameState::DeckSelect, GameState::DeckSelect };

	_pInput.reset(new Input);
	_pSoundManager.reset(new SoundManager);

	_pTitle.reset(new Title);
	_pTitleMenu.reset(new TitleMenu);

	_pDeckSelectBG.reset(new DeckSelectBG);

	_pDeckSelect1.reset(new DeckSelect(true));
	_pDeckSelect2.reset(new DeckSelect(false));

	_pInGameBG.reset(new InGameBG);
	_pTurnNumber.reset(new TurnNumber);

	_pPlayer1.reset(new Player(true));
	_pPlayer2.reset(new Player(false));

	_isInput = [&](Input::INPUT type, bool p1) {   //ラムダ式
		return _pInput->IsInput(type,p1);
	};

	bool soundManager = _pSoundManager->Init();
	bool seCommon = _pSoundManager->LoadSECommon();

	bool titleInit = _pTitle->Init(_isInput, _pSoundManager);
	bool titleMenuInit = _pTitleMenu->Init(_isInput, _pSoundManager);

/*
	bool deckSelectBGInit = _pDeckSelectBG->Init();

	bool deckSelect1 = _pDeckSelect1->Init(isInput, _pSoundManager);
	bool deckSelect2 = _pDeckSelect2->Init(isInput, _pSoundManager);

	bool inGameBGInit = _pInGameBG->Init();
	bool turnNumberInit = _pTurnNumber->Init();

	bool playerInit1 = _pPlayer1->Init(isInput, _pSoundManager);
	bool playerInit2 = _pPlayer2->Init(isInput, _pSoundManager);

	*/
	if (!soundManager || !seCommon || !titleInit || !titleMenuInit/* || !deckSelectBGInit || !turnNumberInit ||
		!deckSelect1 ||!deckSelect2 || !inGameBGInit || !playerInit1 || !playerInit2*/) {
		return false;
	}

	_pTitle->SetDraw(true);

	return true;
}

bool Game::Process() {

	if (_pInput != nullptr) {   //プレイヤークラスで情報がとられている
		_pInput->Process();
	}

	if (_pSoundManager != nullptr) {
		_pSoundManager->Process();
	}

	GameState state1 = GameState::Max;
	GameState state2 = GameState::Max;

	ProcessTitle(state1, state2);
	ProcessTitleMenu(state1, state2);
	ProcessDeckSelect(state1, state2);
	ProcessPlay(state1, state2);

	return CheckState(state1, state2);
}

void Game::Draw() {

	if (IsTitle()) {
		if (_pTitle != nullptr) {
			_pTitle->Draw();
		}
	}

	if (IsTitleMenu()) {
		if (_pTitleMenu != nullptr) {
			_pTitleMenu->Draw();
		}
	}

	if (IsDeckSelect()) {
		_pDeckSelectBG->Draw();

		if (_pDeckSelect1 != nullptr) {
			_pDeckSelect1->Draw();
		}

		if (_pDeckSelect2 != nullptr) {
			_pDeckSelect2->Draw();
		}
	}

	if (IsPlay()) {
		if (_pInGameBG != nullptr) {
			_pInGameBG->Draw();
		}

		if (_pPlayer1 != nullptr) {
			_pPlayer1->Draw();
		}

		if (_pPlayer2 != nullptr) {
			_pPlayer2->Draw();
		}

		if (_pTurnNumber != nullptr) {
			_pTurnNumber->Draw();
		}

		if (_pPlayer1 != nullptr) {
			_pPlayer1->DrawFront();
		}

		if (_pPlayer2 != nullptr) {
			_pPlayer2->DrawFront();
		}
	}
}

void Game::ProcessTitle(GameState& state1, GameState& state2) {

	if (!IsTitle()) {
		return;
	}

	if (_pTitle != nullptr) {
		GameState state = _pTitle->Process(_state1.now);

		state1 = state;
		state2 = state;
	}
}

void Game::ProcessTitleMenu(GameState& state1, GameState& state2) {

	if (!IsTitleMenu()) {
		return;
	}

	if (_pTitleMenu != nullptr) {
		GameState state = _pTitleMenu->Process(_state1.now);

		state1 = state;
		state2 = state;
	}

}

void Game::ProcessDeckSelect(GameState& state1, GameState& state2) {

	if (!IsDeckSelect()) {
		return;
	}
	/*
#ifdef _DEBUG
	if (_pInput != nullptr && _pInput->IsInput(Input::INPUT::Start, true)) {

		if (_pDeckSelect1 != nullptr) {
			_pDeckSelect1->SetDebugFinish();
		}

		if (_pDeckSelect2 != nullptr) {
			_pDeckSelect2->SetDebugFinish();
		}
	}
#endif // DEBUG
	*/

	if (_pDeckSelect1 != nullptr) {
		state1 = _pDeckSelect1->Process(_state1.now);
	}

	if (_pDeckSelect2 != nullptr) {
		state2 = _pDeckSelect2->Process(_state2.now);
	}

	if (state1 == GameState::DeckSelectFinish && state2 == GameState::DeckSelectFinish) {
		_fadeCount++;

		if (_fadeCount > Parameter::_FadeInFrame + 1) {
			state1 = GameState::PreStart;
			state2 = state1;
		}
	}
}

void Game::ProcessPlay(GameState& state1, GameState& state2) {

	if (!IsPlay()) {
		return;
	}

	if (_pPlayer1 != nullptr) {
		state1 = _pPlayer1->Process(_state1.now);

		if (state1 == GameState::Waiting) {
			_state1.now = state1;

			SetFirstAndSecont(true);
		}
	}

	if (_pPlayer2 != nullptr) {
		state2 = _pPlayer2->Process(_state2.now);

		if (state2 == GameState::Waiting) {
			_state2.now = state2;

			SetFirstAndSecont(false);
		}
	}

	CheckNextAttackState(GameState::StartCutInFirst, state1, state2);
	CheckNextAttackState(GameState::PreAttackNumberSecond, state1, state2);
	CheckNextAttackState(GameState::StartCutInSecond, state1, state2);
	CheckNextAttackState(GameState::Judgment, state1, state2);
	CheckNextAttackState(GameState::Result, state1, state2);
	CheckNextAttackState(GameState::GameEnd, state1, state2);
}

//プレイヤーのステートが1と2で同じだったら進む
bool Game:: CheckState(GameState state1, GameState state2) {

	GameState last1 = _state1.last;
	GameState last2 = _state2.last;

	_state1.last = _state1.now;
	_state2.last = _state2.now;

	if (state1 != last1 || state2 != last2) {
		if (state1 == state2) {
			_state1.now = state1;
			_state2.now = state1;

			_state1.last = state1;
			_state2.last = state1;

			switch (state1){
			case GameState::TitleMenu:
				_pTitle->DeleteImage();
				_pSoundManager->LoadSETitleMenu();
				_pTitleMenu->SetDraw(true);
				break;

			case GameState::DeckSelect:
				_pTitle->DeleteImage();
				_pTitleMenu->DeleteImage();
				_pDeckSelectBG->Init();
				_pDeckSelect1->Init(_isInput, _pSoundManager);
				_pDeckSelect2->Init(_isInput, _pSoundManager);
				_pTitleMenu->DeleteImage();
				_pSoundManager->DeleteSETitleMenu();
				_pSoundManager->LoadSEDeckSelect();
				_pDeckSelectBG->SetDraw(true);
				break;

			case GameState::DeckSelectFinish:
				_pSoundManager->DeleteSEDeckSelect();

				_fadeCount = 0;
				
				Fade::Reset();
				Fade::Start();

				if (_pSoundManager != nullptr) {
					_pSoundManager->PlaySECommon(SoundManager::SECommon::Fade);
					_pSoundManager->FadeStopBgm(Parameter::_FadeInFrame + Parameter::_FadeWaitFrame);
				}
				break;

			case GameState::PreStart:
				_pDeckSelectBG->DeleteImage();
				_pDeckSelect1->DeleteImage();
				_pDeckSelect2->DeleteImage();
				_pInGameBG->Init();
				_pTurnNumber->Init();
				_pPlayer1->Init(_isInput, _pSoundManager);
				_pPlayer2->Init(_isInput, _pSoundManager);
				_pInGameBG->SetDraw(true);
				_pPlayer1->SetDeck(_pDeckSelect1->GetDeck());
				_pPlayer2->SetDeck(_pDeckSelect2->GetDeck());

				if (_pSoundManager != nullptr) {
					_pSoundManager->LoadSEInGame();
					_pSoundManager->PlayBgm(SoundManager::BGM::InGame);
				}
				break;

			case GameState::Start:
				_turnNum++;
				_pTurnNumber->SetNumber(_turnNum);
				_pPlayer1->SetTimer();
				_pPlayer2->SetTimer();
				_pFirst.reset();
				_pFirst = nullptr;
				_pSecond.reset();
				_pSecond = nullptr;
				break;

			case GameState::SetMagicNumber:
				SetMagicNumber();
				break;

			case GameState::Waiting: 
				break;

			case GameState::PreAttackNumberFirst:
				_pFirst->SetAttack();
				break;

			case GameState::StartCutInFirst:
				//_pFirst->StartCutIn(CutIn::Type::Horizontal , false);
				_pFirst->StartCutIn(CutIn::Type::Horizontal, false);
				break;

			case GameState::PreAttackNumberSecond:
				if (SetAttackHp(true)) {
					_pSecond->SetAttack();
				}
				else {
					SetResult();
				}
				break;

			case GameState::StartCutInSecond:
				//_pSecond->StartCutIn(CutIn::Type::Horizontal, false);
				_pSecond->StartCutIn(CutIn::Type::Vertical, false);

				break;

			case GameState::Judgment:
				if (SetAttackHp(false)) {
					TurnFinish();
				}
				else {
					SetResult();
				}
				break;

			case GameState::PreResult:
				break;

			case GameState::ReStart:
				break;

			case GameState::GameEnd:
				return false;
				break;

			default:
				break;
			}
		}
	}

	if (_state1.now == GameState::Waiting && _state2.now == GameState::Waiting) {
		_state1.now = GameState::PreAttackNumberFirst;
		_state2.now = GameState::PreAttackNumberFirst;
	}

	return true;
}

void Game::CheckNextAttackState(GameState state, GameState& state1, GameState& state2) {

	if (_state1.now >= state && _state2.now >= state) {
		return;
	}

	if (state1 == state || state2 == state) {
		state1 = state;
		state2 = state;
	}
}

void Game::SetFirstAndSecont(bool is1P) {

	if (_pFirst == nullptr) {
		_pFirst = is1P ? _pPlayer1 : _pPlayer2;
		_pFirst->SetFirstAttack(true);

		_pSecond = is1P ? _pPlayer2 : _pPlayer1;
		_pSecond->SetFirstAttack(false);
	}
}

bool Game::SetAttackHp(bool first) {

	auto attackPlayer = first ? _pFirst : _pSecond;
	auto hpPlayer = first ? _pSecond : _pFirst;

	int attack = attackPlayer->GetAttack();
	int hp = hpPlayer->GetHp();

	hp -= attack;

	if (hp < 0) {
		hp = 0;
	}

	hpPlayer->SetHp(hp);

	return hp > 0;
}

void Game::SetMagicNumber() {
	
	int randam = MTRandom::Get(30, 90);

	_pPlayer1->SetMagicNumber(randam);
	_pPlayer2->SetMagicNumber(randam);
}

void Game::TurnFinish() {

	_pPlayer1->TurnInit();
	_pPlayer2->TurnInit();

	_state1.now = GameState::ReStart;
	_state2.now = GameState::ReStart;
}

void Game::SetResult() {

	_state1.now = GameState::PreResult;
	_state2.now = GameState::PreResult;
}
