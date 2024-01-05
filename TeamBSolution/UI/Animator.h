#pragma once
#include "MonoBehaviour.h"
#include "Animation.h"

struct animInfo
{
	wstring name;
	//wstring textureKey;
	bool isLoop;
	vector<Keyframe> keyFrames;
};

class Animator : public MonoBehaviour
{
public:
	Animator();
	virtual ~Animator();

	void Init() override;
	void Frame() override;

	//애니메이션
	void CreateAnimation(animInfo info);
	Animation* GetCurrentAnimation();
	const Keyframe& GetCurrentKeyframe() { return _currentAnimation->GetKeyframe(_currentKeyframeIndex); }
	void SetAnimation(Animation* animation) { _currentAnimation = animation; }
	//애니메이션
protected:
	float _sumTime = 0.f;
	INT32 _currentKeyframeIndex = 0;
	Animation* _currentAnimation;
};

