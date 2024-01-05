#include "DragUI.h"
#include "PickingUI.h"
#include "KObject.h"
#include "LStd.h"
#include "LInput.h"
#include "LGlobal.h"
#include "Resize2D.h"
DragUI::DragUI() : MonoBehaviour(L"DragUI")
{
	
}

DragUI::~DragUI()
{
}

void DragUI::Init()
{
	if ((GetGameObject()->GetScript<Resize2D>(L"Resize2D")) != nullptr)
	{
		_resize2D =(GetGameObject()->GetScript<Resize2D>(L"Resize2D"));
	}
}

void DragUI::Frame()
{
	if ((GetGameObject()->GetScript<PickingUI>(L"PickingUI"))->GetIsSelected())
	{
		if ((GetGameObject()->GetScript<PickingUI>(L"PickingUI"))->GetButtonState() == PICKING_STATE::HOLD && !_resize2D->GetisBarPicking())
			GetGameObject()->SetPos({ (float)MOUSEX,(float)MOUSEY, 1 });
	}
}
