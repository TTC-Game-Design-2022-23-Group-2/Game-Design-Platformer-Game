#ifndef __SCENELEVEL2__
#define __SCENELEVEL2__

#include "Module.h"
#include "Player.h"
#include "Coin.h"
#include "Life.h"
#include "Point.h"
#include "Physics.h"
#include "FlyingEnemy.h"

struct SDL_Texture;

class SceneLevel2 : public Module
{
public:

	SceneLevel2(bool startEnabled);

	// Destructor
	virtual ~SceneLevel2();

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
	List<FlyingEnemy*> flyingEnemies;
	List<Coin*> coins;
	List<Life*> lifes;

	//L02: DONE 3: Declare a Player attribute 
	Player* player;
	bool playerDeath;
	const char* musicPath;

	Animation victory;
	Animation defeat;

	SDL_Point death_pos;

private:
	SDL_Texture* victory_defeat;
	SDL_Texture* death_text;


	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;
	// L12: Debug pathfing
	iPoint origin;
	iPoint destination;
	bool originSelected = false;
};

#endif // __SCENELEVEL2__