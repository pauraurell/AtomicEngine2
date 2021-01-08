#pragma once
#include "Globals.h"
#include "Component.h"
#include <vector>;

class AudioObject;

class AudioListener : public Component {
public:
	AudioListener(GameObject* gameObject);
	virtual ~AudioListener();

	void Save(GnJSONArray& save_array) override;
	void Load(GnJSONObj& load_object) override;

	virtual void Update() override;
	virtual void OnEditor() override;

private:
	AudioObject* listener = nullptr;
};
