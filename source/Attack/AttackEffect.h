#pragma once
#include <memory>
#include "../Use/Effect.h"

class AttackEffect {
	AttackEffect();
	virtual ~AttackEffect();

	enum class EffectAttack {
		Sord1P = 0,
		Sord2P,

		Max
	};

	bool Init(EffectAttack handle, int delay);
	void Process();

private:
	void Delete();
	void DeleteEffect();

	std::unique_ptr<Effect> _pEffect;

	bool _isEffect;
};