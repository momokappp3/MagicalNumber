#pragma once
#include "ObjectBace.h"

class Select : public ObjectBase{
public:
	Select(TYPE type);
	virtual ~Select();

	void SetDraw(bool isDraw)override;
	bool SetSelect(bool select);
	virtual bool SetSelectXY(int x, int y);
	void SetSelectDrawInfo(DrawInfo info);

protected:
	int _selectIndex;

};