#pragma once
/*
共通の処理をここでやる
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

	ObjectBase(TYPE type);   //上にないと怒られる
	virtual ~ObjectBase();

	static constexpr int InvalidNum = -1;   //無効な値

	virtual void SetDraw(bool isDraw);   //on offできるように
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
	int _value;   //Cardの番号
	
	std::vector<DrawInfo> _vDrawInfo;   //ハンドルとxy描画するための情報を入れている

};