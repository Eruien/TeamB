#include "DragUI.h"
#include "PickingUI.h"
#include "KSpriteObj.h"
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
	if (dynamic_pointer_cast<Resize2D>(GetGameObject()->GetScript(L"Resize2D")) != nullptr)
	{
		_resize2D = static_pointer_cast<Resize2D>(GetGameObject()->GetScript(L"Resize2D"));
	}
}

void DragUI::Frame()
{
	if (static_pointer_cast<PickingUI>(GetGameObject()->GetScript(L"PickingUI"))->GetButtonState() == PICKING_STATE::HOLD && !_resize2D->GetisBarPicking() )
		GetGameObject()->SetPos({ (float)MOUSEX,(float)MOUSEY, 1 });
}
