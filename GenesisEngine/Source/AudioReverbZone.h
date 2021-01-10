#pragma once
#include "Globals.h"
#include "Component.h"
#include "AudioEmitter.h"
#include <vector>;
#include "MathGeoLib\include\MathGeoLib.h"

#define CIRCUMFERENCE_VERTEX 20.0f //Num of vertices that will have each line loop of the sphere

class AudioReverbZone : public Component {
public:
	AudioReverbZone(GameObject* gameObject);
	virtual ~AudioReverbZone();

	void Save(GnJSONArray& save_array) override;
	void Load(GnJSONObj& load_object) override;

	virtual void Update() override;
	virtual void OnEditor() override;

	bool ContainsSource();
	float CalculateReverbValue(float3 emitterPos);
	void Reverb(float d, AudioEmitter* emitter);

	float r;
	math::Sphere reverb_sphere;
};
