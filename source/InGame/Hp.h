#pragma once
#include "../ObjectBace.h"
#include "../Use/Tween.h"

class Hp : public ObjectBase{
public:
	Hp(bool is1P);
	virtual ~Hp();

	bool Init();
	void Process() override;
	void Draw() override;

	void InitHP(int hp) {
		_hp = hp;
		_nowHp = hp;
		_nextHp = hp;
	}

	void SetHp(int hp);

	int GetHp() {
		return _nowHp;
	}

private:
	void GetHpArea(int hp, Point& rightDown);

	Point _point;
	Point _leftUp;
	Point _rightDown;

	Tween _tween;

	int _hp;
	int _nowHp;
	int _nextHp;
	int _handle;
	int _imageWidth;
	int _imageHeight;

	bool _is1P;

};