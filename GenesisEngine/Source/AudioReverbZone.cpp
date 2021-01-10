#include "AudioReverbZone.h"
#include "Application.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "GnJSON.h"
#include "Transform.h"
#include "AudioEmitter.h"
#include "GameObject.h"

#include "glew/include/glew.h"
#include "ImGui/imgui.h"

AudioReverbZone::AudioReverbZone(GameObject* gameObject) : Component(gameObject)
{
	type = ComponentType::AUDIO_REVERB_ZONE;
	r = 10.f;
}

AudioReverbZone::~AudioReverbZone()
{

}

void AudioReverbZone::Save(GnJSONArray& save_array)
{
	GnJSONObj save_object;
	save_object.AddInt("Type", type);
	save_array.AddObject(save_object);
}

void AudioReverbZone::Load(GnJSONObj& load_object)
{

}

void AudioReverbZone::Update()
{
	Transform* t = (Transform*)_gameObject->GetComponent(ComponentType::TRANSFORM);
	if(t != nullptr) reverb_sphere.pos = t->_position;
	reverb_sphere.r = r;

	glLineWidth(1.0f);
	if (ContainsSource()) { glColor3f(1.0f, 0.0f, 1.0f); }
	else { glColor3f(1.0f, 1.0f, 1.0f); }

	float angle = (360.f / (float)CIRCUMFERENCE_VERTEX);

	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < CIRCUMFERENCE_VERTEX; i++)
		glVertex3f(cos(angle * DEGTORAD * i) * reverb_sphere.r + reverb_sphere.pos.x, reverb_sphere.pos.y, sin(angle * DEGTORAD * i) * reverb_sphere.r + reverb_sphere.pos.z);
		glEnd();
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < CIRCUMFERENCE_VERTEX; i++)
		glVertex3f(cos(angle * DEGTORAD * i) * reverb_sphere.r + reverb_sphere.pos.x, sin(angle * DEGTORAD * i) * reverb_sphere.r + reverb_sphere.pos.y, reverb_sphere.pos.z);
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < CIRCUMFERENCE_VERTEX; i++)
		glVertex3f(reverb_sphere.pos.x, sin(angle * DEGTORAD * i) * reverb_sphere.r + reverb_sphere.pos.y, cos(angle * DEGTORAD * i) * reverb_sphere.r + reverb_sphere.pos.z);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
}

void AudioReverbZone::OnEditor()
{
	if (ImGui::CollapsingHeader("Audio Reverb Zone", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Radius", &r, 0.1, 0, 50);
	}
}

bool AudioReverbZone::ContainsSource()
{
	bool ret = false;

	for (int i = 0; i < App->audio->audio_emitters.size(); i++)
	{
		float reverbValue = 0;
		Transform* transform = (Transform*)App->audio->audio_emitters[i]->_gameObject->GetComponent(ComponentType::TRANSFORM);
		if (transform != nullptr) 
		{
			AudioEmitter* emitter = (AudioEmitter*)App->audio->audio_emitters[i]->_gameObject->GetComponent(ComponentType::AUDIO_EMITTER);
			if (emitter->bypass_reverb_zones == false)
			{
				float3 point = transform->GetPosition();
				//const char* name = App->audio->audio_emitters[i]->_gameObject->name.c_str();
				if (reverb_sphere.Contains(point))
				{
					ret = true;
					reverbValue = CalculateReverbValue(App->audio->audio_emitters[i]->_gameObject->GetTransform()->GetPosition());
				}
				else { ret = false; }

				Reverb(reverbValue, App->audio->audio_emitters[i]);
			}
		}
	}
	return ret;
}

float AudioReverbZone::CalculateReverbValue(float3 emitterPos)
{
	float reverbValue;
	float dis = sqrt(emitterPos.DistanceSq(this->_gameObject->GetTransform()->GetPosition()));
	reverbValue = dis / r;
	reverbValue = 1 - reverbValue;

	return reverbValue;
}

void AudioReverbZone::Reverb(float d, AudioEmitter* emitter)
{
	//AkRtpcValue value = d;
	emitter->reverbValue = d;
	if (emitter->reverbId != 0)
	{
		AKRESULT result = AK::SoundEngine::SetRTPCValue(emitter->reverbId, emitter->reverbValue);
	}
}