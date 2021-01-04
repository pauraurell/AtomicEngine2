#include "AudioReverbZone.h"
#include "Application.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "GnJSON.h"

#include "glew/include/glew.h"
#include "ImGui/imgui.h"

AudioReverbZone::AudioReverbZone(GameObject* gameObject) : Component(gameObject)
{
	type = ComponentType::AUDIO_REVERB_ZONE;
	min_distance = 10;
	max_distance = 15;
}

AudioReverbZone::~AudioReverbZone()
{

}

void AudioReverbZone::Save(GnJSONArray& save_array)
{
	GnJSONObj save_object;
	save_object.AddInt("Type", type);
	save_array.AddObject(save_object);
}

void AudioReverbZone::Load(GnJSONObj& load_object)
{

}

void AudioReverbZone::Update()
{

}

void AudioReverbZone::OnEditor()
{
	if (ImGui::CollapsingHeader("Audio Reverb Zone", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
}