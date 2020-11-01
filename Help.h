#pragma once
#include "Types.h"
#include "Tween.h"
#include <functional>
#include "Input.h"
#include <memory>
#include "SoundManager.h"

class Help {
public:
	Help();
	virtual ~Help();

	enum class RectType{
		Zero = 0,
		First,
		Second,
		Third,
		Final,

		Max
	};

	bool Init(std::function<bool(Input::INPUT, bool)> isInput, std::shared_ptr<SoundManager>& soundManager);
	void Process();
	void Draw();
	void Open();
	void Close();
	void Reset();
	void DeleteImage();
	bool IsClosed() {
		return _end && (_rectType == RectType::Zero);
	}

private:

	std::function<bool(Input::INPUT, bool)> _pIsInput;
	std::shared_ptr<SoundManager> _pSoundManager;

	Tween _tween;

	Point _areaLeftUp;
	Point _areaRightDown;
	Point _firstRect;
	Point _secondRect;
	Point _thirdRect;

	RectType _rectType;

	int _handle;
	int _imageWidth;
	int _imageHeight;

	bool _isStart;
	bool _isEnd;
	bool _end;
};