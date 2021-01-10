#include "AudioEmitter.h"
#include "Application.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "GnJSON.h"
#include "Transform.h"
#include "ModuleAudio.h"

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
	/*float3 Position = _gameObject->GetTransform()->GetPosition();
	emitter = App->audio->CreateSoundObj(gameObject->UUID, gameObject->name.c_str(), Position.x, Position.y, Position.z);*/
	App->audio->audio_emitters.push_back(this);
	LOG("Audio Emitter Component created for %s", _gameObject->GetName())
}

AudioEmitter::~AudioEmitter()
{
	for (int i = 0; i < App->audio->audio_emitters.size(); i++)
	{
		if (App->audio->audio_emitters[i]->name == this->name)
		{
			App->audio->audio_emitters.erase(App->audio->audio_emitters.begin() + i);
		}
	}
	emitter->DeleteObject();
}

void AudioEmitter::Save(GnJSONArray& save_array)
{
	GnJSONObj save_object;
	save_object.AddInt("Type", type);
	save_object.AddInt("priority", *priority);
	save_object.AddFloat("volume", *volume);
	save_object.AddInt("pitch", *pitch);
	save_object.AddBool("bypass_reverb_zones", bypass_reverb_zones);
	save_object.AddBool("play_on_awake", play_on_awake);
	save_object.AddBool("loop", loop);
	save_object.AddBool("mute", mute);
	save_array.AddObject(save_object);
}

void AudioEmitter::Load(GnJSONObj& load_object)
{
	*priority = load_object.GetInt("priority");
	*volume = load_object.GetFloat("volume");
	*pitch = load_object.GetInt("pitch");
	bypass_reverb_zones = load_object.GetBool("bypass_reverb_zones");
	play_on_awake = load_object.GetBool("play_on_awake");
	loop = load_object.GetBool("loop");
	mute = load_object.GetBool("mute");
}

void AudioEmitter::Update()
{
	float3 Position = _gameObject->GetTransform()->GetPosition();
	Position.x = -Position.x;
	Position.z = -Position.z;
	emitter->SetPos(Position, { 1,0,0 }, {0,1,0});
	//_gameObject->GetTransform()->SetPosition(Position.x, Position.y, Position.z);

	if (mute)
	{
		AK::SoundEngine::SetGameObjectOutputBusVolume(emitter->ObjectId, AK_INVALID_GAME_OBJECT, 0);
	}
	else
	{
		AK::SoundEngine::SetGameObjectOutputBusVolume(emitter->ObjectId, AK_INVALID_GAME_OBJECT, emitter->volume);
	}
}

void AudioEmitter::OnEditor()
{
	if (ImGui::CollapsingHeader("Audio Emitter", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Enabled", &enabled);

		ImGui::Separator();
		ImGui::Spacing();
		
		if(ImGui::Checkbox("Mute", &mute))

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