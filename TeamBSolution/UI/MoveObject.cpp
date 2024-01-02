#include "MoveObject.h"
#include "LGlobal.h"
#include "LInput.h"
#include "KSpriteObj.h"

MoveObject::MoveObject() : MonoBehaviour(L"MoveObject")
{

}

MoveObject::~MoveObject()
{
}

void MoveObject::Init()
{
}

void MoveObject::Frame()
{
	GetGameObject()->m_vPosition.x += 0.01;
}
