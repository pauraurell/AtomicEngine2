#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GnJSON.h"
#include "Mesh.h"
#include "AudioEmitter.h"
#include "AudioReverbZone.h"
#include "FileSystem.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Camera.h"
#include "Time.h"
#include "ModuleWwise.h"
#include "AK/Wwise_IDs.h"
#include "AK/SpatialAudio/Common/AkSpatialAudio.h"

ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled), show_grid(true), selectedGameObject(nullptr), root(nullptr) 
{
	name = "scene";
	sceneInGame = false;
	mCurrentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	mCurrentGizmoMode = ImGuizmo::MODE::WORLD;
}

ModuleScene::~ModuleScene() {}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	root = new GameObject();
	selectedGameObject = root;
	root->SetName("Root");

	//GameObject* street_environment = App->resources->RequestGameObject("Assets/Models/street/Street environment_V01.fbx");
	//AddGameObject(street_environment);

	camera = new GameObject();
	camera->AddComponent(ComponentType::CAMERA);
	camera->AddComponent(ComponentType::AUDIO_LISTENER);
	camera->SetName("Main Camera");
	camera->GetTransform()->SetPosition(float3(4.332f, 3.327f, -34.66f));
	AddGameObject(camera);
	App->renderer3D->SetMainCamera((Camera*)camera->GetComponent(ComponentType::CAMERA));

	CreateInitialScene();

	return ret;
}

bool ModuleScene::Init()
{
	return true;
}

// Update: draw background
update_status ModuleScene::Update(float dt)
{
	if (show_grid)
	{
		GnGrid grid(24);
		grid.Render();
	}

	HandleInput();

	/*float dis = sqrt(App->camera->GetCamera()->GetPosition().DistanceSq(car->GetTransform()->_position));
	if (dis > 100) { dis = 100; }
	AudioEmitter* carMusic = (AudioEmitter*)car->GetComponent(ComponentType::AUDIO_EMITTER);
	AkRtpcValue distanceValue = dis;
	AKRESULT result = AK::SoundEngine::SetRTPCValue(carMusic->distanceId, distanceValue);*/

	if (App->in_game && sceneInGame == false)
	{
		background_timer.Start();
		background_music = false;
		StartSceneAudioEvents();
		sceneInGame = true;
	}

	root->Update();

	if (root->GetChildByName("Background Music") != nullptr) { BackgroundMusicLoop(); }

	if (App->in_game && root->GetChildByName("Car") != nullptr) { MoveObject(root->GetChildByName("Car"), 0.1); }

	return UPDATE_CONTINUE;
}

void ModuleScene::HandleInput()
{
	if ((App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN) && (selectedGameObject != nullptr) && (selectedGameObject != root))
		selectedGameObject->to_delete = true;

	if ((App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN))
		mCurrentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

	else if((App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN))
		mCurrentGizmoOperation = ImGuizmo::OPERATION::ROTATE;

	else if ((App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN))
		mCurrentGizmoOperation = ImGuizmo::OPERATION::SCALE;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");

	root->DeleteChildren();
	delete root;
	root = nullptr;

	selectedGameObject = nullptr;

	return true;
}

GameObject* ModuleScene::AddGameObject(GameObject* gameObject)
{
	if (gameObject != nullptr) 
	{
		gameObject->SetParent(root);
		root->AddChild(gameObject);

		selectedGameObject = gameObject;
		return gameObject;
	}
}

void ModuleScene::DeleteGameObject(GameObject* gameObject)
{
	if (root->RemoveChild(gameObject))
	{
		gameObject->DeleteChildren();
	}
	else if (gameObject->GetParent()->RemoveChild(gameObject))
	{
		gameObject->DeleteChildren();
	}

	delete gameObject;
	gameObject = nullptr;
}

std::vector<GameObject*> ModuleScene::GetAllGameObjects()
{
	std::vector<GameObject*> gameObjects;

	PreorderGameObjects(root, gameObjects);

	return gameObjects;
}

void ModuleScene::PreorderGameObjects(GameObject* gameObject, std::vector<GameObject*>& gameObjects)
{
	gameObjects.push_back(gameObject);

	for (size_t i = 0; i < gameObject->GetChildrenAmount(); i++)
	{
		PreorderGameObjects(gameObject->GetChildAt(i), gameObjects);
	}
}

void ModuleScene::EditTransform()
{
	if (selectedGameObject == nullptr)
		return;

	float4x4 viewMatrix = App->camera->GetViewMatrixM().Transposed();
	float4x4 projectionMatrix = App->camera->GetProjectionMatrixM().Transposed();
	float4x4 objectTransform = selectedGameObject->GetTransform()->GetGlobalTransform().Transposed();

	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(App->editor->sceneWindowOrigin.x, App->editor->sceneWindowOrigin.y, App->editor->image_size.x, App->editor->image_size.y);

	float tempTransform[16];
	memcpy(tempTransform, objectTransform.ptr(), 16 * sizeof(float));

	ImGuizmo::Manipulate(viewMatrix.ptr(), projectionMatrix.ptr(), mCurrentGizmoOperation, mCurrentGizmoMode, tempTransform);

	if (ImGuizmo::IsUsing())
	{
		float4x4 newTransform;
		newTransform.Set(tempTransform);
		objectTransform = newTransform.Transposed();
		selectedGameObject->GetTransform()->SetGlobalTransform(objectTransform);
	}
}

