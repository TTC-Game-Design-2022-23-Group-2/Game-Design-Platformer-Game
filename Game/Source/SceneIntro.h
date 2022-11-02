#ifndef __SCENE_INTRO_H__
#define __SCENE_INTRO_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class SceneIntro : public Module
{
public:
	//Constructor
	SceneIntro(bool startEnabled);

	//Destructor
	~SceneIntro();

	// Called when the module is activated
	bool Start();

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt);

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();

	bool SceneIntro::CleanUp();

public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bgTexture = nullptr;
	SDL_Texture* introTexture = nullptr;

	bool play_intro = false;
	int counter = 120;
};

#endif