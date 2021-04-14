#include "SoundManager.h"
#include "DxLib.h"

SoundManager::SoundManager() {

	_vBgmFileName.clear();
	_vDelayPlay.clear();
	_vSECommonHandle.clear();
	_vSETitleHandle.clear();
	_vSETitleMenuHandle.clear();
	_vSEDeckSelectHandle.clear();
	_vSEInGameHandle.clear();

	_nowPlayBgm = BGM::Max;

	_bgmFadeCount = 0;
	_bgmFadeFrame = 0.0f;
}

SoundManager::~SoundManager() {
}

bool SoundManager::Init() {

	_vBgmFileName.push_back(_T("mp3/Title.mp3"));
	_vBgmFileName.push_back(_T("mp3/InGame.mp3"));

	return true;
}

// ==============================================================
//
// Common
//
// ==============================================================

bool SoundManager::LoadSECommon() {

	std::vector<const TCHAR*> SEname = {
		_T("mp3/SE/move.mp3"), _T("mp3/SE/Abutton.mp3"), _T("mp3/SE/Bbutton.mp3"), _T("mp3/SE/fade.mp3"),
		 _T("mp3/SE/DeckSelectOK.mp3"), _T("mp3/SE/DeckSelectCancel.mp3")
	};

	return LoadSE(SEname, _vSECommonHandle);
}

void SoundManager::DeleteSECommon() {

	DeleteSE(_vSECommonHandle);
}

bool SoundManager::PlaySECommon(SECommon type ,int delayFrame) {

	if (type >= SECommon::Max) {
		return false;
	}

	int index = static_cast<int>(type);

	if (index >= _vSECommonHandle.size()) {
		return false;
	}

	return PlaySE(_vSECommonHandle[index], delayFrame);
}

// ==============================================================
//
// Title
//
// ==============================================================

bool SoundManager::LoadSETitle() {

	std::vector<const TCHAR*> SEname = {
		_T("mp3/SE/titleEffect.mp3")
	};

	return LoadSE(SEname, _vSETitleHandle);
}

void SoundManager::DeleteSETitle() {

	DeleteSE(_vSETitleHandle);
}

bool SoundManager::PlaySETitle(SETitle type, int delayFrame) {

	if (type >= SETitle::Max) {
		return false;
	}

	int index = static_cast<int>(type);

	if (index >= _vSETitleHandle.size()) {
		return false;
	}

	return PlaySE(_vSETitleHandle[index], delayFrame);
}

// ==============================================================
//
// TitleMenu
//
// ==============================================================

bool SoundManager::LoadSETitleMenu() {

	std::vector<const TCHAR*> SEname = {
	_T("mp3/SE/menuEffect.mp3"), _T("mp3/SE/help.mp3")
	};

	return LoadSE(SEname, _vSETitleMenuHandle);
}

void SoundManager::DeleteSETitleMenu() {

	DeleteSE(_vSETitleMenuHandle);
}

bool SoundManager::PlaySETitleMenu(SETitleMenu type, int delayFrame) {

	if (type >= SETitleMenu::Max) {
		return false;
	}

	int index = static_cast<int>(type);

	if (index >= _vSETitleMenuHandle.size()) {
		return false;
	}

	return PlaySE(_vSETitleMenuHandle[index], delayFrame);
}

// ==============================================================
//
// DeckSelect
//
// ==============================================================

bool SoundManager::LoadSEDeckSelect() {

	std::vector<const TCHAR*> SEname = {
	_T("mp3/SE/deckSelectMove.mp3")
	};

	return LoadSE(SEname, _vSEDeckSelectHandle);
}

void SoundManager::DeleteSEDeckSelect() {

	DeleteSE(_vSEDeckSelectHandle);
}

bool SoundManager::PlaySEDeckSelect(SEDeckSelect type, int delayFrame) {

	if (type >= SEDeckSelect::Max) {
		return false;
	}

	int index = static_cast<int>(type);

	if (index >= _vSEDeckSelectHandle.size()) {
		return false;
	}

	return PlaySE(_vSEDeckSelectHandle[index], delayFrame);
}

