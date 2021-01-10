#pragma once
#include "Module.h"
#include "Globals.h"
#include "ImGui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"
#include "Timer.h"

#include <vector>

class GameObject;
class GnTexture;

class ModuleScene : public Module
{
public:
	ModuleScene(bool start_enabled = true);
	~ModuleScene();

	bool Start();
	bool Init();
	bool LoadConfig(GnJSONObj& config) override;
	update_status Update(float dt);
	//update_status PostUpdate(float dt);
	void HandleInput();
	bool CleanUp();

	GameObject* AddGameObject(GameObject* gameObject);
	void DeleteGameObject(GameObject* gameObject);
	GameObject* GetRoot() { return root; }
	std::vector<GameObject*> GetAllGameObjects();
	void PreorderGameObjects(GameObject* gameObject, std::vector<GameObject*>& gameObjects);
	void EditTransform();
	void CreateInitialScene();

	bool ClearScene();

	bool Save(const char* file_path);
	bool Load(const char* scene_file);

	void MoveObject(GameObject* obj, float speed);

	void BackgroundMusicLoop();

	void StartSceneAudioEvents();
	void StopSceneAudioEvents();

public:
	bool show_grid;
	GameObject* camera;
	GameObject* selectedGameObject;
	Timer background_timer;
	bool background_music;
	bool sceneInGame;

private:
	GameObject* root;
	ImGuizmo::OPERATION mCurrentGizmoOperation;
	ImGuizmo::MODE mCurrentGizmoMode;
};
