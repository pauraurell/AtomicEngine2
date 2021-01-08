#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "AK/Wwise_IDs.h"
#include "GameObject.h"
#include "Transform.h"

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
	for (int i = 0; i < audio_objects.size(); i++)
	{
		//audio_objects[i]->DeleteObject();
	}
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

AudioObject* ModuleAudio::CreateSource(GameObject* go)
{
	AudioObject* new_sourceObj = new AudioObject(go->UUID, go->GetName());
	new_sourceObj->SetPos(go->GetTransform()->GetPosition(), { 1,0,0 }, { 0,1,0 });

	return new_sourceObj;
}

AudioObject* ModuleAudio::CreateListener(GameObject* go)
{
	AudioObject* new_listenerObj = new AudioObject(go->UUID, go->GetName());

	AkGameObjectID listenerID = new_listenerObj->GetId();
	AK::SoundEngine::SetDefaultListeners(&listenerID, 1);
	new_listenerObj->SetPos(go->GetTransform()->GetPosition(), { 1,0,0 }, { 0,1,0 });
	App->audio->ListenerObjectId = listenerID;
	return new_listenerObj;
}

void ModuleAudio::PlayEvent(uint id, AudioObject* obj)
{
	AK::SoundEngine::PostEvent(id, obj->ObjectId);
}

void ModuleAudio::PauseEvent(uint id, AudioObject* obj)
{
	AK::SoundEngine::ExecuteActionOnEvent(id, AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Pause, obj->ObjectId);
}

void ModuleAudio::ResumeEvent(uint id, AudioObject* obj)
{
	AK::SoundEngine::ExecuteActionOnEvent(id, AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Resume, obj->ObjectId);
}

void ModuleAudio::StopEvent(uint id, AudioObject* obj)
{
	AK::SoundEngine::ExecuteActionOnEvent(id, AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Stop, obj->ObjectId);
}