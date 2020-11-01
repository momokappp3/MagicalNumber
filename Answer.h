#pragma once
#include "ObjectBace.h"
#include "DrawNumber.h"
#include<memory>

class Answer: public ObjectBase {
public:
	Answer(bool is1P);
	virtual ~Answer();

	bool Init();
	bool SetNumber(int num);
	void Draw()override;
	void TurnInit();
	void SetNoAnswer(bool noAnswer) {
		_isNoAnswer = noAnswer;
	}

private:

	std::unique_ptr<DrawNumber> _pDrawNumber;

	Point _point;

	unsigned long _handle;
	bool _is1P;
	bool _isNoAnswer;

};
