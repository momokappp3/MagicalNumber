#include "Sign.h"
#include "DxLib.h"
#include "Tchar.h"
#include <array>
#include "ResourceServer.h"

Sign::Sign() :Select(TYPE::Sign){

	_kind = Kind::Max;
}

Sign::~Sign() {
}

bool Sign::Init(Kind kind) {

	_value = static_cast<int>(kind);   //�n���Ă���kind��int�ɕϊ����ĕۑ�

	DrawInfo info;

	if (!SetDrawInfo(_value, info)) {
		return false;
	}

	_vDrawInfo.push_back(info);

	DrawInfo select;

	if (!SetDrawInfoSelect(select)) {
		return false;
	}

	Select::SetSelectDrawInfo(select);

	_kind = kind;

	return true;
}

bool Sign::SetSelectXY(int x, int y) {

	if (_selectIndex == -1) {
		return false;
	}

	_vDrawInfo[_selectIndex].xy = { x - 33,y - 33 };

	return true;
}

bool Sign::SetDrawInfo(int kind, DrawInfo& info) {

	if (kind < 0 || kind > 5) {
		return false;
	}

	const TCHAR* fileName = nullptr;

	std::array<const TCHAR*, SignMax > sign = {   //�n���Ă����L���̏��Ԃ�Ή�������
		_T("png/sign/plus.png"),_T("png/sign/minus.png"),_T("png/sign/times.png"),_T("png/sign/divide.png"),
		_T("png/sign/begin.png"),_T("png/sign/end.png"),
	};

	fileName = sign[kind];

	int handle = ResourceServer::LoadGraph(fileName);

	if (handle == -1) {
		return false;
	}

	info.handle = handle;  //�����̃Z�b�g��������D���\���ł��鏀�����ł���

	return true;   //���͎Q�Ɠn�������珑���ς���Ă���
}

bool Sign::SetDrawInfoSelect(DrawInfo& info) {

	int handle = ResourceServer::LoadGraph(_T("png/select/wakuSign.png"));

	if (handle == -1) {   //�n���h���ɓ����Ă��Ȃ����m���߂�
		return false;
	}

	info.handle = handle;  //�����̃Z�b�g��������D���\���ł��鏀�����ł���

	return true;   //���͎Q�Ɠn�������珑���ς���Ă���
}