#include "Field.h"
#include "../Use/Parameter.h"
#include "../Sign.h"
#include "../ResourceServer.h"

Field::Field(bool is1P) :ObjectBase(TYPE::Field) {

	_onlySelectPoint = { 0,0 };

	_pIsInput = nullptr;
	_pSoundManager = nullptr;

	_vPoint.clear();
	_vSelect.clear();

	for (int i = 0; i < Sign::SignMax; i++) {
		Sign* sign = new Sign;
		Sign::Kind kind = static_cast <Sign::Kind>(i);
		;
		if (!sign->Init(kind)) {
			continue;
		}
		_vSelect.push_back(sign);
	}

	_select = Sign::SignMax;   //選択の初期位置
	_onSelect = -1;
	_addCount = 0;

	_is1P = is1P;
	_isFinish = false;
}

Field::~Field() {

	for (int i = 0; i < _vSelect.size(); i++) {
		if (_vSelect[i] != nullptr) {
			delete _vSelect[i];
			_vSelect[i] = nullptr;
		}
	}
	_vSelect.clear();
}

bool Field::Init(std::function<bool(Input::INPUT, bool)> isInput, std::shared_ptr<SoundManager>& soundManager) {

	if (isInput == nullptr) {
		return false;
	}
	_pIsInput = isInput;

	if (soundManager == nullptr) {
		return false;
	}

	_pSoundManager = soundManager;

	_vPoint.clear();   //保険

	SetSignPosition();   //座標を入れている
	SetHandPosition();

	for (int i = 0; i < Parameter::_handNum; i++) {
		_vSelect.push_back(nullptr);
	}

	for (int i = 0; i < Sign::SignMax; i++) {
		Point point = _vPoint[i];

		_vSelect[i]->SetXY(point.x,point.y);
		_vSelect[i]->SetSelectXY(point.x, point.y);

		//α用の仮の処理
		_vSelect[i]->SetDraw(true);
	}

	_handleSelect = ResourceServer::LoadGraph(_T("png/select/waku.png"));

	if (_handleSelect == -1) {   //ハンドルに入っていないか確かめる
		return false;
	}

	return true;
}

void Field::TurnInit() {

	_onlySelectPoint = { 0, 0 };

	_select = Sign::SignMax;   //選択の初期位置
	_onSelect = -1;
	_addCount = 0;

	_isFinish = false;
}

void Field::Process() {

	for (int i = Sign::SignMax; i < _vSelect.size(); i++) {   
		Select* select = _vSelect[i];
	
		if (select == nullptr) {
			continue;
		}

		Card* card = dynamic_cast<Card*>(select);

		if (card == nullptr || card->IsSetXY()) {
			continue;
		}

		Point point = _vPoint[i];   //配列を記号ぶんずらす

		card->SetXY(point.x, point.y);
		card->SetSelectXY(point.x, point.y);

		//α用の仮
		card->SetDraw(true);
	}

	ProcessSelect();  //バグ　SetSelectをよんでいるから常に呼ば
}

void Field::Draw() {

	for (int i = 0; i < Sign::SignMax; i++) {
		Sign* sign = dynamic_cast<Sign*>(_vSelect[i]);  //サインクラス

		if (sign == nullptr) {
			continue;
		}
		sign->Draw();   //ポインタだから
	}

	for (int i = Sign::SignMax; i < _vSelect.size(); i++) {
		Card* card = dynamic_cast<Card*>(_vSelect[i]);   //カードクラス

		if (card == nullptr) {
			continue;
		}
		card->Draw();   //ObjectBaseのDrawを呼び出し描画 手札ぶん
	}

	if (_onlySelectPoint.x != 0 && _onlySelectPoint.y != 0) {
		DrawGraph(_onlySelectPoint.x, _onlySelectPoint.y, _handleSelect, TRUE);
	}
}

void Field::SetSignPosition() {

	auto signBigen = _is1P ? Parameter::_vSignPosition1P.begin() : Parameter::_vSignPosition2P.begin();
	auto signEnd = _is1P ? Parameter::_vSignPosition1P.end() : Parameter::_vSignPosition2P.end();

	for (auto itr = signBigen; itr != signEnd; itr++) {

		Point point = *itr;

		_vPoint.push_back(point);   //フィールドの場所情報にパラメータの座標をセット
	}
}

void Field::SetHandPosition() {

	auto handBigen = _is1P ? Parameter::_vHandPosition1P.begin() : Parameter::_vHandPosition2P.begin();
	auto handEnd = _is1P ? Parameter::_vHandPosition1P.end() : Parameter::_vHandPosition2P.end();

	for (auto itr = handBigen; itr != handEnd; itr++) {

		Point point = *itr;   //_vPointに構造体を入れるため

		_vPoint.push_back(point);
	}
}

void Field::ProcessSelect() {

	if (_pIsInput == nullptr || _isFinish) {
		return;
	}

	bool se = false;

	//右
	if (_pIsInput(Input::INPUT::Right,_is1P)) {
		if (!(IsRightEdge(_select))) {
			_select++;
			se = true;
		}
	}
	
	//左
	if (_pIsInput(Input::INPUT::Left, _is1P)) {
		if (!(IsLeftEdge(_select))) {
			_select--;
			se = true;
		}
	}

	//上
	if (_pIsInput(Input::INPUT::Up, _is1P)) {
		if (!(IsSign(_select))) {
			if (IsNumberBottom(_select)) {
				_select -= 5;
			}
			else {
				if (_select == 6) {
					_select = 0;
				}
				else if (_select == 10) {
					_select = 5;
				}
				else {
					_select -= 5;
				}
			}
			se = true;
		}
	}

	//下
	if (_pIsInput(Input::INPUT::Down, _is1P)) {
		if (!(IsNumberBottom(_select))) {
			if (IsSign(_select)) {
				if (_select == 0 || _select == 1) {
					_select = 6;
				}
				else if (_select == 4 || _select == 5) {
					_select = 9;
				}
				else {
					_select += 5;
				}
			}
			else {
				_select += 5;
			}

			se = true;
		}
	}

	if (se && _pSoundManager != nullptr) {
		_pSoundManager->PlaySECommon(SoundManager::SECommon::Move);
	}

	_onSelect = -1;

	if (_pIsInput(Input::INPUT::B, _is1P)) {
		if (_addCount < Parameter::_formulaMaxCard) {
			_onSelect = _select;

			if (_pSoundManager != nullptr) {
				_pSoundManager->PlaySEInGame(SoundManager::InGame::CardSelect);
			}
		}
	}
	
	_onlySelectPoint = { 0,0 };

	for (int i = 0; i < _vSelect.size(); i++) {  //添え字と_selectが同じとき true
		if (_vSelect[i] == nullptr) {
			if (_select == i) {
				_onlySelectPoint = _vPoint[i];
			}
			continue;
		}

		_vSelect[i]->SetSelect(i == _select);
	}	
}

bool Field::IsSign(int select) {
	return select < Sign::SignMax;
}

bool Field::IsNumber(int select) {
	return select >= Sign::SignMax;
}

bool Field::IsNumberBottom(int select) {
	return select >= 11;
}

bool Field::IsRightEdge(int select) {
	return (select == 5) || (select == 10) || (select == 15);
}

bool Field::IsLeftEdge(int select) {
	return (select == 0) || (select == 6) || (select == 11);
}