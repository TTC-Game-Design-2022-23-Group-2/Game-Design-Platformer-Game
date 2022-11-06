#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"

struct SDL_Texture;

class SceneLevel1 : public Module
{
public:

	SceneLevel1(bool startEnabled);

	// Destructor
	virtual ~SceneLevel1();

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

	// Called before quitting
	bool CleanUp();

public:

	//L02: DONE 3: Declare a Player attribute 
	Player* player;
	bool godMode = false;
	bool playerDeath;
	const char* musicPath;

private:
	SDL_Texture* img;

};

#endif // __SCENE_H__