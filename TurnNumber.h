#pragma once
#include <memory>
#include"DrawNumber.h"
#include"Types.h"

class TurnNumber {
public:
	TurnNumber();
	virtual ~TurnNumber();

	bool Init();
	bool SetNumber(int num);
	void Draw();

private:
	std::unique_ptr<DrawNumber> _pDrawNumber;

	Point _point;

	unsigned long _handle;
};