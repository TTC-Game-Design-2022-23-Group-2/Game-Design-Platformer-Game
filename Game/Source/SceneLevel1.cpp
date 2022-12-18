#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLevel1.h"
#include "SceneMenu.h"
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

	if (!app->physics->isEnabled) { app->physics->Enable(); }

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

    configNode = app->LoadConfigFileToVar();
	config = configNode.child(name.GetString());
	// SMALL TERR ENEMIES
	for (pugi::xml_node pikuEnemyNode = config.child("terrestreenemysmall"); pikuEnemyNode; pikuEnemyNode = pikuEnemyNode.next_sibling("terrestreenemysmall"))
	{
		TerrestreEnemySmall* pikuEnemy = (TerrestreEnemySmall*)app->entityManager->CreateEntity(EntityType::PIKU);
		pikuEnemy->parameters = pikuEnemyNode;
		terrestreSmallEnemies.Add(pikuEnemy);
	}

	configNode = app->LoadConfigFileToVar();
	config = configNode.child(name.GetString());
	// BIG TERR ENEMIES
	for (pugi::xml_node mamapikuEnemyNode = config.child("terrestreenemybig"); mamapikuEnemyNode; mamapikuEnemyNode = mamapikuEnemyNode.next_sibling("terrestreenemybig"))
	{
		TerrestreEnemyBig* mamapikuEnemy = (TerrestreEnemyBig*)app->entityManager->CreateEntity(EntityType::MAMAPIKU);
		mamapikuEnemy->parameters = mamapikuEnemyNode;
		terrestreBigEnemies.Add(mamapikuEnemy);
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
	mouseTileTex = app->tex->Load(config.child("textures").attribute("path").as_string());
	// Texture to show path origin 
	originTex = app->tex->Load(config.child("textures").attribute("x").as_string());

	// L03: DONE: Load map
	if (!app->map->isEnabled) { app->map->Enable(); }
	bool retLoad = app->map->Load(name.GetString());

	// L12 Create walkability map
	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}

	if (app->sceneMenu->loadPrevious) {
		app->LoadGameRequest();
		app->sceneMenu->loadPrevious = false;
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
	if (player->position.x > 400 / app->win->GetScale() && player->position.x < ((app->map->mapData.tileWidth * app->map->mapData.width) - 616 / app->win->GetScale())) {
		app->render->camera.x = ((player->position.x - 400 / app->win->GetScale()) * -1) * app->win->GetScale();
	}

	if (player->position.y > 300 / app->win->GetScale() && player->position.y < ((app->map->mapData.tileHeight * app->map->mapData.height) - 458 / app->win->GetScale())) {
		app->render->camera.y = ((player->position.y - 300 / app->win->GetScale()) * -1) * app->win->GetScale();
	}

	// Draw map
	app->map->Draw();

	//ENEMY PATHFINDING
	//Terrestre small enemies

	ListItem<TerrestreEnemySmall*>* terrestreSmallEnemyItem = terrestreSmallEnemies.start;

	while (terrestreSmallEnemyItem != NULL)
	{
		if (terrestreSmallEnemyItem != NULL && terrestreSmallEnemyItem->data->GetState() == 2) {
			app->pathfinding->ClearLastPath();

			//Define origin of path
			origin.x = METERS_TO_PIXELS(terrestreSmallEnemyItem->data->getpBody()->body->GetPosition().x);
			origin.y = METERS_TO_PIXELS(terrestreSmallEnemyItem->data->getpBody()->body->GetPosition().y);
			origin = app->map->WorldToMap(origin.x, origin.y);

			//Define destination of path
			destination.x = METERS_TO_PIXELS(player->getpBody()->body->GetPosition().x);
			destination.y = METERS_TO_PIXELS(player->getpBody()->body->GetPosition().y);
			destination = app->map->WorldToMap(destination.x, destination.y);

			app->pathfinding->CreatePath(origin, destination);

			const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				if (i == 1) {
					//Pass the movement info to the enemy
					terrestreSmallEnemyItem->data->objective.x = PIXEL_TO_METERS(pos.x);
					terrestreSmallEnemyItem->data->objective.y = PIXEL_TO_METERS(pos.y);
				}
				if (app->physics->debug) { app->render->DrawTexture(mouseTileTex, pos.x, pos.y); }
			}
			//Draw path
			iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
			if (app->physics->debug) { app->render->DrawTexture(originTex, originScreen.x, originScreen.y); }
		}
		terrestreSmallEnemyItem = terrestreSmallEnemyItem->next;
	}

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

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || player->endLevelDie)
		app->fade->FadeToBlack(this, (Module*)app->sceneMenu, 30);

	//DEBUG KEYS
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		app->fade->FadeToBlack(this, (Module*)app->sceneLevel1, 0);
	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN || player->endLevel) {
		app->fade->FadeToBlack(this, (Module*)app->sceneLevel2, 0);
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
		if (player->godMode) { player->godMode = false; }
		else if (!player->godMode) { player->godMode = true; }
	}

	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		if (app->FPS == 60) { app->FPS = 30; }
		else if (app->FPS == 30) { app->FPS = 60; }
	}

	if (app->physics->debug) {
		ListItem<TerrestreEnemySmall*>* terrestreSmallEnemyItem = terrestreSmallEnemies.start;

		while (terrestreSmallEnemyItem != NULL)
		{
			if (terrestreSmallEnemyItem->data->active) {
				PhysBody* pbodyP = player->getpBody();
				PhysBody* pbodyE = terrestreSmallEnemyItem->data->getpBody();
				app->render->DrawLine(METERS_TO_PIXELS(pbodyE->body->GetPosition().x),
					METERS_TO_PIXELS(pbodyE->body->GetPosition().y),
					METERS_TO_PIXELS(pbodyP->body->GetPosition().x),
					METERS_TO_PIXELS(pbodyP->body->GetPosition().y), 255, 0, 0);
			}
			terrestreSmallEnemyItem = terrestreSmallEnemyItem->next;
		}
	}

	if (app->physics->debug) {
		ListItem<TerrestreEnemyBig*>* terrestreBigEnemyItem = terrestreBigEnemies.start;

		while (terrestreBigEnemyItem != NULL)
		{
			if (terrestreBigEnemyItem->data->active) {
				PhysBody* pbodyP = player->getpBody();
				PhysBody* pbodyE = terrestreBigEnemyItem->data->getpBody();
				app->render->DrawLine(METERS_TO_PIXELS(pbodyE->body->GetPosition().x),
					METERS_TO_PIXELS(pbodyE->body->GetPosition().y),
					METERS_TO_PIXELS(pbodyP->body->GetPosition().x),
					METERS_TO_PIXELS(pbodyP->body->GetPosition().y), 255, 0, 0);
			}
			terrestreBigEnemyItem = terrestreBigEnemyItem->next;
		}
	}

	return ret;
}

