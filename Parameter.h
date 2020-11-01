#pragma once
#include <vector>
#include "Types.h"
#include "Tween.h"

 class Parameter {
private:
	Parameter();
public:
	virtual ~Parameter();

	static bool Init();

	static int _ScreenWidth;
	static int _ScreenHeight;
	static int _ScreenHalfWidth;
	static int _ScreenHalfHeight;

	static int _FadeInFrame;
	static int _FadeWaitFrame;
	static int _FadeOutFrame;
	static int _FadeFrame;

	static Point _TitleMA;
	static Point _TitleJI;
	static Point _TitleKA;
	static Point _TitleRU;
	static Point _TitleNA;
	static Point _TitleNN;
	static Point _TitleBA;
	static Point _TitleAA;

	static int _TitleStartFrame;
	static int _TitleDelayFrame;
	static int _TitleEffectDelayFrame;
	static int _TitleEndFrame;
	static int _TitleCharcterProcessFrame;

	static Point _MenuMatch;
	static Point _MenuHelp;
	static Point _MenuEndGame;

	static int _MenuHelpOpenFrame;
	static Tween::Type _MenuHelpOpenType;

	static int _MenuHelpCloseFrame;
	static Tween::Type _MenuHelpCloseType;

	static int _deckNum;   //デッキの枚数
	static int _handNum;

	static int _hp;
	static int _turnTime;

	static int _someNumberInDeck;   //デッキに入れられる同じカードの枚数

	static Point _ChoiceNormalWaku1P;   //デッキ構築画面大きい枠
	static Point _ChoiceNormalWaku2P;
	static Point _ChoicePremiumWaku1P;
	static Point _ChoicePremiumWaku2P;

	static Point _ChoiceNormalBackWaku1P;
	static Point _ChoiceNormalBackWaku2P;
	static Point _ChoicePremiumBackWaku1P;
	static Point _ChoicePremiumBackWaku2P;

	static Point _ChoiceNormalFilter1P;
	static Point _ChoiceNormalFilter2P;
	static Point _ChoicePremiumFilter1P;
	static Point _ChoicePremiumFilter2P;

	static Point _SelectedDeckFrame1P;
	static Point _SelectedDeckFrame2P;

	static Point _SelectedDeckStartPosition1P;
	static Point _SelectedDeckStartPosition2P;

	static int _SelectedDeckIntervalWidth;
	static int _SelectedDeckIntervalHeight;

	static Point _ChoiceNormalStartPosition1P;
	static Point _ChoiceNormalEndPosition1P;
	static Point _ChoiceNormalStartPosition2P;
	static Point _ChoiceNormalEndPosition2P;

	static Point _ChoicePremiumStartPosition1P;
	static Point _ChoicePremiumEndPosition1P;
	static Point _ChoicePremiumStartPosition2P;
	static Point _ChoicePremiumEndPosition2P;

	static Point _ChoiceNormalCardStartPotistion1P;
	static Point _ChoiceNormalCardStartPotistion2P;

	static Point _ChoicePremiumCardStartPotistion1P;
	static Point _ChoicePremiumCardStartPotistion2P;

	static int _ChoiceCardIntervalWidth;

	static int _HpImageMarginLeft;
	static int _HpImageMarginRight;
	static Point _HpPosition1P;
	static Point _HpPosition2P;

	static Point _MagicNumberPosition1P;
	static Point _MagicNumberPosition2P;

	static Point _TurnNumberPosition;

	static Point _TimerPosition1P;
	static Point _TimerPosition2P;

	static Point _AnswerPosition1P;
	static Point _AnswerPosition2P;

	static std::vector<Point> _vSignPosition1P;
	static std::vector<Point> _vSignPosition2P;

	static std::vector<Point> _vHandPosition1P;
	static std::vector<Point> _vHandPosition2P;

	static int _formulaMaxCard;

	static Point _formulaPosition1P;
	static Point _formulaPosition2P;

	static int _formulaIntervalWidth;
	static Point _formulaSignOffSet;

	static Point _attackNumberPotion1P;
	static Point _attackNumberPotion2P;

	static int _attackNumberBace;
	static int _attackNumberPerfectBonus;
	static int _attackNumberDiffMax;
	static int _attackNumberUnit;

	static int _attackWaitStartFrame;
	static int _attackWaitEndFrame;

	static Point _cutInHorizontalPosition;
	static Point _cutInVerticalPosition1P;
	static Point _cutInVerticalPosition2P;

};