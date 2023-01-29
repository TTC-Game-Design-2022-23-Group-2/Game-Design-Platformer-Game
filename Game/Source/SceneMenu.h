#pragma once
#ifndef __SCENE_MENU_H__
#define __SCENE_MENU_H__

#include "Module.h"
#include "Animation.h"
#include "GuiManager.h"

struct SDL_Texture;

class SceneMenu : public Module
{
public:
	//Constructor
	SceneMenu(bool startEnabled);

	//Destructor
	~SceneMenu();

	// Called when the module is activated
	bool Start();

	bool PreUpdate();

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt);

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();

	bool CleanUp();

	bool SaveState(pugi::xml_node& data);

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* menuTexture = nullptr;
	SDL_Texture* buttonTexture = nullptr;
	SDL_Texture* buttonTextureSelected = nullptr;
	Animation menu;

	//GUI Buttons
	GuiButton* startButton;
	GuiButton* continueButton;
	GuiButton* exitButton;
	GuiButton* creditsButton;
	GuiButton* settingsButton;

	int iconCounter = 240;
	bool appStart = true;

	int currentLevel;
	bool loadPrevious;
	bool quit = false;
};

#endif