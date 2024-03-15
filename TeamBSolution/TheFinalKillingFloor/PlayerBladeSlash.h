#include "LPlayer.h"

class PlayerBladeSlash : public PlayerState
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
	PlayerBladeSlash(LPlayer* parent);
	virtual ~PlayerBladeSlash();
};

