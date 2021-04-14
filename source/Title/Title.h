#pragma once
#include "../ObjectBace.h"
#include "../Use/Types.h"
#include <memory>
#include "TitleCharacter.h"
#include "../Use/Effect.h"
#include "../Use/SoundManager.h"
#include <functional>
#include "../Input.h"

class Title : public ObjectBase{
public :
	Title();
	virtual ~Title();

	bool Init(std::function<bool(Input::INPUT, bool)> isInput, std::shared_ptr<SoundManager>& soundManager);
	GameState Process(GameState state);
	void Draw() override;
	void DeleteImage();

private:

	void DrawTitleCharacter(TitleCharacter* titleCharacter);
	void SetTitleCharcterFloatingMode();
	void SetTitelCharcterEndingMode();
	void Delete();

	std::function<bool(Input::INPUT, bool)> _pIsInput;
	std::shared_ptr<SoundManager> _pSoundManager;

	std::unique_ptr<TitleCharacter> _pMA;
	std::unique_ptr<TitleCharacter> _pJI;
	std::unique_ptr<TitleCharacter> _pKA;
	std::unique_ptr<TitleCharacter> _pRU;
	std::unique_ptr<TitleCharacter> _pNA;
	std::unique_ptr<TitleCharacter> _pNN;
	std::unique_ptr<TitleCharacter> _pBA;
	std::unique_ptr<TitleCharacter> _pAA;

	std::unique_ptr<Effect> _pEffect;

	int _delayEffectCount;
	int _endFrameCount;

	bool _isEffect;
	bool _isBgm;
	bool _isBButton;
	bool _isEnd;
};