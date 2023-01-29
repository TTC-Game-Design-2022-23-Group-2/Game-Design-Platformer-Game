#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "List.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void EndCollision(PhysBody* physA, PhysBody* physB);

	int GetState() const { return state; }

	PhysBody* getpBody();


public:

	Animation* currentAnim = nullptr;

	Animation idlestaticleftanim;
	Animation statictodynamicleftanim;
	Animation idleRightAnim;
	Animation idleLeftAnim;
	Animation runningRightAnim;
	Animation runningLeftAnim;
	Animation dieRightAnim;
	Animation dieLeftAnim;
	Animation jumpRightAnim;
	Animation jumpLeftAnim;
	Animation fallRightAnim;
	Animation fallLeftAnim;
	Animation initChargeAnim;
	Animation chargeAnim;
	Animation endChargeAnim;
	Animation attackRightAnim;
	Animation attackLeftAnim;
	Animation specialRightAnim;
	Animation specialLeftAnim;


	bool endLevel;
	bool endLevelDie;
	bool godMode = false;

private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
	List<PhysBody*> attackCollisions;

	int winLoseTimer;
	int state;
	int pickCoinFxId;
	bool facing;
	bool canJump;
	int remainJumps;
	int chargeTimer;
	bool isCharging;
	bool isColliding;
	int specialCooldown;

};

#endif // __PLAYER_H__