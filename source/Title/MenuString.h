#pragma once
#include "../Use/Types.h"
#include "../Use/Tween.h"

class MenuString {
public:
	MenuString();
	virtual ~MenuString();

	enum class Kind {
		Match = 0,
		Help,
		EndGame,

		Max
	};

	virtual bool Init(Kind kind, int delay);
	virtual void Process();
	virtual void Draw();
	void DeleteImage();

	int GetDelay() {
		return _delay;
	}

protected:
	void DrawScale(float scale);
	void SetTweenScale(int start, int end, int frame, Tween::Type type);

	Tween _tween;

	Point _center;

	int _handle;
	int _delay;

	float _imageWidth;
	float _imageHeight;
	float _scale;

};