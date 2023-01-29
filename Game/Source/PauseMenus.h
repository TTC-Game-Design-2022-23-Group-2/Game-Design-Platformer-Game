#ifndef __PAUSEMENUS_H__
#define __PAUSEMENUS_H__

#include "Module.h"
#include "Player.h"
#include "GuiManager.h"

struct SDL_Texture;

class PauseMenus : public Module
{
public:

	PauseMenus(bool startEnabled);

	// Destructor
	virtual ~PauseMenus();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	bool OnGuiMouseClickEvent(GuiControl* control);

	// Called before quitting
	bool CleanUp();

	bool LoadState(pugi::xml_node& data);
	bool SaveState(pugi::xml_node& data);

	void SettingsMenu() {
		creditsMenu = false;
		gameMenu = false;
		settingsMenu = true;
		pauseGame = true;
		resumeButton->state = GuiControlState::DISABLED;
		menuButton->state = GuiControlState::DISABLED;
		exitButton->state = GuiControlState::DISABLED;
		backButton->state = GuiControlState::NORMAL;
		settingsButton->state = GuiControlState::DISABLED;
	}
	void GameMenu() {
		creditsMenu = false;
		gameMenu = true;
		settingsMenu = false;
		pauseGame = true;
		resumeButton->state = GuiControlState::NORMAL;
		menuButton->state = GuiControlState::NORMAL;
		exitButton->state = GuiControlState::NORMAL;
		backButton->state = GuiControlState::DISABLED;
		settingsButton->state = GuiControlState::NORMAL;
	}
	void CreditsMenu() {
		creditsMenu = true;
		gameMenu = false;
		settingsMenu = false;
		pauseGame = true;
		resumeButton->state = GuiControlState::DISABLED;
		menuButton->state = GuiControlState::DISABLED;
		exitButton->state = GuiControlState::DISABLED;
		backButton->state = GuiControlState::NORMAL;
		settingsButton->state = GuiControlState::DISABLED;
	}
	void CloseMenus() {
		creditsMenu = false;
		gameMenu = false;
		settingsMenu = false;
		pauseGame = false;
		resumeButton->state = GuiControlState::DISABLED;
		menuButton->state = GuiControlState::DISABLED;
		exitButton->state = GuiControlState::DISABLED;
		backButton->state = GuiControlState::DISABLED;
		settingsButton->state = GuiControlState::DISABLED;
	}
	bool isPaused() { return pauseGame; }

public:


private:
	bool creditsMenu = false;
	bool settingsMenu = false;
	bool gameMenu = false;
	bool pauseGame = false;

	bool quit = false;

	SDL_Texture* backgroundMenu_text;
	SDL_Texture* credits_text;
	SDL_Texture* buttons_text;
	SDL_Texture* buttonsSelected_text;

	//GUI Buttons
	GuiButton* backButton;
	GuiButton* exitButton;
	GuiButton* settingsButton;
	GuiButton* resumeButton;
	GuiButton* menuButton;
};

#endif // __PAUSEMENUS_H__