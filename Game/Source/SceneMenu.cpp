#include "SceneMenu.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "ModuleFadeToBlack.h"
#include "SceneLevel1.h"
#include "SceneGui.h"
#include "PauseMenus.h"

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
	buttonTexture = app->tex->Load(config.child("buttons").attribute("texturepath").as_string());
	buttonTextureSelected = app->tex->Load(config.child("buttons").attribute("texturepathselected").as_string());

	//GUI Buttons
	startButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, { 134,116,243,70 }, this, { 0,0,243,70 }, buttonTexture, buttonTextureSelected, {});
	continueButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, { 134,198,243,70 }, this, { 0,70,243,70 }, buttonTexture, buttonTextureSelected, {});
	exitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, { 188,280,135,40 }, this, { 54,140,135,40 }, buttonTexture, buttonTextureSelected, {});
	creditsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, { 134,280,49,40 }, this, { 0,140,49,40 }, buttonTexture, buttonTextureSelected, {});
	settingsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, { 328,280,49,40 }, this, { 194,140,49,40 }, buttonTexture, buttonTextureSelected, {});

	startButton->state = GuiControlState::NORMAL;
	continueButton->state = GuiControlState::NORMAL;
	exitButton->state = GuiControlState::NORMAL;
	creditsButton->state = GuiControlState::NORMAL;
	settingsButton->state = GuiControlState::NORMAL;

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	bool loadPrevious = false;

	//IMPORTANT, SCENE GUI IS RESETED WHEN GO ON TITLE
	if (app->sceneGui->isEnabled) { app->sceneGui->Disable(); }

	return true;
}

bool SceneMenu::PreUpdate() {
	if (app->pauseMenus->isPaused()) {
		startButton->state = GuiControlState::DISABLED;
		continueButton->state = GuiControlState::DISABLED;
		exitButton->state = GuiControlState::DISABLED;
		creditsButton->state = GuiControlState::DISABLED;
		settingsButton->state = GuiControlState::DISABLED;
	}
	else {
		startButton->state = GuiControlState::NORMAL;
		continueButton->state = GuiControlState::NORMAL;
		exitButton->state = GuiControlState::NORMAL;
		creditsButton->state = GuiControlState::NORMAL;
		settingsButton->state = GuiControlState::NORMAL;
	}

	return true;
}

bool SceneMenu::Update(float dt)
{
	if (app->pauseMenus->isPaused()) { return true; }

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
		startButton->Draw(app->render);
		continueButton->Draw(app->render);
		exitButton->Draw(app->render);
		creditsButton->Draw(app->render);
		settingsButton->Draw(app->render);
	}

	if (quit) { return false; }

	return ret;
}

bool SceneMenu::OnGuiMouseClickEvent(GuiControl* control) {
	if (appStart) { return true; }

	switch (control->type)
	{
	case GuiControlType::BUTTON:
		if (control->id == startButton->id && startButton->canClick == true) {
			app->fade->FadeToBlack(this, (Module*)app->sceneLevel1, 30);
		}
		else if (control->id == continueButton->id && continueButton->canClick == true) {
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
				else { app->fade->FadeToBlack(this, (Module*)app->sceneLevel1, 30); }
			}
		}
		else if (control->id == exitButton->id && exitButton->canClick == true) {
			quit = true;
		}
		else if (control->id == creditsButton->id && creditsButton->canClick == true) {
			app->pauseMenus->CreditsMenu();
		}
		else if (control->id == settingsButton->id && settingsButton->canClick == true) {
			app->pauseMenus->SettingsMenu();
		}
		break;
	case GuiControlType::TOGGLE:
		break;
	case GuiControlType::CHECKBOX:
		break;
	case GuiControlType::SLIDER:
		break;
	case GuiControlType::SLIDERBAR:
		break;
	case GuiControlType::COMBOBOX:
		break;
	case GuiControlType::DROPDOWNBOX:
		break;
	case GuiControlType::INPUTBOX:
		break;
	case GuiControlType::VALUEBOX:
		break;
	case GuiControlType::SPINNER:
		break;
	default:
		break;
	}

	return true;
}

bool SceneMenu::CleanUp()
{
	LOG("Deleting background assets");

	app->tex->Unload(menuTexture);

	startButton->state = GuiControlState::DISABLED;
	continueButton->state = GuiControlState::DISABLED;
	exitButton->state = GuiControlState::DISABLED;
	creditsButton->state = GuiControlState::DISABLED;
	settingsButton->state = GuiControlState::DISABLED;

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