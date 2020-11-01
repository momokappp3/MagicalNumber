/*
** ���L���\�[�X�𐧌䂷�邽�߂̃T�[�o
*/

#include "DxLib.h"
#include "ResourceServer.h"
#include <vector>

namespace {
    // �ÓI�����o����
    std::unordered_map<std::string, int> _mapGraph;  //�t�@�C����
}

void ResourceServer::Init() {
    _mapGraph.clear();
}

void ResourceServer::Release() {   //�������ǂ����Ă���̂�
    ClearGraph();
}

void ResourceServer::ClearGraph() {

    // ���ׂẴf�[�^�̍폜������
    for (auto itr = _mapGraph.begin(); itr != _mapGraph.end(); itr++) {
        DeleteGraph(itr->second);   //�����ō폜���Ă���clear
    }
    _mapGraph.clear();
}

int ResourceServer::LoadGraph(const TCHAR* FileName) {

    // �L�[�̌���
    auto itr = _mapGraph.find(FileName);  //�����̃t�@�C�����̃L�[�̈ʒu
    
    if (itr != _mapGraph.end()) {   //�ŏI�v�f�̎��ł͂Ȃ�������@�@�����̐���

        // �L�[��������
        return itr->second;   //second = �n���h��
    }

    int cg = DxLib::LoadGraph(FileName);     // DXLIB��API���ĂԂ̂ŁA::��擪�ɕt���A���̃N���X�̓������O�̊֐��Ƌ�ʂ���
    // �L�[�ƃf�[�^��map�ɓo�^
    _mapGraph[FileName] = cg;

    return cg;   //���ʔԍ���Ԃ�
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