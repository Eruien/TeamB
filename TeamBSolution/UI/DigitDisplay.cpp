#include "DigitDisplay.h"
#include "KObject.h"
#include "LGlobal.h"
#include "LCamera.h"
DigitDisplay::DigitDisplay(int digit, wstring texXmlPath) : _digitNum(digit), _texXmlPath(texXmlPath), MonoBehaviour(L"DigitDisplay")
{
    
}

DigitDisplay::~DigitDisplay()
{
}

void DigitDisplay::Init()
{
	for (int i = 0; i < _digitNum; i++)
	{
		_objects.push_back(make_shared<KObject>());
		_objects[i]->Init();
		_objects[i]->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/0.png");
		TVector3 pos =GetGameObject()->m_vPosition;
		pos.x = GetGameObject()->m_vPosition.x + (GetGameObject()->m_vScale.x / _digitNum) - (i * (GetGameObject()->m_vScale.x / _digitNum));
		_objects[i]->SetPos(pos);
		_objects[i]->SetScale({ GetGameObject()->m_vScale.x / _digitNum,GetGameObject()->m_vScale.y,0 });
	}

}

void DigitDisplay::Frame()
{

	for (int i = 0; i < _digitNum; i++)
	{

		TVector3 pos = GetGameObject()->m_vPosition;
		pos.x = GetGameObject()->m_vPosition.x + (GetGameObject()->m_vScale.x / _digitNum) - (i * (GetGameObject()->m_vScale.x / _digitNum));
		_objects[i]->SetPos(pos);
		_objects[i]->SetScale({ GetGameObject()->m_vScale.x / _digitNum,GetGameObject()->m_vScale.y,0 });
		_objects[i]->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
		_objects[i]->Frame();

	}

}

void DigitDisplay::Render()
{
	for (auto obj : _objects)
	{
		obj->Render();
	}
}

void DigitDisplay::UpdateNumber()
{
    vector<int> digits;
    //int temp = _num;

    int temp = LGlobal::g_fGameTimer;

    while (temp > 0) {
        int digit = temp % 10;
        digits.push_back(digit);
        temp /= 10;
    }

    for (int i = 0; i < _digitNum; i++)
    {
        _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList[0]);
        _objects[i]->m_Tex->Apply();
    }
    for (int i = 0; i < digits.size(); i++)
    {

        switch (digits[i]) {
        case 0:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList[0]);
            _objects[i]->m_Tex->Apply();
            break;
        case 1:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList[1]);
            _objects[i]->m_Tex->Apply();
            break;
        case 2:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList[2]);
            _objects[i]->m_Tex->Apply();
            break;
        case 3:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList[3]);
            _objects[i]->m_Tex->Apply();
            break;
        case 4:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList[4]);
            _objects[i]->m_Tex->Apply();
            break;
        case 5:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList[5]);
            _objects[i]->m_Tex->Apply();
            break;
        case 6:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList[6]);
            _objects[i]->m_Tex->Apply();
            break;
        case 7:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList[7]);
            _objects[i]->m_Tex->Apply();
            break;
        case 8:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList[8]);
            _objects[i]->m_Tex->Apply();
            break;
        case 9:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList[9]);
            _objects[i]->m_Tex->Apply();
            break;
        default:
            std::cout << "Invalid value" << std::endl;
            break;
        }



    }
}

void DigitDisplay::Load()
{

}

void DigitDisplay::Save()
{
    
}
