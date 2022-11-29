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
#include "Physics.h"
#include "PathFinding.h"

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
	victory_defeat = app->tex->Load(config.child("textures").attribute("victory_defeat").as_string());
	death_text = app->tex->Load(config.child("textures").attribute("death_text").as_string());

	victory.PushBack({ 0,0,512,384 });
	victory.loop = false;
	victory.speed = 0.1f;

	defeat.PushBack({ 0,384,512,384 });
	defeat.loop = false;
	defeat.speed = 0.1f;

	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	playerDeath = false;
	musicPath = config.child("music").attribute("path").as_string();
	app->audio->PlayMusic(musicPath);

	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Maps/path.png");
	// Texture to show path origin 
	originTex = app->tex->Load("Assets/Maps/x.png");

	// L03: DONE: Load map
	bool retLoad = app->map->Load(name.GetString());

	// L12 Create walkability map
	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}

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
	//DEBUG KEYS
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		app->fade->FadeToBlack(this, (Module*)app->sceneLevel1, 0);
	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		//app->fade->FadeToBlack(this, (Module*)app->sceneLevel2, 0);
	}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		app->fade->FadeToBlack(this, this, 0);
	}

	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		if (app->physics->debug) { app->physics->debug = false; }
		else if (!app->physics->debug) { app->physics->debug = true; }
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		if (godMode) { godMode = false; }
		else if (!godMode) { godMode = true; }
	}

	if (player->position.x > 400 / app->win->GetScale() && player->position.x < ((app->map->mapData.tileWidth * app->map->mapData.width) - 616 / app->win->GetScale())) {
		app->render->camera.x = ((player->position.x - 400 / app->win->GetScale()) * -1) * app->win->GetScale();
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

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);
	iPoint mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x - app->map->mapData.tileWidth / 2,
		mouseY - app->render->camera.y - app->map->mapData.tileHeight / 2);

	//Convert again the tile coordinates to world coordinates to render the texture of the tile
	iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
	app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);

	//Test compute path function
	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (originSelected == true)
		{
			app->pathfinding->CreatePath(origin, mouseTile);
			originSelected = false;
		}
		else
		{
			origin = mouseTile;
			originSelected = true;
			app->pathfinding->ClearLastPath();
		}
	}

	// L12: Get the latest calculated path and draw
	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
	}

	// L12: Debug pathfinding
	iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
	app->render->DrawTexture(originTex, originScreen.x, originScreen.y);


	// Draw map
	app->map->Draw();

	return true;
}

// Called each loop iteration
bool SceneLevel1::PostUpdate()
{
	bool ret = true;

	if (player->GetState() == 7) {
		app->render->DrawTexture(death_text, ((app->render->camera.x) * -1) / app->win->GetScale(), ((app->render->camera.y) * -1) / app->win->GetScale());

		SDL_Rect rect = defeat.GetCurrentFrame();
		app->render->DrawTexture(victory_defeat, ((app->render->camera.x)*-1)/ app->win->GetScale(), ((app->render->camera.y)*-1)/app->win->GetScale(), &rect);
	}

	if (player->GetState() == 10) {
		SDL_Rect rect = victory.GetCurrentFrame();
		app->render->DrawTexture(victory_defeat, ((app->render->camera.x) * -1) / app->win->GetScale(), ((app->render->camera.y) * -1) / app->win->GetScale(), &rect);
	}

	if((app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) || (player->endLevel))
		app->fade->FadeToBlack(this, (Module*)app->sceneMenu, 30);

	return ret;
}

// Called before quitting
bool SceneLevel1::CleanUp()
{
	LOG("Freeing sceneLevel1");

	if (app->entityManager->isEnabled) { app->entityManager->Disable(); }

	app->map->CleanUp();

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	app->audio->PlayMusic("");

	app->tex->Unload(victory_defeat);
	app->tex->Unload(death_text);

	return true;
}

bool SceneLevel1::LoadState(pugi::xml_node& data)
{
	PhysBody* pbody = player->getpBody();

	pbody->SetPosition(data.child("player").attribute("x").as_int(), data.child("player").attribute("y").as_int());

	return true;
}

// L03: DONE 8: Create a method to save the state of the renderer
// using append_child and append_attribute
bool SceneLevel1::SaveState(pugi::xml_node& data)
{
	pugi::xml_node playerNude = data.append_child("player");

	playerNude.append_attribute("x") = player->position.x + 16;
	playerNude.append_attribute("y") = player->position.y + 16;

	return true;
}
