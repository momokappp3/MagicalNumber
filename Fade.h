#pragma once
#include "Tween.h"

class Fade {
public:
	Fade();
	virtual ~Fade();

	static bool Init();
	static void Process();
	static void Draw();
	static void Start();
	static void Reset();
	static bool IsEnd();
	static void SetColor(int Red, int Green, int Blue);

private:
	static Tween _tween;

	static unsigned int _color;

	//static int _handle;
	static int _alpha;
	static int _wait;

	static bool _isFade;
	static bool _isFadeOut;
	static bool _end;
};