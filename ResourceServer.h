#pragma once

#include <unordered_map>   //決められた順序で並んでいない
#include<Tchar.h>

/* リソース管理サーバ
 すべて静的メンバで構成する
 LoadGraphをmapを使った配列に例 _mapGraph[/png/player.png]の中身識別番号
 関数のstatic定義はいらない
*/
class ResourceServer {
public:
	static void	Init();   //staticだとインスタンスなしで呼べる
	static void	Release();
	static void	ClearGraph();

	static int LoadGraph(const TCHAR* FileName);
	static bool DeleteGraph(int handle);

};