#include "DxLib.h"
#include "AttackEffect.h"

AttackEffect::AttackEffect(){

	_pEffect = nullptr;

	_isEffect = false;
}

AttackEffect::~AttackEffect() {
}

bool AttackEffect::Init(EffectAttack type, int delay) {

	_pEffect.reset(new Effect);

	const TCHAR* fileName = nullptr;
	bool effectInit = false;

	switch (type){
	case AttackEffect::EffectAttack::Sord1P:
		effectInit = _pEffect->Init(_T("SwordFire.efk"), 10.0f, true);
		break;
	case AttackEffect::EffectAttack::Sord2P:
		effectInit = _pEffect->Init(_T("Swordmizu.efk"), 10.0f, true);
		break;
	default:
		break;
	}

	if (!effectInit) {
		return false;
	}

	_pEffect->SetPoint({ 0,0 });

	return true;
}

void AttackEffect::Process() {

	if (_pEffect != nullptr) {
		_pEffect->Process();
	}
}

void AttackEffect::Delete() {  //音とエフェクトを止める

	if (_pEffect != nullptr) {
		_pEffect->Stop();
	}
}

void AttackEffect::DeleteEffect() {

	if (_pEffect != nullptr) {
		_pEffect->Delete();
	}
}