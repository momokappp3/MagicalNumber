#pragma once
#include "MagicNumber.h"
#include <memory>
#include "Deck.h"
#include "Field.h"
#include "Types.h"
#include <functional>
#include "Input.h"
#include "Formula.h"
#include "Answer.h"
#include "Hp.h"
#include "SoundManager.h"
#include "Timer.h"
#include "Attack.h"
#include "CutIn.h"
#include "Tween.h"

class Player {
public:
	Player(bool is1P);
	virtual ~Player();

	bool Init(std::function<bool(Input::INPUT, bool)> isInput, std::shared_ptr<SoundManager>& soundManager);
	GameState Process(GameState state);
	void ProcessField(GameState state);
	void ProcessFieldPlaying(GameState state);
	void Draw();
	void DrawFront();
	bool SetDeck(std::list<Card*>& deck);
	bool SetHandCard();
	bool SetMagicNumber(int magicNumber);
	bool InitHp();
	bool SetTimer();
	bool SetHp(int hp);
	int GetHp();
	int GetAttack();
	void SetAttack();
	void TurnInit();
	void StartCutIn(CutIn::Type type, bool leftUp);

	void SetFirstAttack(bool first) {
		_isFirstAttack = first;
	}

private:
	void CheckTimeUp();
	GameState ProcessPreAttack(GameState state, bool first);
	GameState ProcessCutIn(GameState state, bool first);
	bool ProcesResult();
	void DrawResult();

	std::function<bool(Input::INPUT, bool)> _pIsInput;
	std::shared_ptr<SoundManager> _pSoundManager;

	std::unique_ptr<Deck> _pDeck;   //ユニークポインタは＊つけなくてOK
	std::unique_ptr<Field> _pField;
	std::unique_ptr<Formula> _pFormula;
	std::unique_ptr<Answer> _pAnswer;
	std::unique_ptr<MagicNumber> _pMagicNumber;
	std::unique_ptr<Hp> _pHp;
	std::unique_ptr<Timer> _pTimer;
	std::unique_ptr<Attack> _pAttack;
	std::unique_ptr<CutIn> _pCutInVertical;
	std::unique_ptr<CutIn> _pCutInHorizontal;

	GameState _isPlayingConfom;
	CutIn::Type _cutInType;

	Tween _resulTween;
	Point _resultLT;
	Point _resultRB;

	int _magicNumber;
	int _cutInWaitCount;
	int _resultHandle;

	bool _is1P;
	bool _isFirstAttack;
	bool _isResult;
};
