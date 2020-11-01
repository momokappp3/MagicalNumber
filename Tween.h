#pragma once
#include "Types.h"

class Tween {
public:
	Tween();
	virtual ~Tween();

	enum class Type {
		Linear = 0,   //íºê¸
		SineStart,
		SineEnd,
		Max,
	};

	bool SetTween(Point start, Point end, int flame, Type type = Type::Linear);
	void Process();
	void Reset() {
		_isStart = false;
		_isEnd = false;
	}

	bool IsStart() {
		return _isStart;
	}
	bool IsEnd() {
		return _isEnd;
	}
	Point GetPosition() {
		return _position;
	}

	float GetRate() {
		return _rate;
	}

private:

	Point _start;
	Point _end;
	Point _position;

	Type _type;
	int _frame;
	int _timer;

	float _rate;

	bool _isStart;
	bool _isEnd;
};