#pragma once
#include <memory>
#include "Types.h"
#include "ObjectBace.h"
#include "DrawNumber.h"

class Timer : public ObjectBase {
public:
	Timer(bool is1P);
	virtual ~Timer();

	bool Init();
	void Process() override;
	bool SetStart(int startNum);
	void Draw() override;
	void SetStop(bool stop) {
		_isStop = stop;
	}

	bool GetIsEnd() {
		return _isEnd;
	}

private:
	bool SetNumber(int num);

	std::unique_ptr<DrawNumber> _pDrawNumber;

	Point _point;

	int _startTime;
	int _startNum;
	int _nowNum;

	unsigned long _handle;

	bool _isStart;
	bool _isStop;
	bool _isEnd;
	bool _is1P;
};