#include "Input.h"

Input::Input() {

	_aPushEdge1p.fill(false);
	_aPushEdge2p.fill(false);

	_lastInput1p = {};   //0‰Šú‰»
	_lastInput2p = {};
}

Input::~Input() {
}

void Input::Process() {

	XINPUT_STATE input1p;
	GetJoypadXInputState(DX_INPUT_PAD1, &input1p);

	XINPUT_STATE input2p;
	GetJoypadXInputState(DX_INPUT_PAD2, &input2p);

	CheckPushEdge(input1p, _lastInput1p, _aPushEdge1p);  //ˆø”‚Å“n‚Á‚Ä‚«‚½‚à‚Ì‚ğ“n‚Á‚Ä‚«‚½”z—ñ‚É
	CheckPushEdge(input2p, _lastInput2p, _aPushEdge2p);

	_lastInput1p = input1p;   //ƒƒ“ƒo•Ï”‚É‘O‚Ìó‘Ô‚ğæ‚é
	_lastInput2p = input2p;
}

    //XINPUT_STATE‚É“ü‚Á‚Ä‚¢‚étrue false‚ğboolŒ^‚Ìarray‚É•ÏŠ·
void Input::CheckPushEdge(XINPUT_STATE& input, XINPUT_STATE& lastinput, std::array<bool, KeyMax>& edge) {

	edge.fill(false);

	for (int i = 0; i < 16; i++) {
		bool button = input.Buttons[i] == 1 && lastinput.Buttons[i] == 0;

		edge[i] = button;   //12‚Ì‚Ç‚¤‚È‚Á‚Ä‚¢‚é‚Ì‚©
	}
}

