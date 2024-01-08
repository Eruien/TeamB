#include "Imgui_ObjectDetail.h"
#include "KObject.h"
#include "PickingUI.h"
#include "ChangeTexture.h"
#include "LGlobal.h"
#include <WICTextureLoader.h>
#pragma comment(lib, "DirectXTK_D.lib")
#include "Sample.h"

bool Imgui_ObjectDetail::_isDialogWindow = false;
void Imgui_ObjectDetail::Init()
{
}

void Imgui_ObjectDetail::Frame()
{


	if (Sample::s_selectedObject)
	{
		ImGui::Begin("Object Details");

		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();

		// 창 안에 마우스가 있는지 검사
		
		Sample::s_isMouseInImGuiWindow = ImGui::IsMouseHoveringRect(windowPos, ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y));

	


		char buffer[256] = "";  // 문자열을 저장할 버퍼

		// 원래 가지고 있던 이름 표시
		ImGui::Text("Name: %s", Sample::s_selectedObject->GetName().c_str());
		ImGui::Text("Change Name: ");
		ImGui::SameLine(0, -1);
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputText("##ObjectName", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			// 엔터 키가 눌렸을 때 실행되는 코드
			// buffer에 사용자의 입력이 저장됨
			// 변경된 값을 사용하여 원하는 동작을 수행하세요
			std::string userInput = buffer;
			Sample::s_selectedObject->SetName(userInput.c_str());  // SetName 함수에 사용자의 입력을 전달
		}
		ImGui::Separator();

		ImGui::Text("Position");

		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		ImGui::InputFloat("##PositionX", &Sample::s_selectedObject->m_vPosition.x);

		ImGui::SameLine();
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		ImGui::InputFloat("##PositionY", &Sample::s_selectedObject->m_vPosition.y);

		ImGui::SameLine();
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		ImGui::InputFloat("##PositionZ", &Sample::s_selectedObject->m_vPosition.z);

		ImGui::Separator();

		ImGui::Text("Scale");

		ImGui::Text("Width");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		ImGui::InputFloat("##ScaleX", &Sample::s_selectedObject->m_vScale.x);

		ImGui::SameLine();
		ImGui::Text("Height");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		ImGui::InputFloat("##ScaleY", &Sample::s_selectedObject->m_vScale.y);

		ImGui::Separator();

		//ImGui::Text();
		if (ImGui::Button("Load Texture From File"))
		{
			ImGuiFileDialog::Instance()->OpenDialog("ChooseTexture", "Choose Texture File", ".png", ".");
			_isDialogWindow = true;
		}



		ImGui::End();

		if (ImGuiFileDialog::Instance()->Display("ChooseTexture"))
		{
	
			
			// action if OK
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
				std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
				// action
				std::wstring str = wstring().assign(filePathName.begin(), filePathName.end());

				_filePathName = str;




				HRESULT hr = CreateWICTextureFromFile(LGlobal::g_pDevice.Get(), LGlobal::g_pImmediateContext.Get(), _filePathName.c_str(), nullptr, &my_texture);
				_isTexAcceptWindow = true;
				if (FAILED(hr))
				{
					MessageBoxA(NULL, "CreateWICTextureFromFile Error", "Error Box", MB_OK);
				}

			}

			// close
			_isDialogWindow = false;
			ImGuiFileDialog::Instance()->Close();

		}

		if (my_texture && _isTexAcceptWindow)
		{
			ImGui::Image((void*)my_texture, ImVec2(256, 256));
			if (ImGui::Button("Accept"))
			{
				Sample::s_selectedObject->GetScript<ChangeTexture>(L"ChangeTexture")->ChangeFromPath(_filePathName);
				_isTexAcceptWindow = false;
			}
			if (ImGui::Button("Cancle"))
			{
				_isTexAcceptWindow = false;

				//창닫기
			}
		}
	}
	

}

void Imgui_ObjectDetail::Render()
{
}

void Imgui_ObjectDetail::Test()
{
}
