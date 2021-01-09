#pragma once
#include <AK/SoundEngine/Common/AkTypes.h>
#include "ModuleWwise.h"
#include "MathGeoLib/include/Math/float3.h"

class AudioObject
{
public:
	AudioObject(unsigned __int64 id, const char* name);
	~AudioObject();

public:
	void SetPos(float3 pos, float3 rotF, float3 rotT);
	void SetVolume(uint id, float volume);

	void DeleteObject();
	uint GetId();

	AkVector position, front, top = { 0,0,0 };
	float volume = 1.0f;
	AkGameObjectID ObjectId;

	const char* name = nullptr;
};

