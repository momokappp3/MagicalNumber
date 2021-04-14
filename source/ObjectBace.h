#pragma once
/*
���ʂ̏����������ł��
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

	ObjectBase(TYPE type);   //��ɂȂ��Ɠ{����
	virtual ~ObjectBase();

	static constexpr int InvalidNum = -1;   //�����Ȓl

	virtual void SetDraw(bool isDraw);   //on off�ł���悤��
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
	int _value;   //Card�̔ԍ�
	
	std::vector<DrawInfo> _vDrawInfo;   //�n���h����xy�`�悷�邽�߂̏������Ă���

};