// ==============================================================
//
// InGame
//
// ==============================================================


bool SoundManager::LoadSEInGame() {

	std::vector<const TCHAR*> SEname = {
	_T("mp3/SE/cardSelect.mp3"), _T("mp3/SE/CutInHorizontalStart.mp3"), _T("mp3/SE/CutInHorizontalEnd.mp3"),
	_T("mp3/SE/CutInVerticalStart.mp3"), _T("mp3/SE/CutInVerticalEnd.mp3")
	};

	return LoadSE(SEname, _vSEInGameHandle);
}

void SoundManager::DeleteSEInGame() {

	DeleteSE(_vSEInGameHandle);
}

bool SoundManager::PlaySEInGame(InGame type, int delayFrame) {

	if (type >= InGame::Max) {
		return false;
	}

	int index = static_cast<int>(type);

	if (index >= _vSEInGameHandle.size()) {
		return false;
	}

	return PlaySE(_vSEInGameHandle[index], delayFrame);
}

void SoundManager::Process() {

	if (_nowPlayBgm != BGM::Max && _bgmFadeCount > 0) {
		int volume = static_cast<int>(255.0f * static_cast<float>(_bgmFadeCount) / _bgmFadeCount);

		SetVolumeMusic(volume);

		_bgmFadeCount--;

		if (_bgmFadeCount == 0) {
			_bgmFadeFrame = 0.0f;

			DirectStopBgm();
		}
	}

	if (_vDelayPlay.size() > 0) {  //DelayPlayがあるならdelayPlayのカウントを減らしてSE再生
		for (int i = _vDelayPlay.size() - 1; i >= 0; i--) {
			_vDelayPlay[i].delayFrame--;

			if (_vDelayPlay[i].delayFrame == 0) {
				PlaySoundMem(_vDelayPlay[i].handle, DX_PLAYTYPE_BACK);
				_vDelayPlay.erase(_vDelayPlay.begin() + i);
			}
		}
	}
}

bool SoundManager::PlayBgm(BGM bgm) {

	if (bgm >= BGM::Max && CheckMusic() != 0) {
		return false;
	}

	_nowPlayBgm = bgm;
	_bgmFadeCount = 0;

	int index = static_cast<int>(bgm);

	int ret = PlayMusic(_vBgmFileName[index], DX_PLAYTYPE_LOOP);

	return ret == 0;
}

void SoundManager::DirectStopBgm() {

	StopMusic();
	
	_nowPlayBgm = BGM::Max;
}

void SoundManager::FadeStopBgm(int fadeFrame) {

	_bgmFadeFrame = static_cast<float>(fadeFrame);
	_bgmFadeCount = fadeFrame;
}

bool SoundManager::LoadSE(std::vector<const TCHAR*>& fileNameList, std::vector<int>& handleList) {
	
	for (int i = 0; i < fileNameList.size(); i++) {
		int handle = LoadSoundMem(fileNameList[i]);

		if (handle == -1) {
			return false;
		}

		handleList.push_back(handle);
	}

	return true;
}

void SoundManager::DeleteSE(std::vector<int>& handleList) {

	for (int i = 0; i < handleList.size(); i++) {
		int handle = handleList[i];

		if (handle != -1) {
			DeleteSoundMem(handle);
		}
	}

	handleList.clear();
}

bool SoundManager::PlaySE(int handle, int delayFrame) {

	if (handle == -1) {
		return false;
	}

	if (delayFrame == 0) {
		return PlaySoundMem(handle, DX_PLAYTYPE_BACK);  //delayFrameが0ならそのまま音データの再生
	}
	else {
		DelayPlay delayPlay = { delayFrame ,handle };

		_vDelayPlay.push_back(delayPlay);

		return true;
	}
}