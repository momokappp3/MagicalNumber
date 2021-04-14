#pragma once
#include <list>
#include "../Card.h"
#include <vector>

class Deck {
public:
	Deck();
	virtual ~Deck();

	bool IsDeck();   //デッキと使用済みカード両方0ならそもそもAddしていない
	Card* GetCard();   //デッキの一番上のカードを一枚引く
	void AddCard(Card* card);
	void AddDiscard(Card* card);
	void Shuffle();
	void ResetDiscard();   //シャッフルされた使用済みカードを追加する
	
private:
	std::list<Card*> _lDeck;   //listの先頭が一番上
	std::vector<Card*> _vDiscard;  //使用済みカードを

};