#include "Resize2D.h"
#include "KSpriteObj.h"
#include "LGlobal.h"
#include "LStd.h"
#include "LInput.h"
#include "LWrite.h"
Resize2D::Resize2D() : MonoBehaviour(L"Resize2D")
{
}

Resize2D::~Resize2D()
{
}

void Resize2D::Init()
{
	//resize
	


	

	
}

void Resize2D::Frame()
{
	TRectangle	rTop = { GetGameObject()->GetRect().x,
	GetGameObject()->GetRect().y + GetGameObject()->GetRect().height/2,
	GetGameObject()->GetRect().width,
		_barSize };

	TRectangle	rBottom = { GetGameObject()->GetRect().x,
	GetGameObject()->GetRect().y - GetGameObject()->GetRect().height / 2,
	GetGameObject()->GetRect().width,
		_barSize };

	TRectangle	rLeft = { GetGameObject()->GetRect().x - GetGameObject()->GetRect().width / 2,
	GetGameObject()->GetRect().y,
	_barSize,
	GetGameObject()->GetRect().height };

	TRectangle	rRight = { GetGameObject()->GetRect().x + GetGameObject()->GetRect().width / 2,
	GetGameObject()->GetRect().y,
	_barSize,
	GetGameObject()->GetRect().height };
	barPicking = false;

	if (TBASIS_EX::TRectangle::RectToPoint(rTop, {(long)MOUSEX,(long)MOUSEY}))
	{
		SetCursor(LoadCursor(NULL, IDC_SIZENS));
		barPicking = true;
		if (LINPUT.m_MouseState[0] == KEY_HOLD)
		{
			GetGameObject()->m_vScale.y = (MOUSEY- GetGameObject()->m_vPosition.y)*2;
			
		}
	}
	else if (TBASIS_EX::TRectangle::RectToPoint(rBottom, { (long)MOUSEX,(long)MOUSEY }))
	{
		SetCursor(LoadCursor(NULL, IDC_SIZENS));
		barPicking = true;
		if (LINPUT.m_MouseState[0] == KEY_HOLD)
		{
			GetGameObject()->m_vScale.y = (MOUSEY - GetGameObject()->m_vPosition.y) * 2;
		}
	}
	else if (TBASIS_EX::TRectangle::RectToPoint(rLeft, { (long)MOUSEX,(long)MOUSEY }))
	{
		SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		barPicking = true;
		if (LINPUT.m_MouseState[0] == KEY_HOLD)
		{
			GetGameObject()->m_vScale.x = (MOUSEX - GetGameObject()->m_vPosition.x) * 2;
		}
	}
	else if (TBASIS_EX::TRectangle::RectToPoint(rRight, { (long)MOUSEX,(long)MOUSEY }))
	{
		SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		barPicking = true;
		if (LINPUT.m_MouseState[0] == KEY_HOLD)
		{
			GetGameObject()->m_vScale.x = (MOUSEX - GetGameObject()->m_vPosition.x) * 2;
		}
	}
	
	};
