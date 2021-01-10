#pragma once

#include "Module.h"
#include "Globals.h"
#include "ModuleWwise.h"
#include "MathGeoLib/include/Math/float3.h"
#include "AudioObject.h"
#include <vector>

using namespace std;
class GameObject;
class AudioEmitter;

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

	void PlayEvent(uint id, AudioObject* obj);
	void PauseEvent(uint id, AudioObject* obj);
	void ResumeEvent(uint id, AudioObject* obj);
	void StopEvent(uint id, AudioObject* obj);
	void PauseEvents();
	void ResumeEvents();

	void LoadBank(const char* sound_bank);

	AudioObject* CreateSource(GameObject* go);
	AudioObject* CreateListener(GameObject* go);

	AkGameObjectID ListenerObjectId;
	vector<AudioEmitter*> audio_emitters;
};
