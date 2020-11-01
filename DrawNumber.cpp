#include "DrawNumber.h"
#include "DxLib.h"
#include "ResourceServer.h"

namespace {
	unsigned long DrawInfoCount = 1;
}

DrawNumber::DrawNumber() {

	_aImageHandles.fill(-1);   //配列を-1で全て埋める
	_vDrawInfo.clear();

	_overlapRate = 0.0f;
	_minus = false;
}

DrawNumber::~DrawNumber() {
}

//LoadGraphと初期化　_aImageHandlesに識別番号
bool DrawNumber::Init(std::array<const TCHAR*, ImageHandlNum> imageNames, float overlapRate){

	for (int i = 0; i < imageNames.size();i++) {

		const TCHAR* image = imageNames[i];

		int handl = ResourceServer::LoadGraph(image);

		if (handl == -1) {
			return false;
		}

		_aImageHandles[i] = handl;
	}

	_overlapRate = overlapRate;

	return true;   //全てのLoadGraph成功
}

//mainでよぶ
unsigned long DrawNumber::SetNumber(int x, int y, int num, Anchor anchor, unsigned long id) {

	Info info = {};

	ConvertDigit(num, info.aDigit);

	//逆向きにarrayをまわす
	int count = DigitMax, lastSizeX = 0, lastSizeY = 0, tmpX = x;   //表示したいx座標
	bool first = true;
	std::array<int, DigitMax>::const_reverse_iterator itEnd = info.aDigit.rend();

	for (std::array<int, DigitMax>::const_reverse_iterator it = info.aDigit.rbegin(); it != itEnd; ++it) {
		auto digit = *it;

		count--;

		if (digit < 0 || digit > 9) {
			continue;
		}

		int handle = _aImageHandles[digit];
		int sizeX = 0, sizeY = 0;

		if (GetGraphSize(handle, &sizeX, &sizeY) == -1) {
			continue;
		}

		if (!first) {
			tmpX += lastSizeX;
			tmpX -= static_cast<int>(sizeX * _overlapRate);
		}

		info.aPoint[count] = { (tmpX - x), y };  //中{}は構造体を表す

		lastSizeX = sizeX;   //前ゲットサイズでとったサイズをlastに入れれば今のサイズがわかる
		lastSizeY = sizeY;
		first = false;
	}
	tmpX += lastSizeX;   //現在から今のサイズを足す

	int width = tmpX - x;   //足されていった座標 - 表示したい座標を引けば 幅がとれる　バグ箇所
	int height = lastSizeY;

	ConvertAnchor(x, y, width, height, anchor, info);

	info.isDraw = true;

	//更新
	if (id > 0) {
		for (int i = 0; i < _vDrawInfo.size(); i++) {
			if (_vDrawInfo[i].id == id) {
				info.id = id;
				_vDrawInfo[i] = info;
				return id;
			}
		}
	}

	info.id = DrawInfoCount;
	DrawInfoCount++;
	_vDrawInfo.push_back(info);

	return info.id;
}


bool DrawNumber::DeleteNumber(unsigned long id) {

	int index = -1;

	for (int i = 0; i < _vDrawInfo.size(); i++) {

		if (_vDrawInfo[i].id == id) {
			index = i;
			break;
		}
	}
	//for文の中ではeraseしない
	if (index >= 0) {
		_vDrawInfo.erase(_vDrawInfo.begin() + index);
		return true;
	}

	return false;
}


void DrawNumber::Draw() {

	for (int i = 0; i < _vDrawInfo.size();i++) {
		Info info = _vDrawInfo[i];

		Draw(info);
	}
}

void DrawNumber::Draw(Info info) {

	if (info.isDraw == false) {
		return;
	}

	int count = DigitMax;
	std::array<int, DigitMax>::const_reverse_iterator itEnd = info.aDigit.rend();

	for (std::array<int, DigitMax>::const_reverse_iterator it = info.aDigit.rbegin(); it != itEnd; ++it) {
		auto digit = *it;

		count--;

		if (digit < 0 || digit > 9 ) {
			continue;
		}

		int handle = _aImageHandles[digit];

		if (handle == -1) {
			continue;
		}

		Point point = info.aPoint[count];		

		DrawGraph(point.x, point.y, handle, TRUE);
	}
}

bool DrawNumber::SetIsDraw(unsigned long id, bool isDraw) {

	for (int i = 0; i < _vDrawInfo.size(); i++) {
		if (_vDrawInfo[i].id == id) {
			_vDrawInfo[i].isDraw = isDraw;
			return true;
		}
	}
	return false;
}

void DrawNumber::ConvertDigit(int num, std::array<int, DigitMax>& digit) {

	if (num < 0) {
		_minus = true;
		num = -num;   //+で処理できる
	}
	else {
		_minus = false;
	}
	if (num > 99999) {
		digit.fill(9);
		return;
	}
	digit.fill(-1);

	if (num == 0) {
		digit[0] = 0;
		return;
	}
	//分解
	
	int i = 0;

	while (num > 0) {
		int mod = num % 10; // 一の位の余り

		num = num / 10; // 取得済みの余りを"num"から除去
		digit[i++] = mod;
	}
}

//アンカー変換
void DrawNumber::ConvertAnchor(int x, int y, int width, int height, Anchor anchor, Info& info) {

	Point anchorPoint = { 0,0 };

	switch (anchor){
	case DrawNumber::Anchor::LeftUp:
		anchorPoint.x = x;
		anchorPoint.y = y;
		break;
	case DrawNumber::Anchor::CenterUp:
		anchorPoint.x = x - (width / 2);
		anchorPoint.y = y;
		break;
	case DrawNumber::Anchor::RightUp:
		anchorPoint.x = x - width;
		anchorPoint.y = y;
		break;
	case DrawNumber::Anchor::LeftCenter:
		anchorPoint.x = x;
		anchorPoint.y = y - (height / 2);
		break;
	case DrawNumber::Anchor::CenterCenter:
		anchorPoint.x = x - (width / 2);
		anchorPoint.y = y - (height / 2);
		break;
	case DrawNumber::Anchor::RightCenter:
		anchorPoint.x = x - width;
		anchorPoint.y = y - (height / 2);
		break;
	case DrawNumber::Anchor::LeftDown:
		anchorPoint.x = x;
		anchorPoint.y = y - height;
		break;
	case DrawNumber::Anchor::CenterDown:
		anchorPoint.x = x -(width / 2);
		anchorPoint.y = y - height;
		break;
	case DrawNumber::Anchor::RightDown:
		anchorPoint.x = x - width;
		anchorPoint.y = y - height;
		break;
	default:
		break;
	}

	for (int i = 0; i < info.aPoint.size();i++) {
		info.aPoint[i].x += anchorPoint.x;
		info.aPoint[i].y = anchorPoint.y;
	}
}