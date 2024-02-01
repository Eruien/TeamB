#include "LPlayer.h"

class PlayerDeath : public PlayerState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	PlayerDeath(LPlayer* parent);
	virtual ~PlayerDeath();
};
