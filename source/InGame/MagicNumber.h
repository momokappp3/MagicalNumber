#pragma once

#include "../ObjectBace.h"
#include "../DrawNumber.h"
#include <memory>

class MagicNumber : public ObjectBase {
public:
	MagicNumber(bool is1P);
	virtual ~MagicNumber();

	bool Init();
	bool SetNumber(int num);
	void Draw()override;

private:

	std::unique_ptr<DrawNumber> _pDrawNumber;

	Point _point;

	unsigned long _handle;
	bool _is1P;
};
