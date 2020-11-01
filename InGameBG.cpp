#include "InGameBG.h"
#include "ResourceServer.h"

InGameBG::InGameBG() : ObjectBase(ObjectBase::TYPE::InGameBG){

}

InGameBG::~InGameBG() {
}

bool InGameBG::Init() {

	int handle0 = ResourceServer::LoadGraph(_T("png/back/back01.png"));   //–³’n
	//int handle1 = ResourceServer::LoadGraph(_T("png/back/back02.png"));   //–³’n

	if (handle0 == -1/* || handle1 == -1*/) {
		return false;
	}

	DrawInfo drawInfo0 = { handle0,{0,0},false };
	//DrawInfo drawInfo1 = { handle1,{0,0},false };

	_vDrawInfo.push_back(drawInfo0);
	//_vDrawInfo.push_back(drawInfo1);

	return true;
}