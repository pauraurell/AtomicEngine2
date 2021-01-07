#pragma once
#include "Globals.h"
#include "Component.h"
#include <vector>;
#include "MathGeoLib\include\MathGeoLib.h"

class AudioReverbZone : public Component {
public:
	AudioReverbZone(GameObject* gameObject);
	virtual ~AudioReverbZone();

	void Save(GnJSONArray& save_array) override;
	void Load(GnJSONObj& load_object) override;

	virtual void Update() override;
	virtual void OnEditor() override;

	float r;
	math::Sphere sphere;
};
