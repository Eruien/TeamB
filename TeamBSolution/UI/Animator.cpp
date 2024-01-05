#include "Animator.h"
#include "LGlobal.h"

Animator::Animator() : MonoBehaviour(L"Animator")
{
}

Animator::~Animator()
{
}

void Animator::Init()
{
}

void Animator::Frame()
{
	Animation* animation = GetCurrentAnimation();


	if (animation == nullptr)
		return;

	const Keyframe& keyframe = animation->GetKeyframe(_currentKeyframeIndex);

	float deltaTime = LGlobal::g_fSPF;
	_sumTime += deltaTime;

	if (_sumTime >= keyframe.time)
	{
		_currentKeyframeIndex++;
		INT32 totalCount = animation->GetKeyframeCount();

		if (_currentKeyframeIndex >= totalCount)
		{
			if (animation->IsLoop())
				_currentKeyframeIndex = 0;
			else
				_currentKeyframeIndex = totalCount - 1;
		}

		_sumTime = 0.f;
	}

}

void Animator::CreateAnimation(animInfo info)
{

	Animation* animation = new Animation();
	animation->SetName(info.name);
	//animation->SetTexture(Get<LTexture>(info.textureKey));
	animation->SetLoop(info.isLoop);
	//info.keyFrames 
	for (const auto& keyframe : info.keyFrames) {
		LTexture* tex = LManager<LTexture>::GetInstance().Load(keyframe.texFilePath);
		animation->AddKeyframe(Keyframe{ keyframe.texFilePath , keyframe.time });
	}
	LManager<Animation>::GetInstance().Add(info.name, animation);
	SetAnimation(animation);

}
Animation* Animator::GetCurrentAnimation()
{
	return _currentAnimation;
}