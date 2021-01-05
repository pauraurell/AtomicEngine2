#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

ModuleAudio::ModuleAudio(bool start_enabled) : Module(start_enabled){}

ModuleAudio::~ModuleAudio()
{}

bool ModuleAudio::Start()
{
	ModuleWwise::InitSoundEngine();
	return true;
}

update_status ModuleAudio::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleAudio::Update(float dt)
{
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

void ModuleAudio::Play()
{
	
}

void ModuleAudio::Stop()
{

}

void ModuleAudio::Pause()
{

}

void ModuleAudio::Resume()
{
	
}

void ModuleAudio::LoadBank(const char* sound_bank)
{
	std::string path = "../Engine/Assets/SoundBanks/";
	path += sound_bank;
	AkBankID id = 0;
	AKRESULT ResultVal = AK::SoundEngine::LoadBank(path.c_str(), AK_DEFAULT_POOL_ID, id);

	if (ResultVal != AK_Success) { LOG("Couldn't load %s", path.c_str()); }
	else { LOG("%s loaded correctly", path.c_str()); }
}