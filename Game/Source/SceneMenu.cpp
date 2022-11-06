#include "SceneMenu.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "ModuleFadeToBlack.h"
#include "SceneIntro.h"
#include "Scene.h"

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

	//lateralBarsAnim.PushBack({ 0,0,2,15 });//pink
	//lateralBarsAnim.PushBack({ 2,0,2,15 });//red
	//lateralBarsAnim.PushBack({ 4,0,2,15 });//orange
	//lateralBarsAnim.PushBack({ 6,0,2,15 });//yellow
	//lateralBarsAnim.PushBack({ 8,0,2,15 });//white
	//lateralBarsAnim.PushBack({ 10,0,2,15 });//green
	//lateralBarsAnim.PushBack({ 12,0,2,15 });//cyan
	//lateralBarsAnim.PushBack({ 14,0,2,15 });//blue
	//lateralBarsAnim.loop = true;
	//lateralBarsAnim.speed = 0.2f;

	modebgTexture = app->tex->Load("Assets/Textures/menu_provisional.png");

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return true;
}

bool SceneMenu::Update(float dt)
{
	//lateralBarsAnim.Update();
	//lateralBarCounter++;

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		app->fade->FadeToBlack(this, (Module*)app->scene, 30);
	}

	return true;
}

bool SceneMenu::PostUpdate()
{
	bool ret = true;

	// Draw everything --------------------------------------
	app->render->DrawTexture(modebgTexture, 0, 0);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool SceneMenu::CleanUp()
{
	LOG("Deleting background assets");

	app->tex->Unload(modebgTexture);

	return true;
}