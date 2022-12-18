#include "SceneMenu.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "ModuleFadeToBlack.h"
#include "SceneIntro.h"
#include "SceneLevel1.h"

#include "Log.h"


SceneMenu::SceneMenu(bool startEnabled) : Module(startEnabled)
{
	name.Create("sceneMenu");
}

SceneMenu::~SceneMenu()
{

}

// Load assets
bool SceneMenu::Start()
{
	LOG("Loading Scene Menu");

	pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child(name.GetString());

	menu.PushBack({ 0,0,512,384 });
	menu.PushBack({ 0,384,512,384 });
	menu.PushBack({ 512,384,512,384 });
	menu.PushBack({ 512,0,512,384 });
	menu.loop = false;
	menu.speed = 0.1f;

	buttons.PushBack({ 0,0,243,70 });
	buttons.PushBack({ 0,70,243,70 });
	buttons.loop = true;

	menuTexture = app->tex->Load(config.child("menu").attribute("texturepath").as_string());
	buttonTexture = app->tex->Load(config.child("buttons").attribute("texturepath").as_string());

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	bool loadPrevious = false;

	return true;
}

bool SceneMenu::Update(float dt)
{
	if (appStart) {
		if (iconCounter <= 120) {
			appStart = false;
		}
		iconCounter--;
	}
	else if (iconCounter > 0) {
		menu.Update();
		iconCounter--;
	}
	else {
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
			app->input->GetMouseX() > 136 &&
			app->input->GetMouseX() < 379 &&
			app->input->GetMouseY() > 118 &&
			app->input->GetMouseY() < 188) {
			app->fade->FadeToBlack(this, (Module*)app->sceneLevel2, 30);
		}
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN &&
			app->input->GetMouseX() > 136 &&
			app->input->GetMouseX() < 379 &&
			app->input->GetMouseY() > 204 &&
			app->input->GetMouseY() < 274) {
			pugi::xml_document gameStateFile;
			pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

			if (result == NULL)
			{
				LOG("Could not load xml file savegame.xml. pugi error: %s", result.description());
			}
			else
			{
				if (gameStateFile.child("save_state").child("sceneMenu").child("scene").attribute("level").as_int() == 1) {
					app->fade->FadeToBlack(this, (Module*)app->sceneLevel1, 30);
					loadPrevious = true;
				}
				else if (gameStateFile.child("save_state").child("sceneMenu").child("scene").attribute("level").as_int() == 2) {
					app->fade->FadeToBlack(this, (Module*)app->sceneLevel2, 30);
					loadPrevious = true;
				}
			}
		}
	}

	return true;
}

bool SceneMenu::PostUpdate()
{
	bool ret = true;

	// Draw everything --------------------------------------
	SDL_Rect rect = menu.GetCurrentFrame();
	app->render->DrawTexture(menuTexture, 0, 0, &rect);

	if (appStart) {
		if (iconCounter <= 120) {
			appStart = false;
		}
		iconCounter--;
	}
	else if (iconCounter > 0) {
		menu.Update();
		iconCounter--;
	}
	else {
		if (app->input->GetMouseX() > 136 &&
			app->input->GetMouseX() < 379 &&
			app->input->GetMouseY() > 118 &&
			app->input->GetMouseY() < 188)
		{
			buttons.SetCurrentFrame(0);
			SDL_Rect rect = buttons.GetCurrentFrame();
			app->render->DrawTexture(buttonTexture, 134, 116, &rect);
		}
		else if (app->input->GetMouseX() > 136 &&
			app->input->GetMouseX() < 379 &&
			app->input->GetMouseY() > 204 &&
			app->input->GetMouseY() < 274)
		{
			buttons.SetCurrentFrame(1);
			SDL_Rect rect = buttons.GetCurrentFrame();
			app->render->DrawTexture(buttonTexture, 134, 198, &rect);
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool SceneMenu::CleanUp()
{
	LOG("Deleting background assets");

	app->tex->Unload(menuTexture);

	return true;
}

// L03: DONE 8: Create a method to save the state of the renderer
// using append_child and append_attribute
bool SceneMenu::SaveState(pugi::xml_node& data)
{
	pugi::xml_node playerNude = data.append_child("scene");

	playerNude.append_attribute("level") = currentLevel;

	return true;
}