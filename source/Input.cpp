#include "Input.h"

Input::Input() {

	_aPushEdge1p.fill(false);
	_aPushEdge2p.fill(false);

	_lastInput1p = {};   //0初期化
	_lastInput2p = {};
}

Input::~Input() {
}

void Input::Process() {

	XINPUT_STATE input1p;
	GetJoypadXInputState(DX_INPUT_PAD1, &input1p);

	XINPUT_STATE input2p;
	GetJoypadXInputState(DX_INPUT_PAD2, &input2p);

	CheckPushEdge(input1p, _lastInput1p, _aPushEdge1p);  //引数で渡ってきたものを渡ってきた配列に
	CheckPushEdge(input2p, _lastInput2p, _aPushEdge2p);

	_lastInput1p = input1p;   //メンバ変数に前の状態を取る
	_lastInput2p = input2p;
}

    //XINPUT_STATEに入っているtrue falseをbool型のarrayに変換
void Input::CheckPushEdge(XINPUT_STATE& input, XINPUT_STATE& lastinput, std::array<bool, KeyMax>& edge) {

	edge.fill(false);

	for (int i = 0; i < 16; i++) {
		bool button = input.Buttons[i] == 1 && lastinput.Buttons[i] == 0;

		edge[i] = button;   //12の時どうなっているのか
	}
}

