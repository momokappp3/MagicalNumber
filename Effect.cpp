#include "Effect.h"
#include "EffekseerForDXLib.h"

Effect::Effect() {

	_point = { 0,-150 };

	_effectResourceHandle = -1;
	_playingEffectHandle = -1;

	_isLoop = false;
	_isEnd = false;
}

Effect::~Effect() {
}

bool Effect::Init(const TCHAR* fileName, float magnification, bool isLoop) {

	_effectResourceHandle = LoadEffekseerEffect(fileName, magnification);

	if (_effectResourceHandle == -1) {
		return false;
	}

	_isLoop = isLoop;

	return true;
}

void Effect::Start() {

	if (_effectResourceHandle == -1) {
		return;
	}

	// エフェクトを再生する。
	_playingEffectHandle = PlayEffekseer2DEffect(_effectResourceHandle);
	DrawEffekseer2D_Begin();

}

void Effect::Stop() {

	if (_playingEffectHandle == -1) {
		return;
	}

	StopEffekseer2DEffect(_playingEffectHandle);
	DrawEffekseer2D_End();

	_playingEffectHandle = -1;
}

void Effect::Process() {

	if (_playingEffectHandle == -1) {
		return;
	}

	ProcessPoint();

	// 再生中のエフェクトを移動する。
	SetPosPlayingEffekseer2DEffect(_playingEffectHandle, _point.x, _point.y, 0);

	if (_isLoop) {
			//再生中か
		if (IsEffekseer2DEffectPlaying(_playingEffectHandle) == -1) {
			_playingEffectHandle = PlayEffekseer2DEffect(_effectResourceHandle);
			DrawEffekseer2D_Begin();
		}
	}
	else {
		StopEffekseer2DEffect(_playingEffectHandle);
		DrawEffekseer2D_End();
	}

}

void Effect::ProcessPoint() {
}

void Effect::Delete() {

	Stop();

	if (_effectResourceHandle != -1) {
		DeleteEffekseerEffect(_effectResourceHandle);
	}	
}