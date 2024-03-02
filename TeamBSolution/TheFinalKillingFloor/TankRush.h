#include "Tank.h"

class TankRush : public TankState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
	void UpdateHPbar();
public:
	TankRush(Tank* parent);
	virtual ~TankRush();
};

