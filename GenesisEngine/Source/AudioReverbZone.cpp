#include "AudioReverbZone.h"
#include "Application.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "GnJSON.h"
#include "Transform.h"

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
	sphere.pos = t->_position;
	sphere.r = r;

	glLineWidth(1.0f);
	glColor3f(1.0f, 0.0f, 1.0f);

	int num_vertex = 20;
	float a = 360.f / (float)num_vertex * DEGTORAD;

	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < num_vertex; i++)
		glVertex3f(cos(a * i) * sphere.r + sphere.pos.x, sphere.pos.y, sin(a * i) * sphere.r + sphere.pos.z);
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < num_vertex; i++)
		glVertex3f(cos(a * i) * sphere.r + sphere.pos.x, sin(a * i) * sphere.r + sphere.pos.y, sphere.pos.z);
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < num_vertex; i++)
		glVertex3f(sphere.pos.x, sin(a * i) * sphere.r + sphere.pos.y, cos(a * i) * sphere.r + sphere.pos.z);
	glEnd();
}

void AudioReverbZone::OnEditor()
{
	if (ImGui::CollapsingHeader("Audio Reverb Zone", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Radius", &r, 0.1, 0, 50);
	}
}