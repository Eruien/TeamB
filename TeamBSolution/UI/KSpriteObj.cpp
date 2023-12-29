#include "KSpriteObj.h"
#include "LGlobal.h"


bool KSpriteObj::Init()
{
	
		Set();
		LObject::Init();
		TMatrix matScale, matRotation, matTranslation;
		matScale.CreateScale(m_vScale);
		matRotation.CreateRotationZ(m_vRotation.z);
		matTranslation.Translation(m_vPosition);
		m_matWorld = matScale * matRotation * matTranslation;
		return true;
}

bool KSpriteObj::Frame()
{
	Animation* animation = GetCurrentAnimation();


	if (animation == nullptr)
		return false;

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

	TMatrix matScale, matRotation, matTranslation;
	matScale = matScale.CreateScale(m_vScale);
	matRotation= matRotation.CreateRotationX(m_vRotation.x);
	matRotation = matRotation.CreateRotationY(m_vRotation.y);
	matRotation = matRotation.CreateRotationZ(m_vRotation.z);
	matTranslation.Translation(m_vPosition);
	m_matWorld = matScale * matRotation * matTranslation;
	
	
	return true;
}

bool KSpriteObj::Render()
{
	PreRender();
	
	m_Tex->m_pTexSRV = LManager<LTexture>::GetInstance().Load(GetCurrentKeyframe().texFilePath)->m_pTexSRV;
	 
	m_Tex->Apply();

	PostRender();
	return false;
}

bool KSpriteObj::Release()
{
	return false;
}

void KSpriteObj::CreateAnimation(animInfo info)
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
Animation* KSpriteObj::GetCurrentAnimation()
{
	return _currentAnimation;
}
bool KSpriteObj::CreateVertexBuffer()
{
	m_VertexList.resize(6);

	m_VertexList[0].p.x = -1.0f; m_VertexList[0].p.y = 1.0f;  m_VertexList[0].p.z = 0.0f;
	m_VertexList[1].p.x = 1.0f;  m_VertexList[1].p.y = 1.0f;  m_VertexList[1].p.z = 0.0f;
	m_VertexList[2].p.x = -1.0f; m_VertexList[2].p.y = -1.0f; m_VertexList[2].p.z = 0.0f;

	m_VertexList[3].p.x = -1.0f; m_VertexList[3].p.y = -1.0f; m_VertexList[3].p.z = 0.0f;
	m_VertexList[4].p.x = 1.0f;  m_VertexList[4].p.y = 1.0f;  m_VertexList[4].p.z = 0.0f;
	m_VertexList[5].p.x = 1.0f;  m_VertexList[5].p.y = -1.0f; m_VertexList[5].p.z = 0.0f;

	m_VertexList[0].t.x = 0.0f;   m_VertexList[0].t.y = 0.0f;
	m_VertexList[1].t.x = 1.0f;  m_VertexList[1].t.y = 0.0f;
	m_VertexList[2].t.x = 0.0f;   m_VertexList[2].t.y = 1.0f;

	m_VertexList[3].t.x = 0.0f;   m_VertexList[3].t.y = 1.0f;
	m_VertexList[4].t.x = 1.0f;  m_VertexList[4].t.y = 0.0f;
	m_VertexList[5].t.x = 1.0f;  m_VertexList[5].t.y = 1.0f;

	// 버퍼에 대한 설명서
	D3D11_BUFFER_DESC bufferDesc = { 0, };
	bufferDesc.ByteWidth = sizeof(SimpleVertex) * m_VertexList.size();
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initialData = { 0, };
	initialData.pSysMem = &m_VertexList.at(0);

	HRESULT hr = m_pDevice->CreateBuffer(
		&bufferDesc,
		&initialData,
		&m_pVertexBuffer);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create Buffer Error", "Error Box", MB_OK);
		return false;
	}

	return true;
}