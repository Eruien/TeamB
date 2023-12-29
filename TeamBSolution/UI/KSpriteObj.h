#pragma once
#include "LObject.h"
#include "Animation.h"
//#include "LManagerAgr.h"


struct animInfo
{
	wstring name;
	//wstring textureKey;
	bool isLoop;
	vector<Keyframe> keyFrames;
};

class KSpriteObj : public LObject
{
public:
	virtual ~KSpriteObj() {};
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
	void CreateAnimation(animInfo info);
	Animation* GetCurrentAnimation();
	const Keyframe& GetCurrentKeyframe(){ return _currentAnimation->GetKeyframe(_currentKeyframeIndex); }
	void SetAnimation(Animation* animation) { _currentAnimation = animation; }
	bool CreateVertexBuffer() override;
protected:
	float _sumTime = 0.f;
	INT32 _currentKeyframeIndex = 0;
	Animation* _currentAnimation;
};

