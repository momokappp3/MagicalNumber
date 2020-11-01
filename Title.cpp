#include "Title.h"
#include "ResourceServer.h"
#include "DxLib.h"
#include "Parameter.h"

//基底クラスのコンストラクタを呼ぶ
Title::Title() : ObjectBase(TYPE::Title) {

	_pIsInput = nullptr;
	_pSoundManager = nullptr;

	_pMA = nullptr;
	_pJI = nullptr;
	_pKA = nullptr;
	_pRU = nullptr;
	_pNA = nullptr;
	_pNN = nullptr;
	_pBA = nullptr;
	_pAA = nullptr;

	_pEffect = nullptr;

	_delayEffectCount = 0;
	_endFrameCount = 0;

	_isEffect = false;
	_isBgm = false;
	_isBButton = false;
	_isEnd = false;
}

Title::~Title() {
}

bool Title::Init(std::function<bool(Input::INPUT, bool)> isInput, std::shared_ptr<SoundManager>& soundManager) {

	if (isInput == nullptr) {
		return false;
	}

	_pIsInput = isInput;

	if (soundManager != nullptr) {
		bool seTitle = soundManager->LoadSETitle();

		if (!seTitle) {
			return false;
		}
	}
	else {
		return false;
	}

	_pSoundManager = soundManager;

	_pMA.reset(new TitleCharacter);
	_pJI.reset(new TitleCharacter);
	_pKA.reset(new TitleCharacter);
	_pRU.reset(new TitleCharacter);
	_pNA.reset(new TitleCharacter);
	_pNN.reset(new TitleCharacter);
	_pBA.reset(new TitleCharacter);
	_pAA.reset(new TitleCharacter);

	_pEffect.reset(new Effect);

	int start = Parameter::_TitleStartFrame;
	int delay = Parameter::_TitleDelayFrame;

	bool maInit = _pMA->Init(TitleCharacter::CharKind::MA, start);
	start += delay;
	bool jiInit = _pJI->Init(TitleCharacter::CharKind::JI, start);
	start += delay;
	bool kaInit = _pKA->Init(TitleCharacter::CharKind::KA, start);
	start += delay;
	bool ruInit = _pRU->Init(TitleCharacter::CharKind::RU, start);
	start += delay;
	bool naInit = _pNA->Init(TitleCharacter::CharKind::NA, start);
	start += delay;
	bool nnInit = _pNN->Init(TitleCharacter::CharKind::NN, start);
	start += delay;
	bool baInit = _pBA->Init(TitleCharacter::CharKind::BA, start);
	start += delay;
	bool aaInit = _pAA->Init(TitleCharacter::CharKind::AA, start);

	bool effectInit = _pEffect->Init(_T("GateOfCalve.efk"), 10.0f, true);

	int handle = ResourceServer::LoadGraph(_T("png/back/titleback.png"));   //無地

	if (handle == -1 || !maInit || !jiInit || !kaInit || !ruInit || !naInit || !nnInit || !baInit || !aaInit || !effectInit) {
		return false;
	}

	_pEffect->SetPoint({500,600});
	DrawInfo drawInfo = { handle,{0,0},false };

	_vDrawInfo.push_back(drawInfo);

	return true;
}

