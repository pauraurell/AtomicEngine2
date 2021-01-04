#include "AudioEmitter.h"
#include "Application.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "GnJSON.h"

#include "glew/include/glew.h"
#include "ImGui/imgui.h"

AudioEmitter::AudioEmitter(GameObject* gameObject) : Component(gameObject)
{
	type = ComponentType::AUDIO_EMITTER;
	volume = new float(50.0f);
}

AudioEmitter::~AudioEmitter()
{
	
}

void AudioEmitter::Save(GnJSONArray& save_array)
{
	GnJSONObj save_object;
	save_object.AddInt("Type", type);
	save_array.AddObject(save_object);
}

void AudioEmitter::Load(GnJSONObj& load_object)
{

}

void AudioEmitter::Update()
{
	
}

void AudioEmitter::OnEditor()
{
	if (ImGui::CollapsingHeader("Audio Emitter", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Enabled", &enabled);

		ImGui::Separator();
		ImGui::Spacing();
		
		ImGui::DragFloat("Volume", volume, 1, 0, 100);
	}
}