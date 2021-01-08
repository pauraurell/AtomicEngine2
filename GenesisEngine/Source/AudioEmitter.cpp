#include "AudioEmitter.h"
#include "Application.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "GnJSON.h"
#include "Transform.h"

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

	emitter = App->audio->CreateSource(gameObject);
	LOG("Audio Emitter Component created for %s", _gameObject->GetName())
}

AudioEmitter::~AudioEmitter()
{
	emitter->DeleteObject();
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
	float3 Position = _gameObject->GetTransform()->GetPosition();
	emitter->SetPos(Position, { 1,0,0 }, {0,1,0});
}

void AudioEmitter::OnEditor()
{
	if (ImGui::CollapsingHeader("Audio Emitter", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Enabled", &enabled);

		ImGui::Separator();
		ImGui::Spacing();
		
		if(ImGui::Checkbox("Mute", &mute))
		{
			emitter->SetVolume(id, 0);
		}
		ImGui::Checkbox("Loop", &loop);
		ImGui::Checkbox("Bypass Reverb Zones", &bypass_reverb_zones);

		ImGui::DragInt("Priority", priority, 1, 0, 256);
		if (ImGui::DragFloat("Volume", &emitter->volume, 0.01, 0, 1))
		{
			emitter->SetVolume(id, emitter->volume);
		}
	}
}

void AudioEmitter::SetID(AkGameObjectID id)
{
	this->id = id;
}