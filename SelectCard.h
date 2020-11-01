#pragma once
#include "Card.h"
#include "DrawNumber.h"
#include <memory>

class SelectCard : public Card{
public:
	SelectCard();
	virtual ~SelectCard();

	bool Init(int cardNum, bool isPremium = false, bool isChoice = false, bool isSmall = false);
	bool ProcessSelectCount();

	void Process()override;
	void Draw()override;

	void SetSelectCount(int selectCount) {
		_selectCount = selectCount;
	}

	int GetSelectCount() {
		return _selectCount;
	}

private:
	std::unique_ptr<DrawNumber> _pDrawNumber;
	int _selectCount;   //ç°âΩñáëIÇŒÇÍÇƒÇ¢ÇÈÇÃÇ©

	unsigned long _handle;


};