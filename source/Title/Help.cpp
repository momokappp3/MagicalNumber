#include "Help.h"
#include "../ResourceServer.h"
#include "DxLib.h"
#include "../Use/Parameter.h"

Help::Help(){

	_pIsInput = nullptr;
	_pSoundManager = nullptr;

	_areaLeftUp = {Parameter::_ScreenHalfWidth, Parameter::_ScreenHalfHeight};
	_areaRightDown = _areaLeftUp;

	int quarterWidth = Parameter::_ScreenHalfWidth / 2;
	int quarterHeight = Parameter::_ScreenHalfHeight / 2;

	_firstRect = { Parameter::_ScreenHalfWidth + 10, Parameter::_ScreenHalfHeight + quarterHeight };
	_secondRect = { Parameter::_ScreenHalfWidth + quarterWidth, Parameter::_ScreenHalfHeight + quarterHeight };
	_thirdRect = { Parameter::_ScreenHalfWidth + quarterWidth, Parameter::_ScreenHeight };


	_rectType = RectType::Zero;

	_handle = -1;
	_imageWidth = 0;
	_imageHeight = 0;

	_isStart = false;
	_isEnd = false;
	_end = false;

}

Help::~Help() {
}

bool Help::Init(std::function<bool(Input::INPUT, bool)> isInput, std::shared_ptr<SoundManager>& soundManager) {

	if (isInput == nullptr) {
		return false;
	}

	_pIsInput = isInput;

	if (soundManager == nullptr) {
		return false;
	}

	_pSoundManager = soundManager;

	_handle = ResourceServer::LoadGraph(_T("png/back/help1.png"));   //–³’n


	if (_handle == -1) {
		return false;
	}

	if (GetGraphSize(_handle, &_imageWidth, &_imageHeight) == -1) {
		return false;
	}

	return true;
}

void Help::Process() {

	if (!_isStart) {
		return;
	}

	if (!_end && _isEnd && _pIsInput != nullptr) {
		if (_pIsInput(Input::INPUT::A, true)) {
			_end = true;

			Close();

			if (_pSoundManager != nullptr) {
				_pSoundManager->PlaySECommon(SoundManager::SECommon::AButton);
			}
		}
	}
	_tween.Process();

	if (_tween.IsStart()) {
		_areaRightDown = _tween.GetPosition();

		Point size = { _areaRightDown.x - Parameter::_ScreenHalfWidth , _areaRightDown.y - Parameter::_ScreenHalfHeight };

		_areaLeftUp = { Parameter::_ScreenHalfWidth - size.x, Parameter::_ScreenHalfHeight - size.y };
	}

	if (_tween.IsEnd()) {
		
		Point start = { 0,0 };
		Point end = { 0,0 };

		switch (_rectType){
		case Help::RectType::Zero:
			break;

		case Help::RectType::First:
			start = _firstRect;

			if (_isEnd) {
				end = _firstRect;
			}
			else {
				end = _secondRect;
			}
			break;

		case Help::RectType::Second:
			start = _secondRect;

			if (_isEnd) {
				end = _firstRect;
			}
			else {
				end = _thirdRect;
			}
			break;
		case Help::RectType::Third:
			start = _thirdRect;

			if (_isEnd) {
				end = _secondRect;
			}
			else {
				end = { Parameter::_ScreenWidth , Parameter::_ScreenHeight };
			}
			break;

		case Help::RectType::Final:
			_isEnd = true;

			break;
		default:
			break;
		}

		_tween.Reset();

		int frame = _isEnd ? Parameter::_MenuHelpCloseFrame : Parameter::_MenuHelpOpenFrame;
		Tween::Type type = _isEnd ? Parameter::_MenuHelpCloseType : Parameter::_MenuHelpOpenType;

		_tween.SetTween(start,end, frame, type);

		if (!_isEnd && _rectType < RectType::Final) {
			int now = static_cast<int>(_rectType);

			now++;

			_rectType = static_cast<Help::RectType>(now);

			if (_pSoundManager != nullptr) {
				_pSoundManager->PlaySETitleMenu(SoundManager::SETitleMenu::HelpOpen);
			}
		}

		if (_end && _isEnd && _rectType > RectType::Zero) {

			int now = static_cast<int>(_rectType);

			now--;

			_rectType = static_cast<Help::RectType>(now);

			if (_pSoundManager != nullptr) {
				_pSoundManager->PlaySETitleMenu(SoundManager::SETitleMenu::HelpOpen);
			}
		}
	}
}

void Help::Draw() {

	if (!_isStart || _handle == -1 || _imageWidth == 0 || _imageHeight == 0) {
		return;
	}

	SetDrawArea(_areaLeftUp.x, _areaLeftUp.y, _areaRightDown.x, _areaRightDown.y);

	int halfWidth = _imageWidth / 2;
	int halfHeight = _imageHeight / 2;

	DrawGraph(Parameter::_ScreenHalfWidth - halfWidth, Parameter::_ScreenHalfHeight - halfHeight, _handle, TRUE);

	SetDrawArea(0, 0, Parameter::_ScreenWidth, Parameter::_ScreenHeight);
}

void Help::Open() {

	if (_isStart) {
		return;
	}

	_isStart = true;

	_areaLeftUp = { Parameter::_ScreenHalfWidth, Parameter::_ScreenHalfHeight };
	_areaRightDown = _areaLeftUp;
	_rectType = RectType::First;

	_tween.Reset();
	_tween.SetTween(_areaRightDown, _firstRect, Parameter::_MenuHelpOpenFrame, Parameter::_MenuHelpOpenType);
}

void Help::Close() {

	if (!_isEnd) {
		return;
	}

	_areaLeftUp = { 0,0 };
	_areaRightDown = { Parameter::_ScreenWidth, Parameter::_ScreenHeight };
	_rectType = RectType::Third;

	_tween.Reset();
	_tween.SetTween(_areaRightDown, _thirdRect, Parameter::_MenuHelpCloseFrame, Parameter::_MenuHelpCloseType);
}

void Help::Reset() {

	_areaLeftUp = { Parameter::_ScreenHalfWidth, Parameter::_ScreenHalfHeight };
	_areaRightDown = _areaLeftUp;

	_rectType = RectType::Zero;

	_isStart = false;
	_isEnd = false;
	_end = false;
}

void Help::DeleteImage(){

	if (_handle == -1) {
		return;
	}

	ResourceServer::DeleteGraph(_handle);

	_handle = -1;
}

