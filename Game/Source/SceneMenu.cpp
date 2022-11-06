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

	menuTexture = app->tex->Load(config.child("menu").attribute("texturepath").as_string());

	app->render->camera.x = 0;
	app->render->camera.y = 0;

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
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			app->fade->FadeToBlack(this, (Module*)app->scene, 30);
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