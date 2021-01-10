#include "AudioObject.h"
#include "Application.h"
#include "ModuleAudio.h"

AudioObject::AudioObject(unsigned __int64 id, const char* name)
{
	this->ObjectId = id;
	this->name = name;
	AK::SoundEngine::RegisterGameObj(this->ObjectId, this->name);
}

AudioObject::~AudioObject()
{
	AK::SoundEngine::UnregisterGameObj(ObjectId);
}

void AudioObject::SetPos(float3 pos, float3 rotF, float3 rotT)
{
	position.X = pos.x; position.Y = pos.y; position.Z = pos.z;
	front.X = rotF.x; front.Y = rotF.y; front.Z = rotF.z;
	top.X = rotT.x; top.Y = rotT.y; top.Z = rotT.z;

	AkSoundPosition sound_pos;
	sound_pos.Set(position, front, top);
	AK::SoundEngine::SetPosition(ObjectId, sound_pos);
}

void AudioObject::SetVolume(uint id, float volume)
{
	AK::SoundEngine::SetGameObjectOutputBusVolume(this->ObjectId, AK_INVALID_GAME_OBJECT, volume);
	this->volume = volume;
}

uint AudioObject::GetId()
{
	return ObjectId;
}

void AudioObject::DeleteObject()
{
	AK::SoundEngine::UnregisterGameObj(ObjectId);
	delete this;
}

