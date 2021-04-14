#pragma once
#include <list>
#include "../Card.h"
#include <vector>

class Deck {
public:
	Deck();
	virtual ~Deck();

	bool IsDeck();   //�f�b�L�Ǝg�p�ς݃J�[�h����0�Ȃ炻������Add���Ă��Ȃ�
	Card* GetCard();   //�f�b�L�̈�ԏ�̃J�[�h���ꖇ����
	void AddCard(Card* card);
	void AddDiscard(Card* card);
	void Shuffle();
	void ResetDiscard();   //�V���b�t�����ꂽ�g�p�ς݃J�[�h��ǉ�����
	
private:
	std::list<Card*> _lDeck;   //list�̐擪����ԏ�
	std::vector<Card*> _vDiscard;  //�g�p�ς݃J�[�h��

};