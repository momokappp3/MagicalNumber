#include "Parameter.h"

int Parameter::_ScreenWidth;
int Parameter::_ScreenHeight;
int Parameter::_ScreenHalfWidth;
int Parameter::_ScreenHalfHeight;

int Parameter::_FadeInFrame;
int Parameter::_FadeWaitFrame;
int Parameter::_FadeOutFrame;
int Parameter::_FadeFrame;

Point Parameter::_TitleMA;
Point Parameter::_TitleJI;
Point Parameter::_TitleKA;
Point Parameter::_TitleRU;
Point Parameter::_TitleNA;
Point Parameter::_TitleNN;
Point Parameter::_TitleBA;
Point Parameter::_TitleAA;

int Parameter::_TitleStartFrame;
int Parameter::_TitleDelayFrame;
int Parameter::_TitleEffectDelayFrame;
int Parameter::_TitleEndFrame;
int Parameter::_TitleCharcterProcessFrame;

Point Parameter::_MenuMatch;
Point Parameter::_MenuHelp;
Point Parameter::_MenuEndGame;

int Parameter::_MenuHelpOpenFrame;
Tween::Type Parameter::_MenuHelpOpenType;

int Parameter::_MenuHelpCloseFrame;
Tween::Type Parameter::_MenuHelpCloseType;

int Parameter::_deckNum;
int Parameter::_handNum;
int Parameter::_hp;
int Parameter::_turnTime;

int Parameter::_someNumberInDeck;

Point Parameter::_ChoiceNormalWaku1P;
Point Parameter::_ChoiceNormalWaku2P;
Point Parameter::_ChoicePremiumWaku1P;
Point Parameter::_ChoicePremiumWaku2P;

Point Parameter::_ChoiceNormalBackWaku1P;
Point Parameter::_ChoiceNormalBackWaku2P;
Point Parameter::_ChoicePremiumBackWaku1P;
Point Parameter::_ChoicePremiumBackWaku2P;

Point Parameter::_ChoiceNormalFilter1P;
Point Parameter::_ChoiceNormalFilter2P;
Point Parameter::_ChoicePremiumFilter1P;
Point Parameter::_ChoicePremiumFilter2P;

Point Parameter::_SelectedDeckFrame1P;
Point Parameter::_SelectedDeckFrame2P;

Point Parameter::_SelectedDeckStartPosition1P;
Point Parameter::_SelectedDeckStartPosition2P;

int Parameter::_SelectedDeckIntervalWidth;
int Parameter::_SelectedDeckIntervalHeight;

Point Parameter::_ChoiceNormalStartPosition1P;
Point Parameter::_ChoiceNormalEndPosition1P;
Point Parameter::_ChoiceNormalStartPosition2P;
Point Parameter::_ChoiceNormalEndPosition2P;

Point Parameter::_ChoicePremiumStartPosition1P;
Point Parameter::_ChoicePremiumEndPosition1P;
Point Parameter::_ChoicePremiumStartPosition2P;
Point Parameter::_ChoicePremiumEndPosition2P;

Point Parameter::_ChoiceNormalCardStartPotistion1P;
Point Parameter::_ChoiceNormalCardStartPotistion2P;
Point Parameter::_ChoicePremiumCardStartPotistion1P;
Point Parameter::_ChoicePremiumCardStartPotistion2P;

int Parameter::_ChoiceCardIntervalWidth;

int Parameter::_HpImageMarginLeft;
int Parameter::_HpImageMarginRight;
Point Parameter::_HpPosition1P;
Point Parameter::_HpPosition2P;

Point Parameter::_MagicNumberPosition1P;
Point Parameter::_MagicNumberPosition2P;

Point Parameter::_TurnNumberPosition;

Point Parameter::_TimerPosition1P;
Point Parameter::_TimerPosition2P;

Point Parameter::_AnswerPosition1P;
Point Parameter::_AnswerPosition2P;

std::vector<Point> Parameter::_vSignPosition1P;
std::vector<Point> Parameter::_vSignPosition2P;

std::vector<Point> Parameter::_vHandPosition1P;
std::vector<Point> Parameter::_vHandPosition2P;

int Parameter::_formulaMaxCard;

Point Parameter::_formulaPosition1P;
Point Parameter::_formulaPosition2P;

int Parameter::_formulaIntervalWidth;
Point Parameter::_formulaSignOffSet;

Point Parameter::_attackNumberPotion1P;
Point Parameter::_attackNumberPotion2P;

int Parameter::_attackNumberBace;
int Parameter::_attackNumberPerfectBonus;
int Parameter::_attackNumberDiffMax;
int Parameter::_attackNumberUnit;
int Parameter::_attackWaitStartFrame;
int Parameter::_attackWaitEndFrame;

Point Parameter::_cutInHorizontalPosition;
Point Parameter::_cutInVerticalPosition1P;
Point Parameter::_cutInVerticalPosition2P;

Parameter::Parameter() {
}

Parameter::~Parameter() {
}

