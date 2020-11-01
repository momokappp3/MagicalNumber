#pragma once
#include "Select.h"

class Sign : public Select {
public:
	Sign();
	Sign(const Sign&) = default;   //�C���X�^���X���R�s�[�@�R�s�[�R���X�g���N�^
	virtual ~Sign();

	enum class Kind {
		Plus = 0,   //�킩��₷��
		Minus,
		Times,
		Divide,
		BeginBracket,
		EndBracket,

		Max
	};

	static constexpr int SignMax = static_cast <int>(Kind::Max);

	bool Init(Kind kind);
	bool SetSelectXY(int x, int y) override;

	Kind GetKind() {
		return _kind;
	}

private:
	bool SetDrawInfo(int kind, DrawInfo& info);
	bool SetDrawInfoSelect(DrawInfo& info);

	Kind _kind;

};
