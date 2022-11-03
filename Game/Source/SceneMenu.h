#pragma once
#ifndef __SCENE_MENU_H__
#define __SCENE_MENU_H__

#include "Module.h"
#include "Animation.h"

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

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt);

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();

	bool CleanUp();

public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* modebgTexture = nullptr;
};

#endif