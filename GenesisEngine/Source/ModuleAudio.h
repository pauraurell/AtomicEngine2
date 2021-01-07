#pragma once

#include "Module.h"
#include "Globals.h"
#include "ModuleWwise.h"
#include "MathGeoLib/include/Math/float3.h"
#include <AK/SoundEngine/Common/AkTypes.h>

using namespace std;

class ModuleAudio : public Module
{
public:
	ModuleAudio(bool start_enabled = true);
	~ModuleAudio();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void LoadBank(const char* sound_bank);

	AkGameObjectID currentListenerID;
};

class AudioSource {
	AudioSource(unsigned __int64 id, const char* name);
	~AudioSource();

public:

	void PlayEvent(uint id);
	void PauseEvent(uint id);
	void ResumeEvent(uint id);
	void StopEvent(uint id);

	void SetPos(float3 pos, float3 rotF, float3 rotT);
	void SetVolume(uint id, float volume);
	float volume = 1.0f;
	
	AudioSource* CreateAudioSource(uint id, const char* name, float3 position);
	AudioSource* CreateAudioListener(uint id, const char* name, float3 position);

	uint GetId();

private:
	const char* name = nullptr;
	AkGameObjectID id;

	AkVector position = { 0,0,0 };
	AkVector orientationFront = { 0,0,0 };
	AkVector orientationTop = { 0,0,0 };
};
