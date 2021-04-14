#pragma once
#include <array>
#include "DxLib.h"

class Input {
public:
	Input();
	virtual ~Input();

	enum class INPUT {

		//���т��O��̏����A�ύX�s��
		Up = 0,
		Down,
		Left,
		Right,

		Start,
		Back,

		LeftStick,
		RightStick,

		LB,
		RB,

		A = 12,   //�o�O Dx���C�u������12�����`
		B,
		X,
		Y,

		Max
	};

	static constexpr int KeyMax = static_cast<int>(INPUT::Max);   //enumclass����int�ɕϊ�

	void Process();

	bool IsInput(INPUT type,bool p1) {   //�Ăяo���ă{�^�������͂��ꂽ��

		int index = static_cast<int>(type);   //enum�̃L�[�̓Y�����ɕϊ�

		//�����œ����Ă������̂��w��@������������ture
		return p1 ? _aPushEdge1p[index] : _aPushEdge2p[index];
	}
	
private:                                 //�n�����l�����̂܂ܓn���Q�Ɠn��
	void CheckPushEdge(XINPUT_STATE& input, XINPUT_STATE& lastinput, std::array<bool, KeyMax>& edge);  
	
	//KeyMaxEnum�N���X����int�ɕϊ���������
	std::array<bool, KeyMax> _aPushEdge1p;   //enum�N���X�̂ǂ̃L�[�������ꂽ���i�[
	std::array<bool, KeyMax> _aPushEdge2p;

	XINPUT_STATE _lastInput1p;
	XINPUT_STATE _lastInput2p;

};

