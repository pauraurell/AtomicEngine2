#pragma once
#ifndef MODULEAUDIO_H
#define MODULEAUDIO_H

#include "Module.h"
#include "Globals.h"
#include "ModuleWwise.h"

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

	void Play();
	void Stop();
	void Pause();
	void Resume();

	void LoadBank(const char* sound_bank);
};

#endif