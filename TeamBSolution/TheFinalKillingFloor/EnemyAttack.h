#include "LNPC.h"

class EnemyAttack : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	bool m_Timer = false;
public:
	EnemyAttack(LNPC* parent);
	virtual ~EnemyAttack();
};


