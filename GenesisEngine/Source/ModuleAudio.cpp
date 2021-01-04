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