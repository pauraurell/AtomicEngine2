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

	float3 position;
	position.x = gameObject->GetTransform()->_position.x;
	position.y = gameObject->GetTransform()->_position.y;
	position.z = gameObject->GetTransform()->_position.z;

	source = source->CreateAudioListener(gameObject->UUID, gameObject->name.c_str(), position);
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
	Camera* cam = App->camera->GetCamera();
	float3 pos = cam->GetFrustum().pos;
	float3 front = cam->GetFrustum().front;
	float3 top = cam->GetFrustum().up;

	source->SetPos(pos, front, top);
}

void AudioListener::OnEditor()
{
	if (ImGui::CollapsingHeader("Audio Listener", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
}