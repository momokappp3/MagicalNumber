#include "Card.h"
#include "Types.h"
#include "Tchar.h"
#include <array>
#include "ResourceServer.h"

Card::Card():Select(TYPE::Card){   //Card���n���čs���Ă�ObjectBace�̃R���X�g���N�^����ɌĂ΂��

	_floatValue = 0.0f;
	_isSetXY = false;
	_isPremium = false;
}

Card::~Card(){
}

bool Card::Init(int cardNum, bool isPremium, bool isChoice, bool isSmall) {

	_value = cardNum;
	_isPremium = isPremium;

	DrawInfo info;   //�J�[�h�N���X�̕`����

	if (!SetDrawInfo(cardNum, isPremium, info, isChoice,isSmall)) {
		return false;
	}

	_vDrawInfo.push_back(info);   //��������

	DrawInfo select;   //�g

	if (!SetDrawInfoSelect(select,isSmall)) {
		return false;
	}

	Select::SetSelectDrawInfo(select);

	return true;
}

void Card::DeleteImage() {

	if (_vDrawInfo.size() > 0) {
		for (int i = 0; i < _vDrawInfo.size(); i++) {
			DrawInfo drawInfo = _vDrawInfo[i];

			ResourceServer::DeleteGraph(drawInfo.handle);
		}
		_vDrawInfo.clear();
	}
}

bool Card::SetDrawInfo(int cardNum, bool isPremium, DrawInfo& info, bool isChoice, bool isSmall) {

	//�J�[�h�ɂ�2��ނ��邽�߂ǂ��炩�𔻒�
	//�J�[�h�ɂ͐�����10������ǂꂩ�𔻒�

	if (cardNum <= 0 || cardNum > 10) {   //�ی�
		return false;
	}

	const TCHAR* fileName = nullptr;

	std::array<const TCHAR*, CardMax> normalCard = {
		_T("png/card/A1.png"),_T("png/card/A2.png"),_T("png/card/A3.png"),_T("png/card/A4.png"),_T("png/card/A5.png"),
		_T("png/card/A6.png"),_T("png/card/A7.png"),_T("png/card/A8.png"),_T("png/card/A9.png"),_T("png/card/A10.png"),
	};

	std::array<const TCHAR*, CardMax> premiumCard = {
		_T("png/card/B1.png"),_T("png/card/B2.png"),_T("png/card/B3.png"),_T("png/card/B4.png"),_T("png/card/B5.png"),
		_T("png/card/B6.png"),_T("png/card/B7.png"),_T("png/card/B8.png"),_T("png/card/B9.png"),_T("png/card/B10.png"),
	};

	//�J�[�h�I�����
	std::array<const TCHAR*, CardMax> choiceNormalCard = {
		_T("png/deck/A1_b.png"),_T("png/deck/A2_b.png"),_T("png/deck/A3_b.png"),_T("png/deck/A4_b.png"),_T("png/deck/A5_b.png"),
		_T("png/deck/A6_b.png"),_T("png/deck/A7_b.png"),_T("png/deck/A8_b.png"),_T("png/deck/A9_b.png"),_T("png/deck/A10_b.png"),
	};

	std::array<const TCHAR*, CardMax> choicePremiumCard = {
		_T("png/deck/B1_b.png"),_T("png/deck/B2_b.png"),_T("png/deck/B3_b.png"),_T("png/deck/B4_b.png"),_T("png/deck/B5_b.png"),
		_T("png/deck/B6_b.png"),_T("png/deck/B7_b.png"),_T("png/deck/B8_b.png"),_T("png/deck/B9_b.png"),_T("png/deck/B10_b.png"),
	};

	//��
	std::array<const TCHAR*, CardMax> choiceSmallNormalCard = {
	_T("png/decks/A1_s.png"),_T("png/decks/A2_s.png"),_T("png/decks/A3_s.png"),_T("png/decks/A4_s.png"),_T("png/decks/A5_s.png"),
	_T("png/decks/A6_s.png"),_T("png/decks/A7_s.png"),_T("png/decks/A8_s.png"),_T("png/decks/A9_s.png"),_T("png/decks/A10_s.png"),
	};

	std::array<const TCHAR*, CardMax> choiceSmallPremiumCard = {
		_T("png/decks/B1_s.png"),_T("png/decks/B2_s.png"),_T("png/decks/B3_s.png"),_T("png/decks/B4_s.png"),_T("png/decks/B5_s.png"),
		_T("png/decks/B6_s.png"),_T("png/decks/B7_s.png"),_T("png/decks/B8_s.png"),_T("png/decks/B9_s.png"),_T("png/decks/B10_s.png"),
	};

	
	int cardIndex = cardNum - 1;

	if (!isPremium) {   //�����Ȓl��������@(-1) �m�[�}��
		if (isChoice) {
			fileName = choiceNormalCard[cardIndex];
		}
		else if (isSmall) {
			fileName = choiceSmallNormalCard[cardIndex];
		}
		else {
			fileName = normalCard[cardIndex];
		}
	}
	else {
		if (isChoice) {
			fileName = choicePremiumCard[cardIndex];
		}
		else if (isSmall) {
			fileName = choiceSmallPremiumCard[cardIndex];
		}
		else {
			fileName = premiumCard[cardIndex];
		}
	}

	int handle = ResourceServer::LoadGraph(fileName);   //���[�h���Ď��ʔԍ����Q�b�g

	if (handle == -1) {   //�n���h���ɓ����Ă��Ȃ����m���߂�
		return false;
	}

	info.handle = handle;  //�����̃Z�b�g��������D���\���ł��鏀�����ł���

	return true;   //���͎Q�Ɠn�������珑���ς���Ă���
}

bool Card::SetDrawInfoSelect(DrawInfo& info, bool isSmall) {

	const TCHAR* fileName = isSmall ? (_T("png/decks/target.png")) : (_T("png/select/waku.png"));

	int handle = ResourceServer::LoadGraph(fileName);

	if (handle == -1) {   //�n���h���ɓ����Ă��Ȃ����m���߂�
		return false;
	}

	info.handle = handle;  //�����̃Z�b�g��������D���\���ł��鏀�����ł���

	return true;   //���͎Q�Ɠn�������珑���ς���Ă���
}