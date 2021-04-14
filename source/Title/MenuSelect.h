#pragma once
#include "MenuString.h"

class MenuSelect : public MenuString {
public:
	MenuSelect();
	virtual ~MenuSelect();

	bool Init(Kind kind, int delay) override;
	void Process() override;

	bool SetSelect(MenuString::Kind kind);

};