#pragma once
/*
‹¤’Ê‚Ìˆ—‚ğ‚±‚±‚Å‚â‚é
*/

#include "Use/Types.h"
#include <vector>

class ObjectBase {
public:
	enum class TYPE {

		Title = 0,
		TitleCharcter,
		TitleMenu,
		DeckSelectBG,
		DeckSelect,
		ChoiceCard,
		Card,
		Sign,
		InGameBG,
		Field,
		Formula,
		Answer,
		MagicNumber,
		Timer,
		HP,

		Max
	};

	ObjectBase(TYPE type);   //ã‚É‚È‚¢‚Æ“{‚ç‚ê‚é
	virtual ~ObjectBase();

	static constexpr int InvalidNum = -1;   //–³Œø‚È’l

	virtual void SetDraw(bool isDraw);   //on off‚Å‚«‚é‚æ‚¤‚É
	virtual void Process();
	virtual void Draw();

	virtual bool SetXY(int x, int y);
	bool GetDrawInfo(int index,DrawInfo& info);

	Point GetXY();

	TYPE GetType() {
		return _type;
	}

	int GetValue() {
		return _value;
	}

protected:
	TYPE _type;
	int _value;   //Card‚Ì”Ô†
	
	std::vector<DrawInfo> _vDrawInfo;   //ƒnƒ“ƒhƒ‹‚Æxy•`‰æ‚·‚é‚½‚ß‚Ìî•ñ‚ğ“ü‚ê‚Ä‚¢‚é

};