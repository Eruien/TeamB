#pragma once
#include "LPlayer.h"

class PlayerAttack : public PlayerState
{
public:
	bool IsClick = false;
	bool IsFirstClick = false;
	ComboType m_CurrentCombo = ComboType::INWARD;
public:
	bool Montage(int startFrame, int endFrame);
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	PlayerAttack(LPlayer* parent);
	virtual ~PlayerAttack();
};

