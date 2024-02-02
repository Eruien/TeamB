#pragma once
#include "LObject.h"


class LPlaneObj : public LObject
{
public:
	bool CreateVertexBuffer() override;
public:
	bool Init() override;
	bool Frame() override;
	bool  Load(
		TVector3 p,
		TVector3 s,
		std::basic_string<wchar_t> texFile, std::basic_string<wchar_t> shaderFile);//로드추가 수정

public:
	virtual ~LPlaneObj() {}
};


