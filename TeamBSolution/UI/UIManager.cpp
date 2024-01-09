#include "UIManager.h"
#include "LStd.h"
void UIManager::Init()
{
	shared_ptr<FunctionManager> functionManager = make_shared<FunctionManager>();
	functionManager->Init();
	SetFunctionManager(functionManager);



}

void UIManager::Frame()
{
}

void UIManager::Render()
{
}


