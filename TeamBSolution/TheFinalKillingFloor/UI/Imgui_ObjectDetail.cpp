#include "Imgui_ObjectDetail.h"
#include "KObject.h"
#include "PickingUI.h"
#include "ChangeTexture.h"
#include "LGlobal.h"

#include <WICTextureLoader.h>
#pragma comment(lib, "DirectXTK_D.lib")
#include "UIManager.h"
bool Imgui_ObjectDetail::_isDialogWindow = false;
void Imgui_ObjectDetail::Init()
{
}

void Imgui_ObjectDetail::Frame()
{


	if (UIManager::s_selectedObject)
	{
		ImGui::Begin("Object Details");

		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();

		// 창 안에 마우스가 있는지 검사

		UIManager::s_isMouseInImGuiWindow = ImGui::IsMouseHoveringRect(windowPos, ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y));




		char buffer[256] = "";  // 문자열을 저장할 버퍼

		// 원래 가지고 있던 이름 표시

		ImGui::Text("Name: %s", wtm(UIManager::s_selectedObject->GetName()).c_str());
		ImGui::Text("Change Name: ");
		ImGui::SameLine(0, -1);
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputText("##ObjectName", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			// 엔터 키가 눌렸을 때 실행되는 코드
			// buffer에 사용자의 입력이 저장됨
			// 변경된 값을 사용하여 원하는 동작을 수행하세요

			UIManager::s_selectedObject->SetName(mtw(buffer));  // SetName 함수에 사용자의 입력을 전달
		}
		ImGui::Separator();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f)); // 버튼의 색상 설정
		if (ImGui::Button("Remove Object"))
		{
			UIManager::GetInstance().RemoveObject(UIManager::s_selectedObject->GetName());
			UIManager::s_selectedObject = nullptr;
		}
		ImGui::PopStyleColor();
		if (UIManager::s_selectedObject)
		{
			ImGui::Text("Position");
			ImGui::Text("X");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100);
			ImGui::InputFloat("##PositionX", &UIManager::s_selectedObject->m_vPosition.x);

			ImGui::SameLine();
			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100);
			ImGui::InputFloat("##PositionY", &UIManager::s_selectedObject->m_vPosition.y);

			ImGui::SameLine();
			ImGui::Text("Z");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100);
			ImGui::InputFloat("##PositionZ", &UIManager::s_selectedObject->m_vPosition.z);

			ImGui::Separator();

			ImGui::Text("Scale");

			ImGui::Text("Width");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100);
			ImGui::InputFloat("##ScaleX", &UIManager::s_selectedObject->m_vScale.x);

			ImGui::SameLine();
			ImGui::Text("Height");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100);
			ImGui::InputFloat("##ScaleY", &UIManager::s_selectedObject->m_vScale.y);

			ImGui::Separator();

			ImGui::Text("Image Load : ");
			ImGui::SameLine();
			if (ImGui::Button("Load Texture From File"))
			{
				ImGuiFileDialog::Instance()->OpenDialog("ChooseTexture", "Choose Texture File", ".png", ".");
				_isDialogWindow = true;
			}
			//script
			if (ImGui::TreeNode("Scripts"))
			{

				const char* items[] = { "DragUI", "PickingUI", "Resize2D", "Animator", "DigitDisplay", "ExitWindow","BillBoard","ButtonAction","Text", "SceneChange" };
				static int item_current_idx = 0; // Here we store our selection data as an index.
				if (ImGui::BeginListBox(""))
				{
					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						const bool is_selected = (item_current_idx == n);
						if (ImGui::Selectable(items[n], is_selected))
							item_current_idx = n;

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();

					}
					ImGui::EndListBox();
					if (ImGui::Button("Add"))
					{
						switch (item_current_idx)
						{
						case 0:
							UIManager::s_selectedObject->AddScripts(make_shared<DragUI>());
							break;
						case 1:
							UIManager::s_selectedObject->AddScripts(make_shared<PickingUI>());
							break;
						case 2:
							UIManager::s_selectedObject->AddScripts(make_shared<Resize2D>());
							UIManager::s_selectedObject->GetScript< Resize2D>(L"Resize2D")->Init();
							break;
						case 3:
							UIManager::s_selectedObject->AddScripts(make_shared<Animator>(L"../../res/ui/xml/Anim.xml"));
							break;
						case 4:
							UIManager::s_selectedObject->AddScripts(make_shared<DigitDisplay>(3, L"../../res/ui/xml/testNum.xml"));
							UIManager::s_selectedObject->GetScript< DigitDisplay>(L"DigitDisplay")->Init();
							break;
						case 5:
							UIManager::s_selectedObject->AddScripts(make_shared<ExitWindow>());
							break;
						case 6:
							UIManager::s_selectedObject->AddScripts(make_shared<BillBoard>());
							break;
						case 7:
							UIManager::s_selectedObject->AddScripts(make_shared<ButtonAction>(L"../../res/ui/xml/ButtonTex.xml"));
							break;
						case 8:
							UIManager::s_selectedObject->AddScripts(make_shared<Text>(L"NULL"));
							break;
						case 9:
							UIManager::s_selectedObject->AddScripts(make_shared<SceneChange>());
							break;
						default:
							break;
						}
					}
				}





				ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;

				if (ImGui::BeginTabBar("ScriptTabBar", tab_bar_flags))
				{
					for (auto script : UIManager::s_selectedObject->GetScripts())
					{
						if (script->GetName() == L"PickingUI")
						{
							if (ImGui::BeginTabItem("PickingUI"))
							{
								ImGui::Text("Object mouse picking");
								if (ImGui::Button("Delete"))
								{
									UIManager::s_selectedObject->RemoveScript(L"PickingUI");
									ImGui::EndTabItem();
									continue;

								}
								ImGui::EndTabItem();
							}
						}
						if (script->GetName() == L"DragUI")
						{
							if (ImGui::BeginTabItem("DragUI"))
							{
								ImGui::Text("Object Drag");

								if (ImGui::Button("Delete"))
								{
									UIManager::s_selectedObject->RemoveScript(L"DragUI");
									ImGui::EndTabItem();
									continue;

								}
								ImGui::EndTabItem();
							}
						}
						if (script->GetName() == L"Resize2D")
						{
							if (ImGui::BeginTabItem("Resize2D"))
							{
								ImGui::Text("Resize object rect");

								if (ImGui::Button("Delete"))
								{
									UIManager::s_selectedObject->RemoveScript(L"Resize2D");
									ImGui::EndTabItem();
									continue;

								}
								ImGui::EndTabItem();
							}
						}
						if (script->GetName() == L"BillBoard")
						{
							if (ImGui::BeginTabItem("BillBoard"))
							{
								ImGui::Text("BillBoard");

								if (ImGui::Button("Delete"))
								{
									UIManager::s_selectedObject->RemoveScript(L"BillBoard");
									ImGui::EndTabItem();
									continue;

								}
								ImGui::EndTabItem();
							}
						}
						if (script->GetName() == L"DigitDisplay")
						{
							if (ImGui::BeginTabItem("DigitDisplay"))
							{
								ImGui::Text("Number to textureImage");

								if (ImGui::Button("Delete"))
								{
									UIManager::s_selectedObject->RemoveScript(L"DigitDisplay");
									ImGui::EndTabItem();
									continue;
								}
								ImGui::SameLine();
								if (ImGui::Button("Load DigitTexture From File"))
								{
									ImGuiFileDialog::Instance()->OpenDialog("ChooseDigitTexture", "ChooseDigitTexture File", ".xml", ".");
									_isDialogWindow = true;
								}
								ImGui::EndTabItem();
							}
						}
						if (script->GetName() == L"Animator")
						{
							if (ImGui::BeginTabItem("Animator"))
							{
								ImGui::Text("Animation");
								if (ImGui::Button("Delete"))
								{
									UIManager::s_selectedObject->RemoveScript(L"Animator");
									ImGui::EndTabItem();
									continue;

								}
								ImGui::SameLine();
								if (ImGui::Button("Load Animation From File"))
								{
									ImGuiFileDialog::Instance()->OpenDialog("ChooseAnimation", "ChooseAnimation File", ".xml", ".");
									_isDialogWindow = true;
								}

								ImGui::EndTabItem();
							}
						}
						if (script->GetName() == L"ExitWindow")
						{
							if (ImGui::BeginTabItem("ExitWindow"))
							{
								ImGui::Text("object click exit Window");
								if (ImGui::Button("Delete"))
								{
									UIManager::s_selectedObject->RemoveScript(L"ExitWindow");
									ImGui::EndTabItem();
									continue;
								}
								ImGui::EndTabItem();
							}
						}
						if (script->GetName() == L"ButtonAction")
						{
							if (ImGui::BeginTabItem("ButtonAction"))
							{
								ImGui::Text("ButtonStateAction");
								if (ImGui::Button("Delete"))
								{
									UIManager::s_selectedObject->RemoveScript(L"ButtonAction");
									ImGui::EndTabItem();
									continue;
								}
								ImGui::SameLine();
								if (ImGui::Button("Load Animation From File"))
								{
									ImGuiFileDialog::Instance()->OpenDialog("ChooseButtonTex", "ChooseButtonTex File", ".xml", ".");
									_isDialogWindow = true;
								}


								ImGui::EndTabItem();
							}
						}
						if (script->GetName() == L"Text")
						{
							if (ImGui::BeginTabItem("Text"))
							{
								ImGui::Text("object click exit Window");
								if (ImGui::Button("Delete"))
								{
									UIManager::s_selectedObject->RemoveScript(L"Text");
									ImGui::EndTabItem();
									continue;
								}
								if (ImGui::InputText("##Text", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
								{
									UIManager::s_selectedObject->GetScript<Text>(L"Text")->SetText(mtw(buffer));
								}
								ImGui::EndTabItem();
							}
						}
						if (script->GetName() == L"SceneChange")
						{
							if (ImGui::BeginTabItem("SceneChange"))
							{
								ImGui::Text("Go to Main = 5, ,Ingame = 6, End = 7");
								if (ImGui::Button("Delete"))
								{
									UIManager::s_selectedObject->RemoveScript(L"SceneChange");
									ImGui::EndTabItem();
									continue;
								};
								ImGui::Text("Current: %d", static_cast<int>(UIManager::s_selectedObject->GetScript<SceneChange>(L"SceneChange")->GetEvent()));
								if (ImGui::InputText("##SceneChange", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
								{

									UIManager::s_selectedObject->GetScript<SceneChange>(L"SceneChange")->SetEvent(static_cast<Event>(atoi(buffer)));
								}
								ImGui::EndTabItem();
							}
						}
					}

					ImGui::EndTabBar();
				}
				ImGui::Separator();
				ImGui::TreePop();
			}
		}
		// /script

		ImGui::End();


		//이미지 변경
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
			// 애니메이션 변경
			if (ImGuiFileDialog::Instance()->Display("ChooseAnimation"))
			{

				// action if OK
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
					std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
					// action



					UIManager::s_selectedObject->GetScript<Animator>(L"Animator")->SetAnimationFromXml(mtw(filePathName));
					_isTexAcceptWindow = true;

				}

				// close
				_isDialogWindow = false;
				ImGuiFileDialog::Instance()->Close();

			}
			// ButtonAction
			if (ImGuiFileDialog::Instance()->Display("ChooseButtonTex"))
			{

				// action if OK
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
					std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
					// action



					UIManager::s_selectedObject->GetScript<ButtonAction>(L"ButtonAction")->LoadTextureList(mtw(filePathName));
					_isTexAcceptWindow = true;

				}

				// close
				_isDialogWindow = false;
				ImGuiFileDialog::Instance()->Close();

			}
			//Digit
			if (ImGuiFileDialog::Instance()->Display("ChooseDigitTexture"))
			{

				// action if OK
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
					std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
					// action



					UIManager::s_selectedObject->GetScript<DigitDisplay>(L"DigitDisplay")->LoadTextureList(mtw(filePathName));
					_isTexAcceptWindow = true;

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
					UIManager::s_selectedObject->GetScript<ChangeTexture>(L"ChangeTexture")->ChangeFromPath(_filePathName);
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
