#include "TitleMenu.h"
#include "ResourceServer.h"
#include "Fade.h"
#include "Parameter.h"

TitleMenu::TitleMenu() : ObjectBase(TYPE::TitleMenu) {

	_pIsInput = nullptr;
	_pSoundManager = nullptr;

	_pMatch = nullptr;
	_pHelp = nullptr;
	_pEndGame = nullptr;
	_pMenuSelect = nullptr;
	_pHelpWindow = nullptr;

	_nowSelect = MenuString::Kind::Match;

	_fadeCount = 0;

	_seMatch = false;
	_seHelp = false;
	_seEndGame = false;
	_seMenuSelect = false;
	_isSelect = false;
}

TitleMenu::~TitleMenu() {
}

bool TitleMenu::Init(std::function<bool(Input::INPUT, bool)> isInput, std::shared_ptr<SoundManager>& soundManager) {

	if (isInput == nullptr) {
		return false;
	}

	_pIsInput = isInput;

	if (soundManager == nullptr) {
		return false;
	}

	_pSoundManager = soundManager;

	int handle = ResourceServer::LoadGraph(_T("png/back/titleback.png"));   //–³’n

	if (handle == -1) {
		return false;
	}

	DrawInfo drawInfo = { handle,{0,0},false };

	_vDrawInfo.push_back(drawInfo);

	_pMatch.reset(new MenuString);
	_pHelp.reset(new MenuString);
	_pEndGame.reset(new MenuString);
	_pMenuSelect.reset(new MenuSelect);
	_pHelpWindow.reset(new Help);

	int start = 10;
	int delay = 30;

	bool matchInit = _pMatch->Init(MenuString::Kind::Match, start);
	start += delay;
	bool helpInit = _pHelp->Init(MenuString::Kind::Help, start);
	start += delay;
	bool endGame = _pEndGame->Init(MenuString::Kind::EndGame, start);
	start += delay;
	bool menuSelectInit = _pMenuSelect->Init(MenuString::Kind::Match, start);
	bool helpWindowInit = _pHelpWindow->Init(isInput, soundManager);

	if (!matchInit || !helpInit || !endGame || !menuSelectInit ||! helpWindowInit) {
		return false;
	}

	return true;
}

GameState TitleMenu::Process(GameState state) {

	if (_pMatch != nullptr) {
		_pMatch->Process();

		if (!_seMatch) {
			if (_pMatch->GetDelay() == 0) {
				_seMatch = true;
				
				if (_pSoundManager != nullptr) {
					_pSoundManager->PlaySETitleMenu(SoundManager::SETitleMenu::Effect);
				}
			}
		}
	}

	if (_pHelp != nullptr) {
		_pHelp->Process();

		if (!_seHelp) {
			if (_pHelp->GetDelay() == 0) {
				_seHelp = true;

				if (_pSoundManager != nullptr) {
					_pSoundManager->PlaySETitleMenu(SoundManager::SETitleMenu::Effect);
				}
			}
		}
	}

	if (_pEndGame != nullptr) {
		_pEndGame->Process();

		if (!_seEndGame) {
			if (_pEndGame->GetDelay() == 0) {
				_seEndGame = true;

				if (_pSoundManager != nullptr) {
					_pSoundManager->PlaySETitleMenu(SoundManager::SETitleMenu::Effect);
				}
			}
		}
	}

	if (_pMenuSelect != nullptr) {
		_pMenuSelect->Process();

		if (_pMenuSelect->GetDelay() == 0) {
			if (!_seMenuSelect) {
				_seMenuSelect = true;

				if (_pSoundManager != nullptr) {
					_pSoundManager->PlaySECommon(SoundManager::SECommon::Move);
				}
			}

			ProcessInput();
		}
	}

	if (_pHelpWindow != nullptr) {
		_pHelpWindow->Process();

		if (_pHelpWindow->IsClosed()) {
			_pHelpWindow->Reset();

			_isSelect = false;
		}
	}

	return GetNowState();
}

void TitleMenu::Draw() {

	ObjectBase::Draw();

	if (_pMatch != nullptr) {
		_pMatch->Draw();
	}

	if (_pHelp != nullptr) {
		_pHelp->Draw();
	}

	if (_pEndGame != nullptr) {
		_pEndGame->Draw();
	}

	if (_pMenuSelect != nullptr) {
		_pMenuSelect->Draw();
	}

	if (_pHelpWindow != nullptr) {
		_pHelpWindow->Draw();
	}
}

void TitleMenu::ProcessInput() {

	if (_isSelect) {
		return;
	}

	if (_pIsInput(Input::INPUT::Up, true)) {
		if (_nowSelect > MenuString::Kind::Match) {
			int now = static_cast<int>(_nowSelect);

			now--;

			_nowSelect = static_cast<MenuString::Kind>(now);
			_pMenuSelect->SetSelect(_nowSelect);

			if (_pSoundManager != nullptr) {
				_pSoundManager->PlaySECommon(SoundManager::SECommon::Move);
			}
		}
	}

	if (_pIsInput(Input::INPUT::Down, true)) {
		if (_nowSelect < MenuString::Kind::EndGame) {
			int now = static_cast<int>(_nowSelect);

			now++;

			_nowSelect = static_cast<MenuString::Kind>(now);
			_pMenuSelect->SetSelect(_nowSelect);

			if (_pSoundManager != nullptr) {
				_pSoundManager->PlaySECommon(SoundManager::SECommon::Move);
			}
		}
	}

	if (_pIsInput(Input::INPUT::B, true)) {
		_isSelect = true;

		switch (_nowSelect){
		case MenuString::Kind::Match:
			Fade::Reset();
			Fade::Start();

			if (_pSoundManager != nullptr) {
				_pSoundManager->PlaySECommon(SoundManager::SECommon::Fade);
			}
			break;

		case MenuString::Kind::Help:
			_pHelpWindow->Open();
			break;

		case MenuString::Kind::EndGame:
			break;
		default:
			break;
		}

		if (_pSoundManager != nullptr) {
			_pSoundManager->PlaySECommon(SoundManager::SECommon::BButton);
		}
	}
}

GameState TitleMenu::GetNowState() {

	if (!_isSelect) {
		return GameState::TitleMenu;
	}

	if (_nowSelect == MenuString::Kind::Match) {
		_fadeCount++;

		if (_fadeCount <= Parameter::_FadeInFrame) {
			return GameState::TitleMenu;
		}
		else {
			return GameState::DeckSelect;
		}
	}
	else if(_nowSelect == MenuString::Kind::Help){
		return GameState::TitleMenu;
	}

	return GameState::GameEnd;
}

void TitleMenu::DeleteImage() {

	if (_vDrawInfo.size() > 0) {

		DrawInfo drawInfo = _vDrawInfo.front();

		ResourceServer::DeleteGraph(drawInfo.handle);

		_vDrawInfo.clear();
	}

	if (_pMatch != nullptr) {
		_pMatch->DeleteImage();
	}

	if (_pHelp != nullptr) {
		_pHelp->DeleteImage();
	}

	if (_pEndGame != nullptr) {
		_pEndGame->DeleteImage();
	}

	if (_pMenuSelect != nullptr) {
		_pMenuSelect->DeleteImage();
	}

	if (_pHelpWindow != nullptr) {
		_pHelpWindow->DeleteImage();
	}
}