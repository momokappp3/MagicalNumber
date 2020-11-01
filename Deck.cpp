#include "Deck.h"
#include <random>
#include <vector>
#include <iterator>
#include "MTRandom.h"

Deck::Deck() {
	_lDeck.clear();
	_vDiscard.clear();
}

Deck::~Deck() {   //�J�[�h�͈ړ����邩��ړ��������delete����X�}�[�g�|�C���^�ɂ��Ȃ�

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
											
	   //�擪�v�f�ւ̎Q�Ƃ��擾
	_lDeck.pop_front();

	return card;   //�擪�v�f�����^�[��
}

void Deck::AddCard(Card* card) {

	if (card == nullptr) {
		return;
	}

	_lDeck.push_back(card);   //list�̍Ō�ɒǉ�
}

void Deck::AddDiscard(Card* card) {

	if (card == nullptr) {
		return;
	}

	_vDiscard.push_back(card);
}

void Deck::Shuffle() {                                       //���X�g�ŃG���[���o������

	std::vector <Card*> deck(_lDeck.begin(),_lDeck.end());   //���X�g�̓��e���x�N�^�[�ɃR�s�[����

	std::mt19937_64 random = MTRandom::GetRandom();      //MTRandom�N���X��GetRandom�Ăяo��
	std::shuffle(deck.begin(), deck.end(), random);  //���̃��X�g���V���b�t�����ꂽ

	_lDeck.clear();                                         //�����炠�������̂�������
	//std::copy(deck.begin(), deck.end(), _lDeck.begin());   //�x�N�^�[�ŃV���b�t���������̂��R�s�[ (�����)
	
	for (int i = 0; i < deck.size(); i++) {
		_lDeck.push_back(deck[i]);
	}

}

void Deck::ResetDiscard() {

	std::mt19937_64 random = MTRandom::GetRandom();
	std::shuffle(_vDiscard.begin(), _vDiscard.end(), random);

	std::copy(_vDiscard.begin(), _vDiscard.end(), std::back_inserter(_lDeck));   //�f�b�L�ɃV���b�t�����ꂽ�g�p�ς݃J�[�h�����ɑ���
}

