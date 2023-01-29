#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "PauseMenus.h"
#include "SceneLevel1.h"
#include "SceneLevel2.h"
#include "SceneMenu.h"
#include "ModuleFadeToBlack.h"

#include "Defs.h"
#include "Log.h"

PauseMenus::PauseMenus(bool startEnabled) : Module(startEnabled)
{
	name.Create("pauseMenus");
}

// Destructor
PauseMenus::~PauseMenus()
{}

// Called before render is available
bool PauseMenus::Awake(pugi::xml_node& config)
{
	return true;
}

// Called before the first frame
bool PauseMenus::Start()
{
	LOG("Loading SceneLevel1");
	bool ret = true;

	pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child(name.GetString());

	backgroundMenu_text = app->tex->Load(config.child("texturepaths").attribute("background").as_string());
	credits_text = app->tex->Load(config.child("texturepaths").attribute("credits").as_string());
	buttons_text = app->tex->Load(config.child("texturepaths").attribute("buttons").as_string());
	buttonsSelected_text = app->tex->Load(config.child("texturepaths").attribute("buttonsselected").as_string());

	//GUI Buttons

	resumeButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, { 134,116,243,70 }, this, { 0,0,243,70 }, buttons_text, buttonsSelected_text, {});
	menuButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, { 134,198,243,70 }, this, { 0,70,243,70 }, buttons_text, buttonsSelected_text, {});
	exitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 8, { 188,280,135,40 }, this, { 54,140,135,40 }, buttons_text, buttonsSelected_text, {});
	backButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, { 34,30,49,40 }, this, { 0,140,49,40 }, buttons_text, buttonsSelected_text, {});
	settingsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, { 328,280,49,40 }, this, { 194,140,49,40 }, buttons_text, buttonsSelected_text, {});

	resumeButton->state = GuiControlState::DISABLED;
	menuButton->state = GuiControlState::DISABLED;
	exitButton->state = GuiControlState::DISABLED;
	backButton->state = GuiControlState::DISABLED;
	settingsButton->state = GuiControlState::DISABLED;

	//Reset variables
	creditsMenu = false;
	settingsMenu = false;
	gameMenu = false;
	quit = false;

	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	if (result == NULL)
	{
		LOG("Could not load xml file savegame.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		app->pauseMenus->LoadState(gameStateFile.child("save_state").child(app->pauseMenus->name.GetString()));
	}

	return ret;
}

// Called each loop iteration
bool PauseMenus::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool PauseMenus::Update(float dt)
{
	if (creditsMenu) {
		
	}
	else if (gameMenu) {
		
	}
	else if (settingsMenu) {
		
	}
	else {
		
	}

	return true;
}

// Called each loop iteration
bool PauseMenus::PostUpdate()
{
	bool ret = true;

	if (creditsMenu) {
		//Draw Menus elements
		SDL_Rect rect = { 0,0,0,0 };
		app->render->DrawTexture(backgroundMenu_text, 46, 42, NULL, false);
		app->render->DrawTexture(credits_text, 46, 42, NULL, false);
		backButton->Draw(app->render);
	}
	else if (gameMenu) {
		//Draw Menus elements
		SDL_Rect rect = { 0,0,0,0 };
		app->render->DrawTexture(backgroundMenu_text, 46, 42, NULL, false);
		resumeButton->Draw(app->render);
		menuButton->Draw(app->render);
		exitButton->Draw(app->render);
		settingsButton->Draw(app->render);
	}
	else if (settingsMenu) {
		//Draw Menus elements
		SDL_Rect rect = { 0,0,0,0 };
		app->render->DrawTexture(backgroundMenu_text, 46, 42, NULL, false);
		backButton->Draw(app->render);
	}

	if (quit) { return false; }

	return ret;
}

bool PauseMenus::OnGuiMouseClickEvent(GuiControl* control) {

	switch (control->type)
	{
	case GuiControlType::BUTTON:
		if (control->id == resumeButton->id && resumeButton->canClick == true) {
			CloseMenus();
		}
		else if (control->id == menuButton->id && menuButton->canClick == true) {
			if (app->sceneLevel1->isEnabled) {
				app->fade->FadeToBlack((Module*)app->sceneLevel1, (Module*)app->sceneMenu, 30);
				CloseMenus();
			}
			else if (app->sceneLevel2->isEnabled) {
				app->fade->FadeToBlack((Module*)app->sceneLevel2, (Module*)app->sceneMenu, 30);
				CloseMenus();
			}
		}
		else if (control->id == exitButton->id && exitButton->canClick == true) {
			quit = true;
		}
		else if (control->id == backButton->id && backButton->canClick == true) {
			if (!app->sceneMenu->isEnabled) {
				GameMenu();
			}
			else { CloseMenus(); }
		}
		else if (control->id == settingsButton->id && settingsButton->canClick == true) {
			SettingsMenu();
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

// Called before quitting
bool PauseMenus::CleanUp()
{
	LOG("Freeing sceneGui");

	app->tex->Unload(backgroundMenu_text);
	app->tex->Unload(buttons_text);
	app->tex->Unload(buttonsSelected_text);

	return true;
}

bool PauseMenus::LoadState(pugi::xml_node& data)
{
	/*money = data.child("gamedata").attribute("money").as_int();
	lives = data.child("gamedata").attribute("lives").as_int();*/

	return true;
}

// L03: DONE 8: Create a method to save the state of the renderer
// using append_child and append_attribute
bool PauseMenus::SaveState(pugi::xml_node& data)
{
	/*pugi::xml_node guiNude = data.append_child("gamedata");

	guiNude.append_attribute("lives") = lives;
	guiNude.append_attribute("money") = money;*/

	return true;
}