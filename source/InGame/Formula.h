#pragma once
#include "../ObjectBace.h"
#include <vector>
#include "../Select.h"
#include "../Sign.h"
#include "../Card.h"

class Formula :public ObjectBase {   //式の後ろで描画したいと言われたら
public:
	Formula(bool is1P);
	virtual ~Formula();

	void Draw() override;
	bool IsAdd(Select* select);   //Fieldクラスから追加できるかチェック
	bool Add(Select* select,int history);
	bool GetCancel(Select*& select,int& history );
	bool IsCard(Select* select);
	bool IsSign(Select* select);
	bool IsCheck();
	bool IsChangeAnswer() {
		return _change;
	}
	int GetAnswer() {
		return _answer;
	}
	void GetDiscard(std::vector<Card*>& discardList);
	void TurnInit();

private:
	int GetPriority(Select* select);
	bool Convert();
	bool Calculation(float& answer);
	float Calculation(float num0, Sign* sign, float num1);
	void SetPosition();
	Point GetSignSize(Sign::Kind kind);
	bool HasBeginBracket();
	bool HasEndBracket();
	bool IsBracketFinish();
	bool IsBracket(Sign* sign);
	bool IsBeginBracket(Sign* sign);
	bool IsEndBracket(Sign* sign);
	void ProcessConvert();

	std::vector<Select*> _vReversePolishNotation;   // 数字を入れていく
	std::vector<Select*> _vFormula;
	std::vector<int> _vHistory;   //手札のインデックスを覚えておく

	float _floatAnswer;
	int _answer;
	bool _complet;
	bool _change;
	bool _is1P;
	
};