#include "Button.h"
#include "KSpriteObj.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"
#include "TMath.h"
#include "LStd.h"


Button::Button() : MonoBehaviour(L"Button")
{
}
Button::~Button()
{
}
void Button::Init()
{
}

void Button::Frame()
{
	//마우스, 오브젝트 좌표 출력
	LWrite::GetInstance().AddText(to_wstring(LInput::GetInstance().GetWorldPos(LGlobal::g_WindowWidth, LGlobal::g_WindowHeight
	, LGlobal::g_pMainCamera->m_vCameraPos.x, LGlobal::g_pMainCamera->m_vCameraPos.y).x), 100, 100);
	LWrite::GetInstance().AddText(to_wstring(LInput::GetInstance().GetWorldPos(LGlobal::g_WindowWidth, LGlobal::g_WindowHeight
		, LGlobal::g_pMainCamera->m_vCameraPos.x, LGlobal::g_pMainCamera->m_vCameraPos.y).y), 300, 100);

	LWrite::GetInstance().AddText(to_wstring(GetGameObject()->GetRect().x), 100, 300);
	LWrite::GetInstance().AddText(to_wstring(GetGameObject()->GetRect().y), 200, 300);

	POINT mouse ;
	mouse.x = LINPUT.GetWorldPos(LGlobal::g_WindowWidth, LGlobal::g_WindowHeight
		, LGlobal::g_pMainCamera->m_vCameraPos.x, LGlobal::g_pMainCamera->m_vCameraPos.y).x;
	mouse.y = LINPUT.GetWorldPos(LGlobal::g_WindowWidth, LGlobal::g_WindowHeight
		, LGlobal::g_pMainCamera->m_vCameraPos.x, LGlobal::g_pMainCamera->m_vCameraPos.y).y;

	TRectangle rect;
	
	rect.x = GetGameObject()->GetRect().x;
	rect.y = GetGameObject()->GetRect().y;
	rect.height = GetGameObject()->GetRect().height;
	rect.width = GetGameObject()->GetRect().width;

	_buttonState = BUTTON_STATE::NONE;// 맨마지막에
	if (TBASIS_EX::TRectangle::RectToPoint(rect, mouse))
	{
		if (LINPUT.m_MouseState[0] == KEY_PUSH)
		{
			_buttonState = BUTTON_STATE::PRESS;
		}
		else if(LINPUT.m_MouseState[0] == KEY_HOLD)
			_buttonState = BUTTON_STATE::HOLD;
		else if (LINPUT.m_MouseState[0] == KEY_UP)
			_buttonState = BUTTON_STATE::UP;
		else
		_buttonState = BUTTON_STATE::HOVER;
	}
	LWRITE.AddText(to_wstring((int)_buttonState), 500, 100);
	
	if (_buttonState == BUTTON_STATE::HOLD)
		GetGameObject()->SetPos({ (float)mouse.x,(float)mouse.y,1 });
}
