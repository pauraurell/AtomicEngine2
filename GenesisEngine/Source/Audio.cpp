#include "Audio.h"
#include "Application.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "GnJSON.h"

#include "ResourceMesh.h"

#include "glew/include/glew.h"
#include "ImGui/imgui.h"

// GnMesh =========================================================================================================================

Audio::Audio(GameObject* gameObject) : Component(gameObject)
{
	type = ComponentType::MESH;
	volume = new float(50.0f);
}

Audio::~Audio()
{
	
}

void Audio::Save(GnJSONArray& save_array)
{
	GnJSONObj save_object;

	save_object.AddInt("Type", type);

	save_array.AddObject(save_object);
}

void Audio::Load(GnJSONObj& load_object)
{

}

void Audio::Update()
{
	
}

void Audio::OnEditor()
{
	if (ImGui::CollapsingHeader("Audio", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Enabled", &enabled);

		ImGui::Separator();
		ImGui::Spacing();
		
		ImGui::DragFloat("Volume", volume, 1, 0, 100);
	}
}