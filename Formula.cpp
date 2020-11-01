#include "Formula.h"
#include "Parameter.h"
#include "DxLib.h"

Formula::Formula(bool is1P) : ObjectBase(TYPE::Formula) {

	_vReversePolishNotation.clear();
	_vFormula.clear();
	_vHistory.clear();

	_floatAnswer = 0.0f;
	_answer = 0;
	_complet = false;
	_change = false;
	_is1P = is1P;
}

Formula::~Formula() {
}

void Formula::Draw() {

	for (int i = 0; i < _vFormula.size(); i++) {
		if (_vFormula[i] == nullptr) {
			continue;
		}
		_vFormula[i]->Draw();
	}
}

bool Formula::IsAdd(Select* select) {

	if (_vFormula.size() == 0) {
		
		if (IsCard(select)) {
			return true;
		}

		if (IsSign(select)) {

			Sign* sign = dynamic_cast<Sign*>(select);
			
			if (IsBracket(sign)) {
				return true;
			}
		}
	}
	else {
		Sign* sign = dynamic_cast<Sign*>(select);
		Select* end = _vFormula.back();

		if (IsCard(end)) {
			if (IsSign(select)) {
				if (!IsBeginBracket(sign)) {
					return true;
				}
			}
		}
		else {
			Sign* endSign = dynamic_cast<Sign*>(end);

			if (IsBeginBracket(endSign)) {
				if (IsCard(select)) {
					return true;
				}
			}
			else if(IsEndBracket(endSign)){
				if (IsSign(select)) {
					if (!IsBracket(sign)) {
						return true;
					}
				}
			}
			else {
				if (IsCard(select)) {   //バグ　orだと失敗
					return true;
				}

				if (IsBeginBracket(sign)) {
					if (!HasBeginBracket()) {   //括弧は1回だけしか使えない仕様なので
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool Formula::Add(Select* select, int history) {

	if (!IsAdd(select)) {
		return false;
	}

	_vFormula.push_back(select);
	_vHistory.push_back(history);  //手札のインデックスを取っておく

	SetPosition();   //addされたら
	ProcessConvert();

	return true;
}

void Formula::ProcessConvert() {

	bool complet = false;
	bool change = false;

	if (IsCheck()) {
		if (Convert()) {
			complet = Calculation(_floatAnswer);

			if (complet) {
				int temp = static_cast<int>(_floatAnswer);

				if (_answer != temp) {
					_answer = temp;
					change = true;
				}
			}
		}
	}

	_complet = complet;
	_change = change;
}

bool Formula::GetCancel(Select*& select, int& history) {

	if (_vFormula.size() == 0) {
		return false;
	}

	select = _vFormula.back();
	history = _vHistory.back();

	_vFormula.pop_back();
	_vHistory.pop_back();

	SetPosition();
	ProcessConvert();

	return true;
}

bool Formula::IsCheck() {

	if (_vFormula.size() < 3) {   //式が成り立つ最小の構成が三つ
		return false;
	}

	Sign* signEnd = dynamic_cast<Sign*>(_vFormula.back());   //最後を取得

	if (signEnd == nullptr) {
		return IsBracketFinish();   //カードの時始まり括弧がある場合式が成り立っていない
	}

	if (IsEndBracket(signEnd)) {
		return true;
	}

	return false;
}

void Formula::GetDiscard(std::vector<Card*>& discardList) {

	for (int i = 0; i < _vFormula.size(); i++) {
		Select* select = _vFormula[i];

		if (IsCard(select)) {
			Card* card = dynamic_cast<Card*>(select);

			if (card != nullptr) {
				discardList.push_back(card);
			}
		}
		else {
			delete select;
		}
		_vFormula[i] = nullptr;
	}
	_vFormula.clear();
	_vHistory.clear();
	_vReversePolishNotation.clear();
}

void Formula::TurnInit() {

	_floatAnswer = 0.0f;
	_answer = 0;
	_complet = false;
	_change = false;
}

int Formula::GetPriority(Select* select) {

	Sign* sign = dynamic_cast<Sign*>(select);

	if (sign != nullptr) {
		if (sign->GetKind() == Sign::Kind::Times || sign->GetKind() == Sign::Kind::Divide) {
			return 4;
		}

		if (sign->GetKind() == Sign::Kind::Plus || sign->GetKind() == Sign::Kind::Minus) {
			return 5;
		}
	}

	return 99;
}

bool Formula::Convert() {

	std::vector<Select*> stack;

	_vReversePolishNotation.clear();

	for (auto itr = _vFormula.begin(); itr != _vFormula.end(); itr++) {
		Select* select = *itr;   //_lFormula

		if (IsCard(select)) {   //数字だったら
			_vReversePolishNotation.push_back(select);
		}
		else {
			Sign* sign = dynamic_cast<Sign*>(select);

			if (sign == nullptr) {
				return false;
			}

			if (IsEndBracket(sign)) {
				while (true) {
					Select* pop = stack.back();   //末尾の参照を取得
					stack.pop_back();   //末尾の要素の削除

					Sign* popSign = dynamic_cast<Sign*>(pop);

					if (popSign != nullptr) {
						if (IsBeginBracket(popSign)) {
							break;
						}
					}
					_vReversePolishNotation.push_back(pop);   //取得したものを入れる
				}
			}
			else if (stack.empty() || IsBeginBracket(sign)) {
				stack.push_back(select);
			}
			else {
				bool stackPushBack = false;

				while (!(stack.empty())) {
					auto pop = stack.back();

					if (GetPriority(select) > GetPriority(pop)) {   //優先順位の確認
						_vReversePolishNotation.push_back(pop);   //popのが優先度が高かった処理　計算vectorに
						stack.pop_back();
					}
					else {
						stack.push_back(select);
						stackPushBack = true;
						break;
					}
				}

				if (!stackPushBack) {
					stack.push_back(select);
				}
			}
		}
	}

	while (!(stack.empty())) {
		auto pop = stack.back();

		_vReversePolishNotation.push_back(pop);
		stack.pop_back();
	}
	return true;
}

bool Formula::Calculation(float& answer) {

	std::vector<Select*> stack;
	std::vector<Card*> deleteList;

	for (auto itr = _vReversePolishNotation.begin(); itr != _vReversePolishNotation.end(); itr++) {
		Select* select = *itr;

		if (IsCard(select)) {

			Card* card = dynamic_cast<Card*>(select);

			if (card == nullptr) {
				return false;
			}

			float valueFloat = static_cast<float>(card->GetValue());

			card->SetFloatValue(valueFloat);
			stack.push_back(select);
		}
		else {
			Sign* sign = dynamic_cast<Sign*>(select);

			if (sign == nullptr) {
				return false;
			}

			Select* pop0 = stack.back();
			stack.pop_back();

			Card* card0 = dynamic_cast<Card*>(pop0);

			if (card0 == nullptr) {
				return false;
			}

			Select* pop1 = stack.back();
			stack.pop_back();

			Card* card1 = dynamic_cast<Card*>(pop1);

			if (card1 == nullptr) {
				return false;
			}

			float valueFloat0 = card0->GetFloatValue();   //Cardクラスの __floatValue
			float valueFloat1 = card1->GetFloatValue();
			float value = Calculation(valueFloat1, sign, valueFloat0); //計算されたものがvalueに
			Card* temp = new Card;

			temp->SetFloatValue(value);

			stack.push_back(temp);
			deleteList.push_back(temp);
		}
	}	

	if (stack.size() == 0) {
		return false;
	}

	Select* pop = stack.back();

	Card* card = dynamic_cast<Card*>(pop);

	if (card == nullptr) {
		return false;
	}

	answer = card->GetFloatValue();

	for (int i = 0; i < deleteList.size(); i++) {

		delete deleteList[i];
	}

	deleteList.clear();
	_vReversePolishNotation.clear();

	return true;
}

float Formula::Calculation(float num0, Sign* sign, float num1) {

	if (sign == nullptr) {   //nullだったら0.0fを返す
		return 0.0f;
	}

	Sign::Kind signType = sign->GetKind();

	switch (signType) {
	case Sign::Kind::Plus:
		return num0 + num1;
	case Sign::Kind::Minus:
		return num0 - num1;
	case Sign::Kind::Times:
		return num0 * num1;
	case Sign::Kind::Divide:
		return num0 / num1;
	default:
		return 0.0f;
	}
}

void Formula::SetPosition() {   //formulaクラスの全ての座標を決定しなおす

	Point point = _is1P ? Parameter::_formulaPosition1P : Parameter::_formulaPosition2P;
	Point tmep = point;

	for (int i = 0; i < _vFormula.size(); i++) {   //指定のポジションからインターバルぶん離してxyをセット
		_vFormula[i]->SetSelect(false);

		Point signOffSet = { 0,0 };
		int  nowX = tmep.x;
		bool skipInterval = false;

		if (IsSign(_vFormula[i])) {
			signOffSet = Parameter::_formulaSignOffSet;

			Sign* sign = dynamic_cast<Sign*>(_vFormula[i]);

			if (IsBeginBracket(sign)) {
				Point size = GetSignSize(Sign::Kind::BeginBracket);

				signOffSet.x -= static_cast<int>(static_cast<float>(size.x) * 1.5f);
				signOffSet.y -= size.y / 6;
				skipInterval = true;
			}
			else if (IsEndBracket(sign)) {
				Point size = GetSignSize(Sign::Kind::BeginBracket);

				signOffSet.x -= size.x;
				signOffSet.y -= size.y / 6;
				skipInterval = true;
			}
		}

		_vFormula[i]->SetXY(nowX + signOffSet.x, tmep.y + signOffSet.y);

		if (!skipInterval) {
			tmep.x = nowX + Parameter::_formulaIntervalWidth;
		}
	}
}

Point Formula::GetSignSize(Sign::Kind kind) {

	Point size = { 0,0 };

	for (int i = 0; i < _vFormula.size(); i++) {
		if (IsSign(_vFormula[i])) {
			Sign* sign = dynamic_cast<Sign*>(_vFormula[i]);

			if (sign == nullptr) {
				continue;
			}

			if( sign->GetKind() == kind){
				DrawInfo info;

				if (_vFormula[i]->GetDrawInfo(0, info)) {
					if (GetGraphSize(info.handle, &(size.x), &(size.y)) == 0) {

						return size;  //ここでreturn してたらとれてる
					}
				}
			}
		}
	}

	return size;
}

bool Formula::HasBeginBracket() {

	for (int i = 0; i < _vFormula.size(); i++) {
		if (IsSign(_vFormula[i])) {

			Sign* sign = dynamic_cast<Sign*>(_vFormula[i]);

			if (IsBeginBracket(sign)) {
				return true;
			}
		}
	}

	return false;
}

bool Formula::HasEndBracket() {

	for (int i = 0; i < _vFormula.size(); i++) {
		if (IsSign(_vFormula[i])) {

			Sign* sign = dynamic_cast<Sign*>(_vFormula[i]);

			if (IsEndBracket(sign)) {
				return true;
			}
		}
	}

	return false;
}

bool Formula::IsBracketFinish() {

	if (!HasBeginBracket()) {
		return true;   //使われていない成り立っている
	}

	return HasEndBracket();   //endBracketがある成り立っている
}

bool Formula::IsCard(Select* select) {

	if (select == nullptr) {
		return false;
	}

	          //selectがカードだった時(ObjectBaceの関数)
	return (select->GetType() == ObjectBase::TYPE::Card);
}

bool Formula::IsSign(Select* select) {
	
	if (select == nullptr) {
		return false;
	}

	return (select->GetType() == ObjectBase::TYPE::Sign);
}

bool Formula::IsBracket(Sign* sign) {

	if (sign == nullptr) {
		return false;
	}

	return IsBeginBracket(sign) || IsEndBracket(sign);
}

bool Formula::IsBeginBracket(Sign* sign) {

	if (sign == nullptr) {
		return false;
	}

	return (sign->GetKind() == Sign::Kind::BeginBracket);
}

bool Formula::IsEndBracket(Sign* sign) {

	if (sign == nullptr) {
		return false;
	}

	return (sign->GetKind() == Sign::Kind::EndBracket);
}