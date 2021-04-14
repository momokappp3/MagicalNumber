#pragma once

#include "../ObjectBace.h"

class DeckSelectBG : public ObjectBase {
public:
	DeckSelectBG();
	virtual ~DeckSelectBG();

	bool Init();
	void DeleteImage();

};