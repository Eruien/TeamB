#pragma once
#include "LObject.h"
#include "Animation.h"
#include "MonoBehaviour.h"


struct animInfo
{
	wstring name;
	//wstring textureKey;
	bool isLoop;
	vector<Keyframe> keyFrames;
};

class KSpriteObj : public LObject , public enable_shared_from_this<KSpriteObj>
{
public:
	virtual ~KSpriteObj() {};
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
	//애니메이션
	void CreateAnimation(animInfo info);
	Animation* GetCurrentAnimation();
	const Keyframe& GetCurrentKeyframe(){ return _currentAnimation->GetKeyframe(_currentKeyframeIndex); }
	void SetAnimation(Animation* animation) { _currentAnimation = animation; }
	// Rect
	void SetRect(TVector3 pos, TVector3 scale);
	TRectangle GetRect() { return _rect; };
	bool CreateVertexBuffer() override;
	//스크립트
	void AddScripts(shared_ptr<MonoBehaviour> script) { script->SetGameObject(shared_from_this()); _scripts.push_back(script); };

	shared_ptr<MonoBehaviour> GetScript(wstring name);
//애니메이션
protected:
	float _sumTime = 0.f;
	INT32 _currentKeyframeIndex = 0;
	Animation* _currentAnimation;
//Rect
protected:
	TRectangle _rect;


protected:
	vector<shared_ptr<MonoBehaviour>> _scripts;
};

