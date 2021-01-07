#pragma once
#include "Globals.h"
#include "Component.h"
#include <vector>;
#include "ModuleAudio.h"

class AudioEmitter : public Component {
public:
	AudioEmitter(GameObject* gameObject);
	virtual ~AudioEmitter();

	void Save(GnJSONArray& save_array) override;
	void Load(GnJSONObj& load_object) override;

	virtual void Update() override;
	virtual void OnEditor() override;

	AudioSource* audio_source;

	bool mute;
	bool bypass_reverb_zones;
	bool play_on_awake;
	bool loop;
	int* priority;
	float* volume;
	float* pitch;
};
