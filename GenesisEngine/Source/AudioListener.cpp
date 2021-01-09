#include "AudioListener.h"
#include "Application.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "GnJSON.h"
#include "Transform.h"
#include "Camera.h"

#include "glew/include/glew.h"
#include "ImGui/imgui.h"

AudioListener::AudioListener(GameObject* gameObject) : Component(gameObject)
{
	type = ComponentType::AUDIO_LISTENER;

	listener = App->audio->CreateListener(gameObject);
	LOG("Audio Listener Component created for %s", _gameObject->GetName())
}

AudioListener::~AudioListener()
{
	listener->DeleteObject();
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
	float3 position = App->camera->GetCamera()->GetFrustum().pos;
	position.x = -position.x;
	position.z = -position.z;
	float3 front = App->camera->GetCamera()->GetFrustum().front;
	float3 up = App->camera->GetCamera()->GetFrustum().up;
	listener->SetPos(position, front, up);
}

void AudioListener::OnEditor()
{
	if (ImGui::CollapsingHeader("Audio Listener", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
}