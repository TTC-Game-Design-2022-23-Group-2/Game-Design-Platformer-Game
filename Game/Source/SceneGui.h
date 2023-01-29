#ifndef __SCENEGUI_H__
#define __SCENEGUI_H__

#include "Module.h"
#include "Player.h"

struct SDL_Texture;

class SceneGui : public Module
{
public:

	SceneGui(bool startEnabled);

	// Destructor
	virtual ~SceneGui();

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

	bool LoadState(pugi::xml_node& data);
	bool SaveState(pugi::xml_node& data);

public:
	int lives;
	int money;

	Animation money_anim;
	Animation lives_anim1;
	Animation lives_anim2;
	Animation lives_anim3;

private:
	SDL_Texture* lives_text;
	SDL_Texture* money_text;

	char moneyText[6] = { "\0" };
	int fontYellow_text;
};

#endif // __SCENEGUI_H__