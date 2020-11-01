#pragma once
#include "ObjectBace.h"
#include "Tween.h"

class TitleCharacter : public ObjectBase{
public:
	TitleCharacter();
	virtual ~TitleCharacter();

	enum class CharKind{
		MA = 0,
		JI,
		KA,
		RU,
		NA,
		NN,
		BA,
		AA,

		Max
	};

	bool Init(CharKind charkind, int delay);
	void Process() override;
	void SetFloatMode();
	void SetEndMode();
	void DeleteImage();

	int GetAlpha() {
		return _alpha;
	}

private:
	void ProcessDelay();
	void ProcessFloating();
	void ProcessEnding();
	void SetFloating();
	void SetEnding();
	bool SetStart();

	Tween _tween;

	Point _point;
	int _delay;
	int _alpha;

	bool _start;
	bool _isFloat;
	bool _floatUp;
	bool _isEnd;
};
