#pragma once
#include <array>
#include <memory>
#include "Types.h"
#include "Tween.h"
#include "SoundManager.h"


class CutIn {
public:
	CutIn(bool is1P);
	virtual ~CutIn();

	enum class Type {
		Vertical,
		Horizontal,

		Max
	};

	static constexpr int ImageNum = 2;

	bool Init(Type type, std::shared_ptr<SoundManager>& soundManager);
	bool Start(bool leftUp);
	bool Finish();
	void TurnInit();
	void Process();
	void Draw();

	bool IsWait() {
		return _wait;
	}
	
	bool IsFinish() {
		return _finish;
	}

	bool IsComplete() {
		return _finishCount >= 255;
	}

private:
	bool StartBG(bool leftUp);
	bool StartCharcter(bool leftUp);
	void DrawBG();
	void DrawCharcter();
	bool GetBGPosition(Point& leftUp, Point& rigthDown, int& alpha);
	bool GetCharcterPosition(Point& leftUp, int& alpha);

	Tween _tweenBG;
	Tween _tweenCarcter;
	std::shared_ptr<SoundManager> _pSoundManager;

	std::array<int, ImageNum> _handle;

	Point _point;
	Type _type;

	int _width;
	int _height;
	int _finishCount;

	bool _is1P;
	bool _endBG;
	bool _drawTypeLeftUp;
	bool _wait;
	bool _finish;
};