// Called before quitting
bool SceneLevel1::CleanUp()
{
	LOG("Freeing sceneLevel1");

	if (app->entityManager->isEnabled) { app->entityManager->Disable(); }
	if (app->map->isEnabled) { app->map->Disable(); }
	if (app->physics->isEnabled) { app->physics->Disable(); }

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	app->audio->PlayMusic("");

	app->tex->Unload(victory_defeat);
	app->tex->Unload(death_text);

	app->tex->Unload(mouseTileTex);
	app->tex->Unload(originTex);

	return true;
}

bool SceneLevel1::LoadState(pugi::xml_node& data)
{
	if(isEnabled){
		PhysBody* pbody = player->getpBody();

		pbody->SetPosition(data.child("player").attribute("x").as_int(), data.child("player").attribute("y").as_int());
		player->position.x = (pbody->body->GetPosition().x) + 16;
		player->position.y = (pbody->body->GetPosition().x) + 16;
	}

	return true;
}

// L03: DONE 8: Create a method to save the state of the renderer
// using append_child and append_attribute
bool SceneLevel1::SaveState(pugi::xml_node& data)
{
	if(isEnabled){
		pugi::xml_node playerNude = data.append_child("player");

		playerNude.append_attribute("x") = player->position.x + 16;
		playerNude.append_attribute("y") = player->position.y + 16;

		app->sceneMenu->currentLevel = 1;
	}

	return true;
}
