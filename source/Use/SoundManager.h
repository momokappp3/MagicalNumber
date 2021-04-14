#pragma once
#include <vector>
#include <Tchar.h>

class SoundManager {
public:
	SoundManager();
	virtual ~SoundManager();

	enum class BGM{
		Title = 0,
		InGame,

		Max
	};

	enum class SECommon{
		Move = 0,
		BButton,
		AButton,
		Fade,
		OK,
		Cancel,

		Max
	};

	enum class SETitle {
		Effect = 0,

		Max
	};

	enum class SETitleMenu {
		Effect = 0,
		HelpOpen,

		Max
	};

	enum class SEDeckSelect {
		ChoiceMove = 0,

		Max
	};

	enum class InGame {
		CardSelect = 0,
		CutInVerticalStart,
		CutInVerticalEnd,
		CutInHorizontalStart,
		CutInHorizontalEnd,

		Max
	};

	bool Init();

	bool LoadSECommon();
	void DeleteSECommon();
	bool PlaySECommon(SECommon type, int delayFrame = 0);

	bool LoadSETitle();
	void DeleteSETitle();
	bool PlaySETitle(SETitle type, int delayFrame = 0);

	bool LoadSETitleMenu();
	void DeleteSETitleMenu();
	bool PlaySETitleMenu(SETitleMenu type, int delayFrame = 0);

	bool LoadSEDeckSelect();
	void DeleteSEDeckSelect();
	bool PlaySEDeckSelect(SEDeckSelect type, int delayFrame = 0);

	bool LoadSEInGame();
	void DeleteSEInGame();
	bool PlaySEInGame(InGame type, int delayFrame = 0);

	void Process();
	bool PlayBgm(BGM bgm);
	void DirectStopBgm();
	void FadeStopBgm(int fadeFrame);

private:

	struct DelayPlay{
		int delayFrame;
		int handle;
	};

	bool LoadSE(std::vector<const TCHAR*>& fileNameList, std::vector<int>& handleList);
	void DeleteSE(std::vector<int>& handleList);
	bool PlaySE(int handle, int delayFrame);

	std::vector<const TCHAR*> _vBgmFileName;
	std::vector<struct DelayPlay> _vDelayPlay;
	std::vector<int> _vSECommonHandle;
	std::vector<int> _vSETitleHandle;
	std::vector<int> _vSETitleMenuHandle;
	std::vector<int> _vSEDeckSelectHandle;
	std::vector<int> _vSEInGameHandle;

	BGM _nowPlayBgm;

	int _bgmFadeCount;
	float _bgmFadeFrame;

};