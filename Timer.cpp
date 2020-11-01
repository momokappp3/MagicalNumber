#include "Timer.h"
#include "Parameter.h"
#include "DxLib.h"

Timer::Timer(bool is1P) : ObjectBase(ObjectBase::TYPE::Timer) {

	_pDrawNumber = nullptr;
	_point = { 0,0 };

	_startTime = 0;
	_startNum = 0;
	_nowNum = 0;

	_handle = -1;

	_isStart = false;
	_isStop = false;
	_isEnd = false;
	_is1P = is1P;
}

Timer::~Timer(){
}

bool Timer::Init() {

	std::array<const TCHAR*, DrawNumber::ImageHandlNum> imageNames = {
	_T("png/number/time0.png"), _T("png/number/time1.png") ,
	_T("png/number/time2.png"), _T("png/number/time3.png") ,
	_T("png/number/time4.png"), _T("png/number/time5.png") ,
	_T("png/number/time6.png"), _T("png/number/time7.png") ,
	_T("png/number/time8.png"), _T("png/number/time9.png")
	};

	_pDrawNumber.reset(new DrawNumber);

	_point = _is1P ? Parameter::_TimerPosition1P : Parameter::_TimerPosition2P;

	if (!_pDrawNumber->Init(imageNames)) {
		return false;
	}

	return true;
}

void Timer::Process() {

	if (!_isStart || _isEnd || _isStop) {
		return;
	}

	int nowTime = GetNowCount() - _startTime;
	int nowSecond = nowTime / 1000;
	int nowNum = _startNum - nowSecond;

	if (_nowNum != nowNum) {
		_nowNum = nowNum;

		if (_nowNum < 0) {
			_nowNum = 0;
		}

		if (_nowNum == 0) {
			_isEnd = true;
		}

		SetNumber(_nowNum);
	}
}

bool Timer::SetStart(int startNum) {

	_isStart = true;
	_isEnd = false;
	_isStop = false;

	_startNum = startNum;
	_nowNum = startNum;

	_startTime = GetNowCount();

	return SetNumber(startNum);
}

bool Timer::SetNumber(int num) {

	if (_pDrawNumber == nullptr) {
		return false;
	}

	_handle = _pDrawNumber->SetNumber(_point.x, _point.y, num, DrawNumber::Anchor::LeftUp, _handle);

	return true;
}

void Timer::Draw() {

	if (_pDrawNumber != nullptr) {
		_pDrawNumber->Draw();
	}
}