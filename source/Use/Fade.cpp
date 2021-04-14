#include "Fade.h"
#include "../ResourceServer.h"
#include "DxLib.h"
#include "Parameter.h"

Tween Fade::_tween;
unsigned int Fade::_color;

//int Fade::_handle;
int Fade::_alpha;
int Fade::_wait;

bool Fade::_isFade;
bool Fade::_isFadeOut;
bool Fade::_end;

Fade::Fade() {

}

Fade::~Fade() {
}

bool Fade::Init(){

	/*
	_handle = ResourceServer::LoadGraph(_T("png/back/fade.png"));

	if (_handle == -1) {
		return false;
	}
	*/

	_color = GetColor(0, 0, 0);

	_alpha = 0;
	_wait = 0;

	_isFade = false;
	_isFadeOut = false;
	_end = false;

	return true;
}

void Fade::Process() {

	if (!_isFade || _end) {
		return;
	}

	_tween.Process();

	if (_tween.IsStart()) {
		_alpha = _tween.GetPosition().x;
	}

	if (_tween.IsEnd()) {
		if (_isFadeOut) {
			_end = true;
		}
		else {
			_wait++;

			if (_wait >= Parameter::_FadeWaitFrame) {
				_isFadeOut = true;
				_tween.Reset();
				_tween.SetTween({ 255,0 }, { 0,0 }, Parameter::_FadeOutFrame, Tween::Type::SineStart);
			}
		}
	}
}

void Fade::Draw() {

	if (!_isFade || _end) {
		return;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);
	//DrawGraph(0, 0, _handle, TRUE);
	DrawBox(0, 0, 1920, 1080, _color, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Fade::Start() {

	if (_isFade) {
		return;
	}

	_isFade = true;

	_isFadeOut = false;
	_end = false;

	_alpha = 0;
	_wait = 0;

	_tween.SetTween({ 0 ,0 }, { 255,0 }, Parameter::_FadeInFrame, Tween::Type::SineStart);

}

void Fade::Reset() {

	_tween.Reset();

	_isFade = false;
	_isFadeOut = false;
	_end = false;

	_alpha = 0;
	_wait = 0;
}

bool Fade::IsEnd() {
	return _end;
}

void Fade::SetColor(int Red, int Green, int Blue) {

	_color = GetColor(Red, Green, Blue);
}