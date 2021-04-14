#pragma once
#include <memory>
#include "CutIn.h"
#include "../DrawNumber.h"
#include "../Use/Types.h"
#include "../Use/Tween.h"

class Attack {
public:
	Attack(bool is1P);
	virtual ~Attack();

	bool Init(std::shared_ptr<SoundManager>& soundManager);
	void Process();
	void Draw();
	void TurnInit();
	void SetAttackBace(bool isAttack);

	bool IsEnd();

private:
	std::unique_ptr<DrawNumber> _pDrawNumber;
	std::unique_ptr<CutIn> _pVertical;
	std::unique_ptr<CutIn> _pHorizontal;
	Tween _tween;

	unsigned long _handle;

	Point _point;

	float _ovalWidth;
	float _ovalHeight;

	int _waitStartCount;
	int _waitEndCount;

	bool _isStart;
	bool _isAttack;
	bool _isDrawNumber;
	bool _is1P;
};