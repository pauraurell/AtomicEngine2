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
	volume = new float(1.0f);
	mute = false;
	bypass_reverb_zones = false;
	play_on_awake = false;
	loop = false;
	priority = new int(128);
	pitch = new float(0.0f);
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
		
		ImGui::Checkbox("Mute", &mute);
		ImGui::Checkbox("Loop", &loop);
		ImGui::Checkbox("Bypass Reverb Zones", &bypass_reverb_zones);
		ImGui::Checkbox("Play on Awake", &play_on_awake);

		ImGui::DragInt("Priority", priority, 1, 0, 256);
		ImGui::DragFloat("Volume", volume, 0.1, 0, 1);
		ImGui::DragFloat("Pitch", pitch, 0.1, -3.0f, 3.0f);
	}
}