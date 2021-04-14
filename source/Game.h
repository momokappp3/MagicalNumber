#pragma once
#include "InGame/Player.h"
#include "Use/Types.h"
#include "Input.h"
#include "DeckSelect/DeckSelect.h"
#include "Title/Title.h"
#include "Use/SoundManager.h"
#include "Title/TitleMenu.h"
#include "InGame/InGameBG.h"
#include "DeckSelect/DeckSelectBG.h"
#include "InGame/TurnNumber.h"

class Game {
public:
	Game();
	virtual ~Game();

	bool Init();
	bool Process();
	void Draw();

	bool IsTitle() {
		return  (_state1.now == GameState::Title) && (_state2.now == GameState::Title);
	}

	bool IsTitleMenu() {
		return (_state1.now == GameState::TitleMenu) && (_state2.now == GameState::TitleMenu);
	}

	bool IsDeckSelect() {
		return (_state1.now >= GameState::DeckSelect) && (_state2.now >= GameState::DeckSelect) &&
			(_state1.now <= GameState::DeckSelectFade) && (_state2.now <= GameState::DeckSelectFade);
	}

	bool IsPlay() {
		return (_state1.now >= GameState::PreStart) && (_state2.now >= GameState::PreStart) &&
			(_state1.now <= GameState::Result) && (_state2.now <= GameState::Result);
	}

private:
	void ProcessTitle(GameState& state1, GameState& state2);
	void ProcessTitleMenu(GameState& state1, GameState& state2);
	void ProcessDeckSelect(GameState& state1, GameState& state2);
	void ProcessPlay(GameState& state1, GameState& state2);
	bool CheckState(GameState state1, GameState state2);
	void CheckNextAttackState(GameState state, GameState& state1, GameState& state2);
	void SetFirstAndSecont(bool is1P);
	bool SetAttackHp(bool first);
	void SetMagicNumber();
	void TurnFinish();
	void SetResult();

	TransState _state1;
	TransState _state2;

	std::unique_ptr<Input> _pInput;
	std::shared_ptr<SoundManager> _pSoundManager;

	std::unique_ptr<Title> _pTitle;
	std::unique_ptr<TitleMenu> _pTitleMenu;

	std::unique_ptr<DeckSelectBG> _pDeckSelectBG;

	std::unique_ptr<DeckSelect> _pDeckSelect1;
	std::unique_ptr<DeckSelect> _pDeckSelect2;

	std::unique_ptr<InGameBG> _pInGameBG;
	std::unique_ptr<TurnNumber> _pTurnNumber;

	std::shared_ptr<Player> _pPlayer1;
	std::shared_ptr<Player> _pPlayer2;

	std::shared_ptr<Player> _pFirst;
	std::shared_ptr<Player> _pSecond;

	std::function<bool(Input::INPUT, bool)> _isInput;

	int _fadeCount;
	int _turnNum;

};