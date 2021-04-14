#pragma once
#include "../ObjectBace.h"
#include <functional>
#include "../Input.h"
#include <memory>
#include "../Use/SoundManager.h"
#include "MenuString.h"
#include "MenuSelect.h"
#include "Help.h"

class TitleMenu : public ObjectBase{
public:
	TitleMenu();
	virtual ~TitleMenu();

	bool Init(std::function<bool(Input::INPUT, bool)> isInput, std::shared_ptr<SoundManager>& soundManager);
	GameState Process(GameState state);
	void Draw() override;
	void DeleteImage();

private:

	void ProcessInput();
	GameState GetNowState();

	std::function<bool(Input::INPUT, bool)> _pIsInput;
	std::shared_ptr<SoundManager> _pSoundManager;
	std::unique_ptr<MenuString> _pMatch;
	std::unique_ptr<MenuString> _pHelp;
	std::unique_ptr<MenuString> _pEndGame;
	std::unique_ptr<MenuSelect> _pMenuSelect;
	std::unique_ptr<Help> _pHelpWindow;

	MenuString::Kind _nowSelect;

	int _fadeCount;

	bool _seMatch;
	bool _seHelp;
	bool _seEndGame;
	bool _seMenuSelect;
	bool _isSelect;

};