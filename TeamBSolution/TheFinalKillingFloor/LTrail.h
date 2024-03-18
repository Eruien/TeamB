#pragma once
#include "LObject.h"

class LTrail : public LObject
{
public:
	bool CreateVertexData() override;
	bool CreateIndexData() override;
};

