#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLevel1.h"
#include "SceneGui.h"
#include "ModuleFonts.h"
#include "PauseMenus.h"

#include "Defs.h"
#include "Log.h"

SceneGui::SceneGui(bool startEnabled) : Module(startEnabled)
{
	name.Create("sceneGui");
}

// Destructor
SceneGui::~SceneGui()
{}

// Called before render is available
bool SceneGui::Awake(pugi::xml_node& config)
{
	return true;
}

// Called before the first frame
bool SceneGui::Start()
{
	money_anim.PushBack({ 0 * 22,0,22,22 });
	money_anim.PushBack({ 1 * 22,0,22,22 });
	money_anim.PushBack({ 2 * 22,0,22,22 });
	money_anim.PushBack({ 3 * 22,0,22,22 });
	money_anim.PushBack({ 0 * 22,0,22,22 });
	money_anim.loop = false;
	money_anim.speed = 0.1f;

	lives_anim1.PushBack({ 0 * 44,0,44,44 });
	lives_anim1.PushBack({ 1 * 44,0,44,44 });
	lives_anim1.PushBack({ 2 * 44,0,44,44 });
	lives_anim1.PushBack({ 3 * 44,0,44,44 });
	lives_anim1.PushBack({ 4 * 44,0,44,44 });
	lives_anim1.loop = false;
	lives_anim1.speed = 0.1f;

	lives_anim2.PushBack({ 0 * 44,0,44,44 });
	lives_anim2.PushBack({ 1 * 44,0,44,44 });
	lives_anim2.PushBack({ 2 * 44,0,44,44 });
	lives_anim2.PushBack({ 3 * 44,0,44,44 });
	lives_anim2.PushBack({ 4 * 44,0,44,44 });
	lives_anim2.loop = false;
	lives_anim2.speed = 0.1f;

	lives_anim3.PushBack({ 0 * 44,0,44,44 });
	lives_anim3.PushBack({ 1 * 44,0,44,44 });
	lives_anim3.PushBack({ 2 * 44,0,44,44 });
	lives_anim3.PushBack({ 3 * 44,0,44,44 });
	lives_anim3.PushBack({ 4 * 44,0,44,44 });
	lives_anim3.loop = false;
	lives_anim3.speed = 0.1f;

	LOG("Loading SceneLevel1");
	bool ret = true;

	pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child(name.GetString());

	lives_text = app->tex->Load(config.child("texturepaths").attribute("lives").as_string());
	money_text = app->tex->Load(config.child("texturepaths").attribute("money").as_string());
	fontYellow_text = app->fonts->Load(config.child("texturepaths").attribute("fontyellow").as_string(), "1234567890", 1);

	//Reset variables
	lives = config.child("gamedata").attribute("lives").as_int();
	money = config.child("gamedata").attribute("money").as_int();

	money_anim.SetCurrentFrame(4);
	lives_anim1.SetCurrentFrame(4);
	lives_anim2.SetCurrentFrame(4);
	lives_anim3.SetCurrentFrame(4);

	return ret;
}

// Called each loop iteration
bool SceneGui::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneGui::Update(float dt)
{
	if (app->pauseMenus->isPaused()) { return true; }

	if (lives > 3) { lives = 3; }
	if (money > 99999) { money = 99999; }
	if (lives < 3) {
		lives_anim3.Reset();
	}
	if (lives < 2) {
		lives_anim2.Reset();
	}
	if (lives < 1) {
		lives_anim1.Reset();
	}

	return true;
}

// Called each loop iteration
bool SceneGui::PostUpdate()
{
	bool ret = true;

	//Draw GUI elements
	money_anim.Update();
	SDL_Rect rect = money_anim.GetCurrentFrame();
	app->render->DrawTexture(money_text, 480, 10, &rect, false);

	sprintf_s(moneyText, 6, "%d", money);
	app->fonts->BlitText(460, 7, fontYellow_text, moneyText, false);

	if (lives >= 1) { lives_anim1.Update(); }
	rect = lives_anim1.GetCurrentFrame();
	app->render->DrawTexture(lives_text, 0, 0, &rect, false);

	if (lives >= 2) { lives_anim2.Update(); }
	rect = lives_anim2.GetCurrentFrame();
	app->render->DrawTexture(lives_text, 44, 0, &rect, false);

	if (lives >= 3) { lives_anim3.Update(); }
	rect = lives_anim3.GetCurrentFrame();
	app->render->DrawTexture(lives_text, 88, 0, &rect, false);

	return ret;
}

// Called before quitting
bool SceneGui::CleanUp()
{
	LOG("Freeing sceneGui");

	app->tex->Unload(lives_text);
	app->tex->Unload(money_text);

	return true;
}

bool SceneGui::LoadState(pugi::xml_node& data)
{
	money = data.child("gamedata").attribute("money").as_int();
	lives = data.child("gamedata").attribute("lives").as_int();

	return true;
}

// L03: DONE 8: Create a method to save the state of the renderer
// using append_child and append_attribute
bool SceneGui::SaveState(pugi::xml_node& data)
{
	pugi::xml_node guiNude = data.append_child("gamedata");

	guiNude.append_attribute("lives") = lives;
	guiNude.append_attribute("money") = money;

	return true;
}
