#pragma once
#include "../ObjectBace.h"
#include "../Use/Types.h"
#include <vector>
#include "../Card.h"
#include <array>
#include "../Sign.h"
#include "../Input.h"
#include <functional>
#include "../Use/SoundManager.h"
#include <memory>

class Field : public ObjectBase{
public:
	Field(bool is1P);
	virtual ~Field();

	bool Init(std::function<bool(Input::INPUT, bool)> isInput, std::shared_ptr<SoundManager>& soundManager);
	void TurnInit();
	void Process() override;
	void Draw() override;
	void ProcessSelect();
	Select* GetSelect(int index) {
		if (index < 0 || index >= _vSelect.size()) {
			return nullptr;
		}
		return _vSelect[index];
	}

	bool SetSelect(int index, Select* select) {
		if (index < 0 || index >= _vSelect.size()) {
			return false;
		}
		_vSelect[index] = select;
		return true;
	}

	int GetSelectSize() {
		return _vSelect.size();
	}

	int GetOnSelect() {
		return _onSelect;
	}

	void SetAddCount() {
		_addCount++;
	}

	void SetSubCount() {
		_addCount--;
	}

	void SetFinish(bool finish) {
		_isFinish = finish;
	}

private:
	void SetSignPosition();
	void SetHandPosition();

	bool IsSign(int select);
	bool IsNumber(int select);
	bool IsNumberBottom(int select);
	bool IsRightEdge(int select);
	bool IsLeftEdge(int select);

                      //Input�N���X��INPUT enum class�^�@���bool
	std::function<bool(Input::INPUT, bool)> _pIsInput;   //Input��IsInput�Ɠ����ɂȂ��Ă���
	std::shared_ptr<SoundManager> _pSoundManager;

	std::vector<Point> _vPoint;    //�L���ƃJ�[�h�̍��W
	std::vector<Select*> _vSelect;   //�I���ł������(�L���Ǝ�D)

	Point _onlySelectPoint;

	int _handleSelect;
	int _select;   //���݂̈ʒu�I��ł���ꏊ�̉���
	int _onSelect;
	int _addCount;

	bool _is1P;
	bool _isFinish;

};
