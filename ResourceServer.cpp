/*
** 共有リソースを制御するためのサーバ
*/

#include "DxLib.h"
#include "ResourceServer.h"
#include <vector>

namespace {
    // 静的メンバ実体
    std::unordered_map<std::string, int> _mapGraph;  //ファイル名
}

void ResourceServer::Init() {
    _mapGraph.clear();
}

void ResourceServer::Release() {   //ここをどうしてするのか
    ClearGraph();
}

void ResourceServer::ClearGraph() {

    // すべてのデータの削除をする
    for (auto itr = _mapGraph.begin(); itr != _mapGraph.end(); itr++) {
        DeleteGraph(itr->second);   //ここで削除してからclear
    }
    _mapGraph.clear();
}

int ResourceServer::LoadGraph(const TCHAR* FileName) {

    // キーの検索
    auto itr = _mapGraph.find(FileName);  //引数のファイル名のキーの位置
    
    if (itr != _mapGraph.end()) {   //最終要素の次ではなかったら　　ここの説明

        // キーがあった
        return itr->second;   //second = ハンドル
    }

    int cg = DxLib::LoadGraph(FileName);     // DXLIBのAPIを呼ぶので、::を先頭に付け、このクラスの同じ名前の関数と区別する
    // キーとデータをmapに登録
    _mapGraph[FileName] = cg;

    return cg;   //識別番号を返す
}

bool ResourceServer::DeleteGraph(int handle) {

    if (handle == -1) {
        return false;
    }

    for (auto itr = _mapGraph.begin(); itr != _mapGraph.end(); itr++) {
        if (itr->second == handle) {
            DxLib::DeleteGraph(handle);

            _mapGraph.erase(itr);

            return true;
        }
    }

    return false;
}