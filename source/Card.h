#pragma once
#include "Select.h"   //SelectÇÕObjectBaseÇåpè≥ÇµÇƒÇ¢ÇÈ

class Card : public Select{
public:
	Card();
	virtual ~Card();

	static constexpr int CardMax = 10;

	bool Init(int cardNum, bool isPremium = false, bool isChoice = false, bool isSmall = false);
	virtual void DeleteImage();

	bool IsSetXY() {
		return _isSetXY;
	}

	bool SetXY(int x, int y) override {   
		ObjectBase::SetXY(x,y);
	   _isSetXY = true;
	   return _isSetXY;
	}

	void ClearSetXY() {
		_isSetXY = false;
	}

	void SetFloatValue(float value) {
		_floatValue = value;
	}

	float GetFloatValue() {
		return _floatValue;
	}

	bool IsPremium() {
		return _isPremium;
	}

private:
	bool SetDrawInfo(int cardNum, bool isPremium, DrawInfo& info, bool isChoice = false,bool isSmall = false);
	bool SetDrawInfoSelect(DrawInfo& info, bool isSmall = false);

	float _floatValue;
	bool _isSetXY;
	bool _isPremium;
	
};