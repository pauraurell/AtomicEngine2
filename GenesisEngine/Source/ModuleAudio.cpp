#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

ModuleAudio::ModuleAudio(bool start_enabled) : Module(start_enabled){}

ModuleAudio::~ModuleAudio()
{}

bool ModuleAudio::Start()
{
	ModuleWwise::InitSoundEngine();
	LoadBank("AtomicSoundBank.bnk");
	return true;
}

update_status ModuleAudio::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleAudio::Update(float dt)
{
	ModuleWwise::ProcessAudio();
	return UPDATE_CONTINUE;
}

update_status ModuleAudio::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleAudio::CleanUp()
{
	ModuleWwise::TermSoundEngine();
	return true;
}

void ModuleAudio::LoadBank(const char* sound_bank)
{
	AkBankID id = 0;
	AKRESULT ResultVal = AK::SoundEngine::LoadBank(sound_bank, AK_DEFAULT_POOL_ID, id);

	if (ResultVal != AK_Success) {
		LOG("Couldn't load %s", sound_bank); 
	}
	else { 
		LOG("%s loaded correctly", sound_bank); 
	}
}

AudioSource::AudioSource(unsigned __int64 id, const char* name)
{
    this->id = id;
    this->name = name;
    AK::SoundEngine::RegisterGameObj(this->id, this->name);
}

AudioSource::~AudioSource()
{
    AK::SoundEngine::UnregisterGameObj(id);
}

void AudioSource::SetPos(float3 pos, float3 rotF, float3 rotT)
{
    position.X = pos.x; position.Y = pos.y; position.Z = pos.z;
	orientationFront.X = rotF.x; orientationFront.Y = rotF.y; orientationFront.Z = rotF.z;
	orientationTop.X = rotT.x; orientationTop.Y = rotT.y; orientationTop.Z = rotT.z;
    
    AkSoundPosition source_position;
	source_position.Set(position, orientationFront, orientationTop);
    AK::SoundEngine::SetPosition(id, source_position);
}

void AudioSource::PlayEvent(uint id)
{
	AK::SoundEngine::PostEvent(id, this->id);
}

void AudioSource::PauseEvent(uint id)
{
	AK::SoundEngine::ExecuteActionOnEvent(id, AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Pause, this->id);
}

void AudioSource::ResumeEvent(uint id)
{
	AK::SoundEngine::ExecuteActionOnEvent(id, AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Resume, this->id);
}

void AudioSource::StopEvent(uint id)
{
	AK::SoundEngine::ExecuteActionOnEvent(id, AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Stop, this->id);
}

void AudioSource::SetVolume(uint id, float volume)
{
	AK::SoundEngine::SetGameObjectOutputBusVolume(this->id, AK_INVALID_GAME_OBJECT, volume);
	this->volume = volume;
}

AudioSource* AudioSource::CreateAudioSource(uint id, const char* name, float3 position)
{
	AudioSource* go = new AudioSource(id, name);
	go->SetPos(position, { 1,0,0 }, { 0,1,0 });

	return go;
}

AudioSource* AudioSource::CreateAudioListener(uint id, const char* name, float3 position)
{
	AudioSource* go = new AudioSource(id, name);

	AkGameObjectID listenerID = go->GetId();
	AK::SoundEngine::SetDefaultListeners(&listenerID, 1);
	go->SetPos(position, { 1,0,0 }, { 0,1,0 });
	App->audio->ListenerId = listenerID;
	return go;
}

uint AudioSource::GetId()
{
	return id;
}
