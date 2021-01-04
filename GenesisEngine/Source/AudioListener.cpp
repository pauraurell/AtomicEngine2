#include "AudioListener.h"
#include "Application.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "GnJSON.h"

#include "glew/include/glew.h"
#include "ImGui/imgui.h"

AudioListener::AudioListener(GameObject* gameObject) : Component(gameObject)
{
	type = ComponentType::AUDIO_LISTENER;
}

AudioListener::~AudioListener()
{

}

void AudioListener::Save(GnJSONArray& save_array)
{
	GnJSONObj save_object;
	save_object.AddInt("Type", type);
	save_array.AddObject(save_object);
}

void AudioListener::Load(GnJSONObj& load_object)
{

}

void AudioListener::Update()
{

}

void AudioListener::OnEditor()
{
	if (ImGui::CollapsingHeader("Audio Listener", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
}