bool ModuleScene::ClearScene()
{
	bool ret = true;

	root->DeleteChildren();
	root = nullptr;

	return ret;
}

bool ModuleScene::Save(const char* file_path)
{
	bool ret = true;

	GnJSONObj save_file;

	GnJSONArray gameObjects = save_file.AddArray("Game Objects");

	root->Save(gameObjects);

	char* buffer = NULL;
	uint size = save_file.Save(&buffer);

	FileSystem::Save(file_path, buffer, size);

	std::string assets_path = "Assets/Scenes/";
	assets_path.append(FileSystem::GetFile(file_path));

	FileSystem::DuplicateFile(file_path, assets_path.c_str());

	save_file.Release();
	RELEASE_ARRAY(buffer);

	return ret;
}

bool ModuleScene::Load(const char* scene_file)
{
	bool ret = true;

	std::string format = FileSystem::GetFileFormat(scene_file);
	if (format != ".scene")
	{
		LOG_WARNING("%s is not a valid scene format and can't be loaded", scene_file);
		return false;
	}

	ClearScene();

	char* buffer = NULL;
	FileSystem::Load(scene_file, &buffer);
	
	GnJSONObj base_object(buffer);
	GnJSONArray gameObjects(base_object.GetArray("Game Objects"));

	std::vector<GameObject*> createdObjects;

	for (size_t i = 0; i < gameObjects.Size(); i++)
	{
		//load game object
		GameObject* gameObject = new GameObject();
		uint parentUUID = gameObject->Load(&gameObjects.GetObjectAt(i));
		createdObjects.push_back(gameObject);

		//check if it's the root game object
		if (strcmp(gameObject->GetName(), "Root") == 0) {
			root = gameObject;
			selectedGameObject = root;
		}

		//Get game object's parent
		for (size_t i = 0; i < createdObjects.size(); i++)
		{
			if (createdObjects[i]->UUID == parentUUID)
			{
				createdObjects[i]->AddChild(gameObject);
			}
		}
	}

	root->UpdateChildrenTransforms();

	if (root != nullptr)
		LOG("Scene: %s loaded successfully", scene_file);

	return ret;
}

bool ModuleScene::LoadConfig(GnJSONObj& config)
{
	show_grid = config.GetBool("show_grid");

	return true;
}

void ModuleScene::MoveObject(GameObject* obj, float speed)
{
	Transform* transform = (Transform*)obj->GetComponent(ComponentType::TRANSFORM);
	if (transform->_position.x < 20 && transform->_position.z <= 0) 
	{
		transform->_position.x = transform->_position.x + speed; 
		obj->GetChildAt(0)->GetTransform()->SetRotation(-90, 0, 90);
	}
	
	else if (transform->_position.x > 20) 
	{
		transform->_position.z = transform->_position.z + speed;
		obj->GetChildAt(0)->GetTransform()->SetRotation(-90, 0, 0);
	} 
	if (transform->_position.z > 20 && transform->_position.x > 0) 
	{
		transform->_position.x = transform->_position.x - speed;
		obj->GetChildAt(0)->GetTransform()->SetRotation(-90, 0, 270);
	} 
	else if (transform->_position.x < 0) 
	{
		transform->_position.z = transform->_position.z - speed;
		obj->GetChildAt(0)->GetTransform()->SetRotation(-90, 0, 180);
	}
	transform->UpdateGlobalTransform();
	obj->UpdateChildrenTransforms();
}

void ModuleScene::BackgroundMusicLoop()
{
	if (background_timer.ReadSec() >= 30)
	{
		if (background_music == false)
		{
			AudioEmitter* music = (AudioEmitter*)root->GetChildByName("Background Music")->GetComponent(ComponentType::AUDIO_EMITTER);
			music->SetID(AK::EVENTS::SONG1_TO_2);
			App->audio->PlayEvent(AK::EVENTS::SONG1_TO_2, music->emitter);
			background_timer.Start();
			background_music = true;
		}
		else if (background_music == true)
		{
			AudioEmitter* music = (AudioEmitter*)root->GetChildByName("Background Music")->GetComponent(ComponentType::AUDIO_EMITTER);
			music->SetID(AK::EVENTS::SONG2_TO_1);
			App->audio->PlayEvent(AK::EVENTS::SONG2_TO_1, music->emitter);
			background_timer.Start();
			background_music = false;
		}
	}
}

