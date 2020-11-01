#include "Deck.h"
#include <random>
#include <vector>
#include <iterator>
#include "MTRandom.h"

Deck::Deck() {
	_lDeck.clear();
	_vDiscard.clear();
}

Deck::~Deck() {   //カードは移動するから移動した先でdeleteするスマートポインタにしない

	for (auto itr = _lDeck.begin(); itr != _lDeck.end();itr++) {

		Card* card = *itr;

		if (card != nullptr) {
			delete card;
		}
	}
	_lDeck.clear();

	for (int i = 0; i < _vDiscard.size(); i++) {

		if (_vDiscard[i] != nullptr) {
			delete _vDiscard[i];
			_vDiscard[i] = nullptr;
		}
	}
	_vDiscard.clear();
}

bool Deck::IsDeck() {

	if (_lDeck.size() == 0 && _vDiscard.size() == 0) {
		return false;
	}

	return true;
}

Card* Deck:: GetCard() {

	if (_lDeck.size() == 0) {
		return nullptr;
	}

	Card* card = _lDeck.front();
											
	   //先頭要素への参照を取得
	_lDeck.pop_front();

	return card;   //先頭要素がリターン
}

void Deck::AddCard(Card* card) {

	if (card == nullptr) {
		return;
	}

	_lDeck.push_back(card);   //listの最後に追加
}

void Deck::AddDiscard(Card* card) {

	if (card == nullptr) {
		return;
	}

	_vDiscard.push_back(card);
}

void Deck::Shuffle() {                                       //リストでエラーが出たため

	std::vector <Card*> deck(_lDeck.begin(),_lDeck.end());   //リストの内容をベクターにコピーして

	std::mt19937_64 random = MTRandom::GetRandom();      //MTRandomクラスのGetRandom呼び出し
	std::shuffle(deck.begin(), deck.end(), random);  //今のリストがシャッフルされた

	_lDeck.clear();                                         //元からあったものを消して
	//std::copy(deck.begin(), deck.end(), _lDeck.begin());   //ベクターでシャッフルしたものをコピー (非効率)
	
	for (int i = 0; i < deck.size(); i++) {
		_lDeck.push_back(deck[i]);
	}

}

void Deck::ResetDiscard() {

	std::mt19937_64 random = MTRandom::GetRandom();
	std::shuffle(_vDiscard.begin(), _vDiscard.end(), random);

	std::copy(_vDiscard.begin(), _vDiscard.end(), std::back_inserter(_lDeck));   //デッキにシャッフルされた使用済みカードを後ろに足す
}

