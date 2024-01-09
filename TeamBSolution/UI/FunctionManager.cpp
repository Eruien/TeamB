#include "FunctionManager.h"
#include "LInput.h"
#include "LGlobal.h"
#include "LStd.h"
#include "LWrite.h"
#include "Resize2D.h"
void FunctionManager::TestFunc()
{
	LWRITE.AddText(L"NONE", 200, 100);
}

void FunctionManager::Init()
{
	/*AddDelegate("MemberFunctionDelegate", [this]() {
		TestFunc();
		});*/
	/*AddDelegate("Test", [](int a, int b) {
		LWRITE.AddText(L"NONE", a, 100);
		});*/

}
