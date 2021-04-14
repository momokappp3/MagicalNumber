#pragma once
#include "../ObjectBace.h"
#include <functional>
#include "../Input.h"
#include <list>
#include "SelectCard.h"
#include <memory>
#include "../Use/Tween.h"
#include "../Use/SoundManager.h"

class ChoiceCard : public ObjectBase{
public:
	ChoiceCard(bool is1P);
	virtual ~ChoiceCard();

	bool Init(std::function<bool(Input::INPUT, bool)> isInput, std::shared_ptr<SoundManager>& soundManager,bool isPremium);
	void Process() override;
	void Draw() override;
	void DeleteImage();

	void SetSelect(bool isSelect);

	bool IsAnimation() {
		return (_animation == Animation::Left) || (_animation == Animation::Right);
	}

	int GetSelectCardNumber() {
		return _selectCardNumber;
	}

	void ResetSelectCardNumber() {
		_selectCardNumber = 0;
	}

	void SetMaxDeck(bool maxDeck) {
		_isMaxDeck = maxDeck;
	}

	void SetCancelCard(int number);

private:
	enum class Animation {
		None = 0,
		Left,
		Right,
		Max
	};

	bool InitGraph();
	void CheckInput();
	void ProcessAnimation();
	SelectCard* GetSelectCard();

	std::list<SelectCard*> _plCard;
	std::function<bool(Input::INPUT, bool)> _pIsInput;
	std::shared_ptr<SoundManager> _pSoundManager;

	Tween _tween;
	Animation _animation;

	Point _animationBase;

	int _selectCardNumber;

	bool _is1P;
	bool _isSelect;   //trueÇ…Ç»Ç¡ÇƒÇ¢ÇÈÇ∆Ç±ÇÎÇæÇØèàóùÇ™Ç≈Ç´ÇÈ
	bool _isPremium;
	bool _isMaxDeck;
};