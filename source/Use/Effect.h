#pragma once
#include <tchar.h>
#include "../Use/Types.h"

class Effect {
public:
	Effect();
	virtual ~Effect();

	bool Init(const TCHAR* fileName,float magnification, bool isLoop);
	virtual void Start();
	virtual void Stop();
	virtual void Process();
	virtual void ProcessPoint();
	virtual void Delete();

	Point GetPoint() {
		return _point;
	}

	void SetPoint(Point point) {
		_point = point;
	}

protected:

	Point _point;

	int _effectResourceHandle;
	int _playingEffectHandle;   //“®‚©‚·Žž‚ÉŽg‚¤

	bool _isLoop;
	bool _isEnd;
};