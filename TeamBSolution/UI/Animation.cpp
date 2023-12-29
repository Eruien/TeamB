#include "Animation.h"


Animation::Animation() : ResourceBase(ResourceType::Animation)
{

}

Animation::~Animation()
{

}




const Keyframe& Animation::GetKeyframe(INT32 index)
{
	return _keyframes[index];
}

INT32 Animation::GetKeyframeCount()
{
	return static_cast<INT32>(_keyframes.size());
}

void Animation::AddKeyframe(const Keyframe& keyframe)
{
	_keyframes.push_back(keyframe);
}
