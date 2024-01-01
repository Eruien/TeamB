#include "PickingUI.h"
#include "KSpriteObj.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"
#include "TMath.h"
#include "LStd.h"


PickingUI::PickingUI() : MonoBehaviour(L"PickingUI")
{
}
PickingUI::~PickingUI()
{
}
void PickingUI::Init()
{
}

void PickingUI::Frame()
{
	//���콺, ������Ʈ ��ǥ ���
	LWrite::GetInstance().AddText(to_wstring(LInput::GetInstance().GetWorldPos(LGlobal::g_WindowWidth, LGlobal::g_WindowHeight
	, LGlobal::g_pMainCamera->m_vCameraPos.x, LGlobal::g_pMainCamera->m_vCameraPos.y).x), 100, 100);
	LWrite::GetInstance().AddText(to_wstring(LInput::GetInstance().GetWorldPos(LGlobal::g_WindowWidth, LGlobal::g_WindowHeight
		, LGlobal::g_pMainCamera->m_vCameraPos.x, LGlobal::g_pMainCamera->m_vCameraPos.y).y), 300, 100);

	LWrite::GetInstance().AddText(to_wstring(GetGameObject()->GetRect().x), 100, 300);
	LWrite::GetInstance().AddText(to_wstring(GetGameObject()->GetRect().y), 200, 300);
	LWrite::GetInstance().AddText(to_wstring(GetGameObject()->m_vScale.x), 100, 400);
	LWrite::GetInstance().AddText(to_wstring(GetGameObject()->m_vScale.y), 100, 450);

	POINT mouse ;
	mouse.x = MOUSEX;
	mouse.y = MOUSEY;




	TRectangle rect;
	
	rect.x = GetGameObject()->GetRect().x;
	rect.y = GetGameObject()->GetRect().y;
	rect.height = GetGameObject()->GetRect().height;
	rect.width = GetGameObject()->GetRect().width;


	

	_buttonState = PICKING_STATE::NONE;// �Ǹ�������
	if (TBASIS_EX::TRectangle::RectToPoint(rect, mouse))
	{
		if (LINPUT.m_MouseState[0] == KEY_PUSH)
		{
			_buttonState = PICKING_STATE::PRESS;
		}
		else if(LINPUT.m_MouseState[0] == KEY_HOLD)
			_buttonState = PICKING_STATE::HOLD;
		else if (LINPUT.m_MouseState[0] == KEY_UP)
			_buttonState = PICKING_STATE::UP;
		else
		_buttonState = PICKING_STATE::HOVER;
	}

		switch (_buttonState)
		{
		case PICKING_STATE::NONE:
			LWRITE.AddText(L"NONE", 500, 100);
			break;
		case PICKING_STATE::HOVER:
			LWRITE.AddText(L"HOVER", 500, 100);
			break;
		case PICKING_STATE::PRESS:
			LWRITE.AddText(L"PRESS", 500, 100);
			break;
		case PICKING_STATE::HOLD:
			LWRITE.AddText(L"HOLD", 500, 100);
			break;
		case PICKING_STATE::UP:
			LWRITE.AddText(L"UP", 500, 100);
			break;
		default:
			break;
		}
	

	


}
