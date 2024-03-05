#include "Tank.h"

class TankRush : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
	void UpdateHPbar();
public:
	float m_DamageRate = 0.1f;
	float m_HeadShootRate = 2.0f;
public:
	TankRush(Tank* parent);
	virtual ~TankRush();
};

