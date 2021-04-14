#pragma once
#include <array>
#include "DxLib.h"

class Input {
public:
	Input();
	virtual ~Input();

	enum class INPUT {

		//並びが前提の処理、変更不可
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

		A = 12,   //バグ Dxライブラリで12から定義
		B,
		X,
		Y,

		Max
	};

	static constexpr int KeyMax = static_cast<int>(INPUT::Max);   //enumclassからintに変換

	void Process();

	bool IsInput(INPUT type,bool p1) {   //呼び出してボタンが入力されたか

		int index = static_cast<int>(type);   //enumのキーの添え字に変換

		//引数で入ってきたものを指定　同じだったらture
		return p1 ? _aPushEdge1p[index] : _aPushEdge2p[index];
	}
	
private:                                 //渡した値をそのまま渡す参照渡し
	void CheckPushEdge(XINPUT_STATE& input, XINPUT_STATE& lastinput, std::array<bool, KeyMax>& edge);  
	
	//KeyMaxEnumクラスからintに変換したもの
	std::array<bool, KeyMax> _aPushEdge1p;   //enumクラスのどのキーが押されたか格納
	std::array<bool, KeyMax> _aPushEdge2p;

	XINPUT_STATE _lastInput1p;
	XINPUT_STATE _lastInput2p;

};

