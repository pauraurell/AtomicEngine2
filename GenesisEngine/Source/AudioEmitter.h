#pragma once
#include "Globals.h"
#include "Component.h"
#include <vector>;

class AudioEmitter : public Component {
public:
	AudioEmitter(GameObject* gameObject);
	virtual ~AudioEmitter();

	void Save(GnJSONArray& save_array) override;
	void Load(GnJSONObj& load_object) override;

	virtual void Update() override;
	virtual void OnEditor() override;

	float* volume;
};
