#pragma once
#include "../ObjectBace.h"
#include <vector>
#include "../Select.h"
#include "../Sign.h"
#include "../Card.h"

class Formula :public ObjectBase {   //���̌��ŕ`�悵�����ƌ���ꂽ��
public:
	Formula(bool is1P);
	virtual ~Formula();

	void Draw() override;
	bool IsAdd(Select* select);   //Field�N���X����ǉ��ł��邩�`�F�b�N
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

	std::vector<Select*> _vReversePolishNotation;   // ���������Ă���
	std::vector<Select*> _vFormula;
	std::vector<int> _vHistory;   //��D�̃C���f�b�N�X���o���Ă���

	float _floatAnswer;
	int _answer;
	bool _complet;
	bool _change;
	bool _is1P;
	
};