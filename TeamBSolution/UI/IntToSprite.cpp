#include "IntToSprite.h"
#include "KObject.h"
#include "LGlobal.h"
#include "LManager.h"
IntToSprite::IntToSprite(int num, vector<wstring> tex) : MonoBehaviour(L"IntToSprite"), _num(num), _numTex(tex)
{
    _num = num;
}

IntToSprite::~IntToSprite()
{
}

void IntToSprite::Init()
{
}

void IntToSprite::Frame()
{
    
  
    switch (_num) {
    case 0:
        GetGameObject()->m_Tex->m_pTexSRV = LManager<LTexture>::GetInstance().Load(_numTex[0])->m_pTexSRV;
        GetGameObject()->m_Tex->Apply();
        break;
    case 1:
        GetGameObject()->m_Tex->m_pTexSRV = LManager<LTexture>::GetInstance().Load(_numTex[1])->m_pTexSRV;
        GetGameObject()->m_Tex->Apply();
        break;
    case 2:
        GetGameObject()->m_Tex->m_pTexSRV = LManager<LTexture>::GetInstance().Load(_numTex[2])->m_pTexSRV;
        GetGameObject()->m_Tex->Apply();
        break;
    case 3:
        GetGameObject()->m_Tex->m_pTexSRV = LManager<LTexture>::GetInstance().Load(_numTex[3])->m_pTexSRV;
        GetGameObject()->m_Tex->Apply();
        break;
    case 4:
        GetGameObject()->m_Tex->m_pTexSRV = LManager<LTexture>::GetInstance().Load(_numTex[4])->m_pTexSRV;
        GetGameObject()->m_Tex->Apply();
        break;
    case 5:
        GetGameObject()->m_Tex->m_pTexSRV = LManager<LTexture>::GetInstance().Load(_numTex[5])->m_pTexSRV;
        GetGameObject()->m_Tex->Apply();
        break;
    case 6:
        GetGameObject()->m_Tex->m_pTexSRV = LManager<LTexture>::GetInstance().Load(_numTex[6])->m_pTexSRV;
        GetGameObject()->m_Tex->Apply();
        break;
    case 7:
        GetGameObject()->m_Tex->m_pTexSRV = LManager<LTexture>::GetInstance().Load(_numTex[7])->m_pTexSRV;
        GetGameObject()->m_Tex->Apply();
        break;
    case 8:
        GetGameObject()->m_Tex->m_pTexSRV = LManager<LTexture>::GetInstance().Load(_numTex[8])->m_pTexSRV;
        GetGameObject()->m_Tex->Apply();
        break;
    case 9:
        GetGameObject()->m_Tex->m_pTexSRV = LManager<LTexture>::GetInstance().Load(_numTex[9])->m_pTexSRV;
        GetGameObject()->m_Tex->Apply();
        break;
    default:
        std::cout << "Invalid value" << std::endl;
        break;
    }
}
