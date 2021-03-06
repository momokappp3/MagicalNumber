#pragma once

enum class GameState {
	Title = 0,
	TitleMenu,
	DeckSelect,
	DeckSelectFinish,
	DeckSelectFade,
	PreStart,
	ReStart,
	Start,
	SetMagicNumber,
	SetHandCard,
	Playing,
	Waiting,
	PreAttackNumberFirst,
	StartCutInFirst,
	PreAttackNumberSecond,
	StartCutInSecond,
	Judgment,
	PreResult,
	Result,
	GameEnd,

	Max
};

struct TransState{   //GameStateを2つもてるように
	GameState now;
	GameState last;
};

struct Point{
	int x;
	int y;
};

struct DrawInfo {
	int handle;
	Point xy;
	bool isDraw;
};