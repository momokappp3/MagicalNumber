#include "CutIn.h"
#include <tchar.h>
#include "ResourceServer.h"
#include "DxLib.h"
#include "Parameter.h"

CutIn::CutIn(bool is1P) {

	_pSoundManager = nullptr;

	_handle.fill(-1);
	_type = Type::Max;

	_point = { 0,0 };

	_width = 0;
	_height = 0;
	_finishCount = 0;

	_is1P = is1P;
	_endBG = false;
	_drawTypeLeftUp = false;
	_wait = false;
	_finish = false;
}

CutIn::~CutIn() {
}

bool CutIn::Init(Type type, std::shared_ptr<SoundManager>& soundManager) {

	if (type == Type::Max) {
		return false;
	}

	if (soundManager == nullptr) {
		return false;
	}

	_pSoundManager = soundManager;

	_type = type;

	const TCHAR* fileName = nullptr;

	std::array<const TCHAR*, ImageNum> vertical1P = {
		_T("png/attack/H1PBack.png"),_T("png/attack/H1PIn.png")
	};

	std::array<const TCHAR*, ImageNum> horizontal1P = {
		_T("png/attack/W1PBack.png"),_T("png/attack/W1PIn.png")
	};

	std::array<const TCHAR*, ImageNum> vertical2P = {
		_T("png/attack/H2PBack.png"),_T("png/attack/H2PIn.png")
	};

	std::array<const TCHAR*, ImageNum> horizontal2P = {
		_T("png/attack/W2PBack.png"),_T("png/attack/W2PIn.png")
	};

	std::array<const TCHAR*, ImageNum>* useImage = nullptr;

	switch (type){
	case CutIn::Type::Vertical:

		useImage = _is1P ? &vertical1P : &vertical2P;
		break;

	case CutIn::Type::Horizontal:

		useImage = _is1P ? &horizontal1P : &horizontal2P;
		break;

	default:
		return false;
	}

	for (int i = 0; i < ImageNum; i++) {
		int handle = ResourceServer::LoadGraph((*useImage)[i]);

		if (handle == -1) {
			return false;
		}

		_handle[i] = handle;
	}
	
	if (GetGraphSize(_handle[0], &_width, &_height) == -1) {
		return false;
	}

	return true;
}

bool CutIn::Start(bool leftUp) {
	
	_drawTypeLeftUp = leftUp;
	_wait = false;
	_finish = false;

	_finishCount = 0;

	_tweenCarcter.Reset();

	return StartBG(leftUp);
}

void CutIn::Process() {

	_tweenBG.Process();
	_tweenCarcter.Process();

	if (!_endBG && _tweenBG.IsEnd()) {
		_endBG = true;
		StartCharcter(_drawTypeLeftUp);
	}

	if (_tweenCarcter.IsEnd()) {
		if (!_wait) {
			_wait = true;
		}
	}
}

void CutIn::Draw() {
	DrawBG();
	DrawCharcter();
}

void CutIn::TurnInit() {

	_finishCount = 0;

	_endBG = false;
	_drawTypeLeftUp = false;
	_wait = false;
	_finish = false;

	_tweenBG.Reset();
	_tweenCarcter.Reset();
}

bool CutIn::Finish() {
	if (_finish || !IsWait()) {
		return false;
	}

	_finish = true;

	_tweenCarcter.Reset();

	Point start = { 0, 0 };
	Point end = { 0,0 };

	switch (_type) {
	case CutIn::Type::Vertical:
		if (_is1P) {
			start.x = Parameter::_cutInVerticalPosition1P.x;
			start.y = Parameter::_cutInVerticalPosition1P.y;
			end.x = Parameter::_cutInVerticalPosition1P.x;
			end.y = _drawTypeLeftUp ? (Parameter::_cutInVerticalPosition1P.y + _height) : (Parameter::_cutInVerticalPosition1P.y - _height);
		}
		else {
			start.x = Parameter::_cutInVerticalPosition2P.x;
			start.y = Parameter::_cutInVerticalPosition2P.y;
			end.x = Parameter::_cutInVerticalPosition2P.x;
			end.y = _drawTypeLeftUp ? (Parameter::_cutInVerticalPosition2P.y + _height) : (Parameter::_cutInVerticalPosition2P.y - _height);
		}

		_pSoundManager->PlaySEInGame(SoundManager::InGame::CutInVerticalStart);
		break;

	case CutIn::Type::Horizontal:
		start.x = Parameter::_cutInHorizontalPosition.x;
		start.y = Parameter::_cutInHorizontalPosition.y;
		end.x = _drawTypeLeftUp ? (Parameter::_cutInHorizontalPosition.x + _width) : (Parameter::_cutInHorizontalPosition.x - _width);
		end.y = Parameter::_cutInHorizontalPosition.y;

		_pSoundManager->PlaySEInGame(SoundManager::InGame::CutInHorizontalStart);
		break;

	default:
		return false;
	}

	_tweenCarcter.SetTween(start, end, 30, Tween::Type::SineEnd);

	return true;
}

