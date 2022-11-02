#include "SceneIntro.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "ModuleFadeToBlack.h"
#include "Window.h"

#include "Log.h"

SceneIntro::SceneIntro(bool startEnabled) : Module(startEnabled)
{
	name.Create("sceneIntro");
}

SceneIntro::~SceneIntro()
{

}

// Load assets
bool SceneIntro::Start()
{
	LOG("Loading background assets");

	bgTexture = app->tex->Load("Assets/Sprites/sprite_title.png");
	introTexture = app->tex->Load("Assets/Sprites/PreTitle.png");

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return true;
}

bool SceneIntro::Update(float dt)
{
	if ((!play_intro) && (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN))
	{
		app->fade->FadeToBlack(this, (Module*)app->sceneMenu, 90);
	}
	else if ((!play_intro) && (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN))
	{
		return false;
	}
	else if ((play_intro) && (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN))
	{
		uint tex_w, tex_h;
		app->tex->GetSize(introTexture, tex_w, tex_h);
		app->render->camera.y = ((tex_h - app->win->GetHeight()) * app->win->GetScale());
	}

	return true;
}

bool SceneIntro::PostUpdate()
{
	// Draw everything --------------------------------------
	uint tex_w, tex_h;
	app->tex->GetSize(introTexture, tex_w, tex_h);
	int camera = ((app->render->camera.y / app->win->GetScale()) + app->win->GetHeight());

	if (camera == tex_h) {
		counter--;
	}
	else {
		app->render->camera.y++;
	}
	if (counter == 0) {
		play_intro = false;
	}

	if (play_intro) {
		app->render->DrawTexture(introTexture, 0, 0, NULL);
	}
	else {
		app->render->camera.y = 0;
		app->render->DrawTexture(bgTexture, 0, 0, NULL);
	}

	return true;
}

bool SceneIntro::CleanUp()
{
	LOG("Deleting background assets");

	app->tex->Unload(bgTexture);

	return true;
}