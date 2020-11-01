#include "DeckSelectBG.h"
#include "ResourceServer.h"

DeckSelectBG::DeckSelectBG() : ObjectBase(ObjectBase::TYPE::DeckSelectBG) {

}

DeckSelectBG::~DeckSelectBG() {
}

bool DeckSelectBG::Init() {

	int handle = ResourceServer::LoadGraph(_T("png/back/back_deck.png"));   //–³’n

	if (handle == -1 ) {
		return false;
	}

	DrawInfo drawInfo = { handle,{0,0},false };

	_vDrawInfo.push_back(drawInfo);

	return true;
}

void DeckSelectBG::DeleteImage() {

	if (_vDrawInfo.size() == 0) {
		return;
	}

	DrawInfo drawInfo = _vDrawInfo.front();

	ResourceServer::DeleteGraph(drawInfo.handle);

	_vDrawInfo.clear();
}