bool Parameter::Init() {

	_ScreenWidth =1920;
	_ScreenHeight = 1080;
	_ScreenHalfWidth = _ScreenWidth / 2;
	_ScreenHalfHeight = _ScreenHeight / 2;

	_FadeInFrame = 40;
	_FadeWaitFrame = 20;
	_FadeOutFrame = 40;
	_FadeFrame = _FadeInFrame + _FadeWaitFrame + _FadeOutFrame;

	_TitleMA = {100,800};
	_TitleJI = {350,800};
	_TitleKA = {600,800};
	_TitleRU = {850,800};
	_TitleNA = {1000,800};
	_TitleNN = {1250,800};
	_TitleBA = {1500,800};
	_TitleAA = {1750,800};

	_TitleStartFrame = 60;
	_TitleDelayFrame = 7;
	_TitleEffectDelayFrame = 120;
	_TitleEndFrame = 30;
	_TitleCharcterProcessFrame = 110;

	_MenuMatch = {960,340};
	_MenuHelp = {960,540};
	_MenuEndGame = {960,740};

	_MenuHelpOpenFrame = 15;
	_MenuHelpOpenType = Tween::Type::SineEnd;
	_MenuHelpCloseFrame = 10;
	_MenuHelpCloseType = Tween::Type::SineStart;

	//本来スクリプトから読み込むようにするがα版は決め打ち
	_deckNum = 20;
	_handNum = 10;
	_hp = 1000;
	_turnTime = 60;

	_someNumberInDeck = 3;

	_ChoiceNormalWaku1P = {20,10};   //枠  濃い枠
	_ChoiceNormalWaku2P = {1029,10};
	//下
	_ChoicePremiumWaku1P = {20,400};    //枠　薄い枠
	_ChoicePremiumWaku2P = {1029,400};

	_ChoiceNormalBackWaku1P = { 33,20 };
	_ChoiceNormalBackWaku2P = { 1042,20 };
	_ChoicePremiumBackWaku1P = { 33,410 };
	_ChoicePremiumBackWaku2P = { 1042,410 };

	_ChoiceNormalFilter1P = { 50,53};   //フィルター
	_ChoiceNormalFilter2P = { 1059,53};
	//下
	_ChoicePremiumFilter1P = { 50,437};
	_ChoicePremiumFilter2P = { 1059,437};

	_SelectedDeckFrame1P = {25,773};   //選択　一番下枠
	_SelectedDeckFrame2P = {1037,773};

	_SelectedDeckStartPosition1P = {40,810};   //下のカード
	_SelectedDeckStartPosition2P = {1050,813};

	_SelectedDeckIntervalWidth = 85;
	_SelectedDeckIntervalHeight = 130;

	_ChoiceNormalStartPosition1P = {113,10};   //見えているスタート場所
	_ChoiceNormalEndPosition1P   = {800,430};   //見えているエンド場所
	_ChoiceNormalStartPosition2P = {1100,10};
	_ChoiceNormalEndPosition2P   = {1800,430};

	_ChoicePremiumStartPosition1P = { 100,350};
	_ChoicePremiumEndPosition1P   = { 800,750};
	_ChoicePremiumStartPosition2P = { 1100,350};
	_ChoicePremiumEndPosition2P   = {1800,750};

	_ChoiceNormalCardStartPotistion1P = { -800,60};   //背面ノーマルカード
	_ChoiceNormalCardStartPotistion2P = { 200, 60};
	_ChoicePremiumCardStartPotistion1P = { -800,440};
	_ChoicePremiumCardStartPotistion2P = { 200,440};

	_ChoiceCardIntervalWidth = 230;   //選択画面インターバル

	_MagicNumberPosition1P = {400,125};
	_MagicNumberPosition2P = {1345,125};

	_TurnNumberPosition = {900,100};

	_TimerPosition1P = {700,130};
	_TimerPosition2P = {1100,130};

	_AnswerPosition1P = {400,460};
	_AnswerPosition2P = {1345,460};

	_HpImageMarginLeft = 17;
	_HpImageMarginRight = 20;
	_HpPosition1P = { 30,30 };
	_HpPosition2P = { 1429,30 };

	_vSignPosition1P = {
		{100,650},{250,650},{400,650},{550,650},{700,650},{850,650}
	};

	_vSignPosition2P = {
		{1070,650},{1220,650},{1370,650},{1520,650},{1670,650},{1820,650}
	};


	//間隔  150
	_vHandPosition1P = {
		{150,750},{300,750},{450,750},{600,750},{750,750},
		{150,900},{300,900},{450,900},{600,900},{750,900}
	};
	_vHandPosition2P = {
		{1110,750},{1260,750},{1410,750},{1560,750},{1710,750},
		{1110,900},{1260,900},{1410,900},{1560,900},{1710,900}
	};
	
	//_formulaPosition1P = { 50,500 };   //式の場所
	//_formulaPosition2P = { 1000,500 };

	_formulaMaxCard = 6;

	_formulaPosition1P = { 50,320 };
	_formulaPosition2P = { 1000,280 };

	_formulaIntervalWidth = 75;
	_formulaSignOffSet = { 15,40 };

	_attackNumberPotion1P = { 500,900 };
	_attackNumberPotion2P = { 1445,900 };

	_attackNumberBace = 100;
	_attackNumberPerfectBonus = 100;
	_attackNumberDiffMax = 10;
	_attackNumberUnit = 5;

	_attackWaitStartFrame = 60;
	_attackWaitEndFrame = 120;

	_cutInHorizontalPosition = { 0,350 };
	_cutInVerticalPosition1P = { 280,0 };
	_cutInVerticalPosition2P = { 1220,0 };

	int handNum1P = _vHandPosition1P.size();
	int handNum2P = _vHandPosition2P.size();

	if (handNum1P != handNum2P) {
		return false;
	}

	if (handNum1P != _handNum) {
		return false;
	}

	return true;
}
