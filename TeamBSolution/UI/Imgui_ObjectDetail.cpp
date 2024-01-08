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

		// â �ȿ� ���콺�� �ִ��� �˻�
		
		Sample::s_isMouseInImGuiWindow = ImGui::IsMouseHoveringRect(windowPos, ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y));

	


		char buffer[256] = "";  // ���ڿ��� ������ ����

		// ���� ������ �ִ� �̸� ǥ��
		ImGui::Text("Name: %s", Sample::s_selectedObject->GetName().c_str());
		ImGui::Text("Change Name: ");
		ImGui::SameLine(0, -1);
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputText("##ObjectName", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			// ���� Ű�� ������ �� ����Ǵ� �ڵ�
			// buffer�� ������� �Է��� �����
			// ����� ���� ����Ͽ� ���ϴ� ������ �����ϼ���
			std::string userInput = buffer;
			Sample::s_selectedObject->SetName(userInput.c_str());  // SetName �Լ��� ������� �Է��� ����
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

				//â�ݱ�
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
