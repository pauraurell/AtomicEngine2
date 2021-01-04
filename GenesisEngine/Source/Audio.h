#pragma once
#include "Globals.h"
#include "Component.h"
#include <vector>;

#include "MathGeoLib/include/MathGeoLib.h"

class ResourceMesh;

typedef float GLfloat;
typedef unsigned short GLushort;
typedef unsigned char GLubyte;

class Audio : public Component {
public:
	Audio(GameObject* gameObject);
	virtual ~Audio();

	void Save(GnJSONArray& save_array) override;
	void Load(GnJSONObj& load_object) override;

	virtual void Update() override;
	virtual void OnEditor() override;

	float* volume;
};
