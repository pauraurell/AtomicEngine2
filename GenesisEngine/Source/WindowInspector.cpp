#include "WindowInspector.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"

WindowInspector::WindowInspector() : EditorWindow()
{
	type = WindowType::INSPECTOR_WINDOW;
}

WindowInspector::~WindowInspector()
{
}

void WindowInspector::Draw()
{
	if (ImGui::Begin("Inspector", &visible))
	{
		if (App->scene->selectedGameObject != nullptr)
		{
			App->scene->selectedGameObject->OnEditor();
		}

		if (ImGui::CollapsingHeader("Resources")) {
			App->resources->OnEditor();
		}

		ImGui::Separator();
		if (ImGui::Button("Add Component..."))
		{
			ImGui::OpenPopup("AddPopUp");
		}
		if (ImGui::BeginPopup("AddPopUp"))
		{
			if (ImGui::MenuItem("Material..."))
			{
				if (App->scene->selectedGameObject->GetComponent(ComponentType::MATERIAL) == nullptr)
				{
					App->scene->selectedGameObject->AddComponent(ComponentType::MATERIAL);
				}
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Audio Emitter..."))
			{
				if (App->scene->selectedGameObject->GetComponent(ComponentType::AUDIO_EMITTER) == nullptr)
				{
					App->scene->selectedGameObject->AddComponent(ComponentType::AUDIO_EMITTER);
				}
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Audio Listener..."))
			{
				if (App->scene->selectedGameObject->GetComponent(ComponentType::AUDIO_LISTENER) == nullptr)
				{
					App->scene->selectedGameObject->AddComponent(ComponentType::AUDIO_LISTENER);
				}
				ImGui::CloseCurrentPopup();
			}
			/*else
			{
				if (ImGui::IsMouseClicked(0))
				{
					atLOG("This GameObject already has a Component Material!");
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Material...", 0, false, false))
				{
					ImGui::CloseCurrentPopup();
				}
			}*/
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}