GameState Title::Process(GameState state) {

	if (!_isBgm) {
		if (_pSoundManager != nullptr) {
			_pSoundManager->PlayBgm(SoundManager::BGM::Title);
			_isBgm = true;
		}
	}

	if (_isEnd) {
		_endFrameCount++;
	}

	if (!_isEnd && _isBButton && _pIsInput != nullptr) {
		if (_pIsInput(Input::INPUT::B, true)) {
			_isEnd = true;

			if (_pSoundManager != nullptr) {
				_pSoundManager->PlaySECommon(SoundManager::SECommon::BButton);
			}

			SetTitelCharcterEndingMode();
			Delete();
		}
	}

	if (_pMA != nullptr) {
		_pMA->Process();
	}

	if (_pJI != nullptr) {
		_pJI->Process();
	}

	if (_pKA != nullptr) {
		_pKA->Process();
	}

	if (_pRU != nullptr) {
		_pRU->Process();
	}

	if (_pNA != nullptr) {
		_pNA->Process();
	}

	if (_pNN != nullptr) {
		_pNN->Process();
	}

	if (_pBA != nullptr) {
		_pBA->Process();
	}

	if (_pAA != nullptr) {
		_pAA->Process();

		if (!_isEffect && _pAA->GetAlpha() == 255) {
			_delayEffectCount++;

			if (_delayEffectCount >= Parameter::_TitleEffectDelayFrame) {
				_isEffect = true;
				_isBButton = true;

				_pEffect->Start();
				SetTitleCharcterFloatingMode();

				if (_pSoundManager != nullptr) {
					_pSoundManager->PlaySETitle(SoundManager::SETitle::Effect, 45);
				}
			}
		}
	}

	if (_pEffect != nullptr) {
		_pEffect->Process();
	}

	return (_endFrameCount > Parameter::_TitleEndFrame) ? GameState::TitleMenu : GameState::Title;
}

void Title::Draw() {

	ObjectBase::Draw();

	DrawTitleCharacter(_pMA.get());
	DrawTitleCharacter(_pJI.get());
	DrawTitleCharacter(_pKA.get());
	DrawTitleCharacter(_pRU.get());
	DrawTitleCharacter(_pNA.get());
	DrawTitleCharacter(_pNN.get());
	DrawTitleCharacter(_pBA.get());
	DrawTitleCharacter(_pAA.get());

}

void Title::DrawTitleCharacter(TitleCharacter* titleCharacter) {

	if (titleCharacter == nullptr) {
		return;
	}

	int alpha = titleCharacter->GetAlpha();

	if (alpha < 255) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	}

	titleCharacter->Draw();

	if (alpha < 255) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void Title::SetTitleCharcterFloatingMode() {

	if (_pMA != nullptr) {
		_pMA->SetFloatMode();
	}

	if (_pJI != nullptr) {
		_pJI->SetFloatMode();
	}

	if (_pKA != nullptr) {
		_pKA->SetFloatMode();
	}

	if (_pRU != nullptr) {
		_pRU->SetFloatMode();
	}

	if (_pNA != nullptr) {
		_pNA->SetFloatMode();
	}

	if (_pNN != nullptr) {
		_pNN->SetFloatMode();
	}

	if (_pBA != nullptr) {
		_pBA->SetFloatMode();
	}

	if (_pAA != nullptr) {
		_pAA->SetFloatMode();
	}
}

void Title::SetTitelCharcterEndingMode() {

	if (_pMA != nullptr) {
		_pMA->SetEndMode();
	}

	if (_pJI != nullptr) {
		_pJI->SetEndMode();
	}

	if (_pKA != nullptr) {
		_pKA->SetEndMode();
	}

	if (_pRU != nullptr) {
		_pRU->SetEndMode();
	}

	if (_pNA != nullptr) {
		_pNA->SetEndMode();
	}

	if (_pNN != nullptr) {
		_pNN->SetEndMode();
	}

	if (_pBA != nullptr) {
		_pBA->SetEndMode();
	}

	if (_pAA != nullptr) {
		_pAA->SetEndMode();
	}
}

void Title::Delete() {

	if (_pSoundManager != nullptr) {
		_pSoundManager->DeleteSETitle();
	}

	if (_pEffect != nullptr) {
		_pEffect->Stop();
	}
}

void Title::DeleteImage() {

	if (_pMA != nullptr) {
		_pMA->DeleteImage();
	}

	if (_pJI != nullptr) {
		_pJI->DeleteImage();
	}

	if (_pKA != nullptr) {
		_pKA->DeleteImage();
	}

	if (_pRU != nullptr) {
		_pRU->DeleteImage();
	}

	if (_pNA != nullptr) {
		_pNA->DeleteImage();
	}

	if (_pNN != nullptr) {
		_pNN->DeleteImage();
	}

	if (_pBA != nullptr) {
		_pBA->DeleteImage();
	}

	if (_pAA != nullptr) {
		_pAA->DeleteImage();
	}

	if (_pEffect != nullptr) {
		_pEffect->Delete();
	}
}