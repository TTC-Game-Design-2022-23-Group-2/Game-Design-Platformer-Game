#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLevel1.h"
#include "EntityManager.h"
#include "Map.h"
#include "ModuleFadeToBlack.h"

#include "Defs.h"
#include "Log.h"

SceneLevel1::SceneLevel1(bool startEnabled) : Module(startEnabled)
{
	name.Create("sceneLevel1");
}

// Destructor
SceneLevel1::~SceneLevel1()
{}

// Called before render is available
bool SceneLevel1::Awake(pugi::xml_node& config)
{
	return true;
}

// Called before the first frame
bool SceneLevel1::Start()
{
	LOG("Loading SceneLevel1");
	bool ret = true;

	pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child(name.GetString());

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	//L02: DONE 3: Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");
	
	app->render->camera.x = 0;
	app->render->camera.y = -192;

	//IMPORTANT, ENTITY MANAGER IS DISABLED BY DEFAULT
	if(!app->entityManager->isEnabled) { app->entityManager->Enable(); }
	//img = app->tex->Load("Assets/Textures/test.png");
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
	
	// L03: DONE: Load map
	app->map->Load(name.GetString());

	// L04: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	app->win->SetTitle(title.GetString());

	return ret;
}

// Called each loop iteration
bool SceneLevel1::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneLevel1::Update(float dt)
{
	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		if (godMode) { godMode = false; }
		if (!godMode) { godMode = true; }
	}

	if (player->position.x > 400 / app->win->GetScale() && player->position.x < ((app->map->mapData.tileWidth * app->map->mapData.width) - 616 / app->win->GetScale())) {
		app->render->camera.x = ((player->position.x - 400 / app->win->GetScale())*-1)* app->win->GetScale();
	}

	if (player->position.y > 300 / app->win->GetScale() && player->position.y < ((app->map->mapData.tileHeight * app->map->mapData.height) - 458 / app->win->GetScale())) {
		app->render->camera.y = ((player->position.y - 300 / app->win->GetScale()) * -1) * app->win->GetScale();
	}

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		app->render->camera.x += 1;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		app->render->camera.x -= 1;

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
		app->render->camera.y -= 1;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
		app->render->camera.y += 1;

	// Draw map
	app->map->Draw();

	return true;
}

// Called each loop iteration
bool SceneLevel1::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		app->fade->FadeToBlack(this, (Module*)app->sceneMenu, 30);

	return ret;
}

// Called before quitting
bool SceneLevel1::CleanUp()
{
	LOG("Freeing scene");

	if (app->entityManager->isEnabled) { app->entityManager->Disable(); }

	app->map->CleanUp();

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return true;
}