void ModuleScene::StartSceneAudioEvents()
{
	if (root->GetChildByName("Background Music") != nullptr) {
		AudioEmitter* music = (AudioEmitter*)root->GetChildByName("Background Music")->GetComponent(ComponentType::AUDIO_EMITTER);
		music->SetID(AK::EVENTS::START_LOOP);
		App->audio->PlayEvent(AK::EVENTS::START_LOOP, music->emitter);
	}
	
	if (root->GetChildByName("Car") != nullptr) {
		AudioEmitter* carMusic = (AudioEmitter*)root->GetChildByName("Car")->GetComponent(ComponentType::AUDIO_EMITTER);
		carMusic->SetID(AK::EVENTS::PLAYMOVINGOBJ);
		App->audio->PlayEvent(AK::EVENTS::PLAYMOVINGOBJ, carMusic->emitter);
	}
	if (root->GetChildByName("Rayan") != nullptr) {
		AudioEmitter* rayanMusic = (AudioEmitter*)root->GetChildByName("Rayan")->GetComponent(ComponentType::AUDIO_EMITTER);
		rayanMusic->SetID(AK::EVENTS::PLAYSTATICOBJ);
		App->audio->PlayEvent(AK::EVENTS::PLAYSTATICOBJ, rayanMusic->emitter);
	}
	
}

void ModuleScene::StopSceneAudioEvents()
{
	if (root->GetChildByName("Background Music") != nullptr) {
		AudioEmitter* music = (AudioEmitter*)root->GetChildByName("Background Music")->GetComponent(ComponentType::AUDIO_EMITTER);
		App->audio->StopEvent(AK::EVENTS::START_LOOP, music->emitter);
	}
	if (root->GetChildByName("Car") != nullptr) {
		AudioEmitter* carMusic = (AudioEmitter*)root->GetChildByName("Car")->GetComponent(ComponentType::AUDIO_EMITTER);
		App->audio->StopEvent(AK::EVENTS::PLAYMOVINGOBJ, carMusic->emitter);
	}
}

void ModuleScene::CreateInitialScene()
{
	GameObject* BackgroundMusic;
	BackgroundMusic = new GameObject();
	BackgroundMusic->SetName("Background Music");
	BackgroundMusic->AddComponent(ComponentType::AUDIO_EMITTER);
	AddGameObject(BackgroundMusic);
	AudioEmitter* music = (AudioEmitter*)BackgroundMusic->GetComponent(ComponentType::AUDIO_EMITTER);

	GameObject* TunnelEffect = new GameObject();
	TunnelEffect = AddGameObject(App->resources->RequestGameObject("Assets/Models/road/tunel.fbx"));
	TunnelEffect->AddComponent(ComponentType::AUDIO_REVERB_ZONE);
	AudioReverbZone* reverbZ = (AudioReverbZone*)TunnelEffect->GetComponent(ComponentType::AUDIO_REVERB_ZONE);
	reverbZ->r = 6.7f;
	TunnelEffect->SetName("Tunnel Effect");
	TunnelEffect->GetTransform()->SetScale(float3(0.110f, 0.110f, 0.110f));
	TunnelEffect->GetTransform()->SetPosition(float3(22.025f, 0.0f, 10.187f));
	TunnelEffect->GetTransform()->SetRotation(90.0f, -90.0f, 0.0f);
	TunnelEffect->GetChildByName("Tube001")->GetTransform()->SetPosition(float3(-5.663f, -59.86f, 0.267f));
	//Material* tunnelMat = (Material*)TunnelEffect->AddComponent(ComponentType::MATERIAL);
	//tunnelMat->SetTexture((ResourceTexture*)App->resources->RequestResource(App->resources->ImportFile("Assets/Textures/tunel.png")));

	GameObject* car = new GameObject();
	car = AddGameObject(App->resources->RequestGameObject("Assets/Models/car/FuzzyRed.fbx"));
	car->AddComponent(ComponentType::AUDIO_EMITTER);
	AudioEmitter* carMusic = (AudioEmitter*)car->GetComponent(ComponentType::AUDIO_EMITTER);
	carMusic->bypass_reverb_zones = false;
	carMusic->distanceId = AK::GAME_PARAMETERS::RTPC_DISTANCE;
	carMusic->reverbId = AK::GAME_PARAMETERS::RTPC_REVERB;
	car->SetName("Car");

	GameObject* Rayan = new GameObject();
	Rayan = AddGameObject(App->resources->RequestGameObject("Assets/Models/Rayman/rayman.fbx"));
	Rayan->AddComponent(ComponentType::AUDIO_EMITTER);
	AudioEmitter* rayanMusic = (AudioEmitter*)Rayan->GetComponent(ComponentType::AUDIO_EMITTER);
	rayanMusic->bypass_reverb_zones = false;
	rayanMusic->reverbId = AK::GAME_PARAMETERS::RTPC_REVERB2;
	Rayan->SetName("Rayan");
	Rayan->GetTransform()->SetPosition(float3(-15.82f, 0.0f, 19.731f));
}