bool CutIn::StartBG(bool leftUp) {

	_endBG = false;
	_tweenBG.Reset();

	Point start = { 0, 0 };
	Point end = { 0,0 };
	int cutinWidth = 0;

	switch (_type){
	case CutIn::Type::Vertical:
		cutinWidth = _width / 2;

		if (_is1P) {
			start.x = Parameter::_cutInVerticalPosition1P.x + cutinWidth;
			end.x = Parameter::_cutInVerticalPosition1P.x + _width;
		}
		else {
			start.x = Parameter::_cutInVerticalPosition2P.x + cutinWidth;
			end.x = Parameter::_cutInVerticalPosition2P.x + _width;
		}

		_pSoundManager->PlaySEInGame(SoundManager::InGame::CutInVerticalStart);
		break;

	case CutIn::Type::Horizontal:
		start.y = Parameter::_cutInHorizontalPosition.y + (_height / 2);
		end.y = Parameter::_cutInHorizontalPosition.y + _height;

		_pSoundManager->PlaySEInGame(SoundManager::InGame::CutInHorizontalStart);
		break;

	default:
		return false;
	}

	_tweenBG.SetTween(start, end, 40, Tween::Type::SineEnd);

	return true;
}

bool CutIn::StartCharcter(bool leftUp) {

	_tweenCarcter.Reset();

	Point start = { 0, 0 };
	Point end = { 0,0 };

	switch (_type) {
	case CutIn::Type::Vertical:
		if (_is1P) {
			start.x = Parameter::_cutInVerticalPosition1P.x;
			start.y = leftUp ? (Parameter::_cutInVerticalPosition1P.y - _height) : (Parameter::_cutInVerticalPosition1P.y + _height);
			end.x = Parameter::_cutInVerticalPosition1P.x;
			end.y = Parameter::_cutInVerticalPosition1P.y;
		}
		else {
			start.x = Parameter::_cutInVerticalPosition2P.x;
			start.y = leftUp ? (Parameter::_cutInVerticalPosition2P.y - _height) : (Parameter::_cutInVerticalPosition2P.y + _height);
			end.x = Parameter::_cutInVerticalPosition2P.x;
			end.y = Parameter::_cutInVerticalPosition2P.y;
		}

		_pSoundManager->PlaySEInGame(SoundManager::InGame::CutInVerticalEnd);
		break;

	case CutIn::Type::Horizontal:
		start.x = leftUp ? ( Parameter::_cutInHorizontalPosition.x -_width) : (Parameter::_cutInHorizontalPosition.x + _width);
		start.y = Parameter::_cutInHorizontalPosition.y;
		end.x = Parameter::_cutInHorizontalPosition.x;
		end.y = Parameter::_cutInHorizontalPosition.y;

		_pSoundManager->PlaySEInGame(SoundManager::InGame::CutInHorizontalEnd);
		break;

	default:
		return false;
	}

	_tweenCarcter.SetTween(start,end,30,Tween::Type::SineStart);

	return true;
}

void CutIn::DrawBG() {

	if (!_tweenBG.IsStart()) {
		return;
	}

	Point leftUp = { 0,0 };
	Point rightDown = { 0,0 };
	int alphaBG = 0;

	if (!GetBGPosition(leftUp, rightDown, alphaBG)) {
		return;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaBG);
	DrawExtendGraph(leftUp.x, leftUp.y, rightDown.x, rightDown.y, _handle[0], TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void CutIn::DrawCharcter() {

	if (!_tweenBG.IsEnd()) {
		return;
	}

	Point leftUp = { 0,0 };
	int alphaCharcter = 0;

	if (!GetCharcterPosition(leftUp, alphaCharcter)) {
		return;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaCharcter);
	DrawGraph(leftUp.x, leftUp.y, _handle[1], TRUE) ;
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

bool CutIn::GetBGPosition(Point& leftUp, Point& rigthDown, int& alpha) {

	Point point = _tweenBG.GetPosition();
	int center = 0, diff = 0;

	switch (_type){
	case CutIn::Type::Vertical:
		if (_is1P) {
			leftUp.y = Parameter::_cutInVerticalPosition1P.y;
			rigthDown.y = Parameter::_cutInVerticalPosition1P.y + _height;
			center = Parameter::_cutInVerticalPosition1P.x + (_width / 2);
		}
		else {
			leftUp.y = Parameter::_cutInVerticalPosition2P.y;
			rigthDown.y = Parameter::_cutInVerticalPosition2P.y + _height;
			center = Parameter::_cutInVerticalPosition2P.x + (_width / 2);
		}

		diff = point.x - center;
		leftUp.x = center - diff;
		rigthDown.x = point.x;

		break;
	case CutIn::Type::Horizontal:
		leftUp.x = Parameter::_cutInHorizontalPosition.x;
		center = Parameter::_cutInHorizontalPosition.y + (_height / 2);
		diff = point.y - center;

		leftUp.y = center - diff;
		rigthDown.x = Parameter::_cutInHorizontalPosition.x + _width;
		rigthDown.y = point.y;

		break;
	default:
		return false;
	}

	if (_finish && _tweenCarcter.IsEnd()) {

		alpha = 255 - _finishCount;

		_finishCount += 6;
		
		if (_finishCount > 255) {
			_finishCount = 255;
		}
	}
	else {
		alpha = 255;
	}
	
	return true;
}

bool CutIn::GetCharcterPosition(Point& leftUp, int& alpha) {

	leftUp = _tweenCarcter.GetPosition();

	if (_finish) {
		alpha = 255;
	}
	else {
		alpha = static_cast<int>(255.0f * _tweenCarcter.GetRate());
	}

	return true;
}