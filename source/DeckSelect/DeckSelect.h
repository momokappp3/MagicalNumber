#pragma once
#include <functional>
#include "../ObjectBace.h"
#include "../Input.h"
#include "ChoiceCard.h"
#include <memory>
#include <list>
#include "../Use/SoundManager.h"

class DeckSelect : public ObjectBase {
public:
	DeckSelect(bool is1P);
	virtual ~DeckSelect();

	enum class Selected {
		NormalCard = 0,
		PremiumCard,
		DeckUp,
		DeckDown,

		Max
	};

/*
#ifdef _DEBUG   //デバッグビルドの時だけ有効になる
	void SetDebugFinish();

#endif // DEBUG

*/
	bool Init(std::function<bool(Input::INPUT, bool)> isInput, std::shared_ptr<SoundManager>& soundManager);
	GameState Process(GameState state);
	void SetDraw(bool isDraw) override;
	void Draw() override;
	void DeleteImage();

	std::list<Card*>& GetDeck() {
		return _lDeck;	
	}

private:

	bool InitGraph();
	void ProcessSelect();
	void ProcessDeckSelect();
	void ProcessDeckCancel();
	void ShiftDeck(int cancelIndex,Point cancelXY);
	bool SetDeck(int cardNum, bool isPremium);

	std::function<bool(Input::INPUT, bool)> _pIsInput;
	std::shared_ptr<SoundManager> _pSoundManager;
	std::unique_ptr<ChoiceCard> _normal;
	std::unique_ptr<ChoiceCard> _premium;
	std::list<Card*> _lDeck;

	Selected _selected;

	int _selectDeckIndex;

	bool _isEnd;
	bool _is1P;
};