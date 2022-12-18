#ifndef __TERRESTREENEMYSMALL_H__
#define __TERRESTREENEMYSMALL_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "List.h"

struct SDL_Texture;

class TerrestreEnemySmall : public Entity
{
public:

	TerrestreEnemySmall();

	virtual ~TerrestreEnemySmall();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void EndCollision(PhysBody* physA, PhysBody* physB);

	int GetState() const { return state; }

	PhysBody* getpBody();


public:

	Animation* currentAnim = nullptr;

	Animation idleRightAnim;
	Animation idleLeftAnim;
	Animation statictodynamicLeftAnim;
	Animation statictodynamicRightAnim;
	Animation runningRightAnim;
	Animation attackRightAnim;
	Animation attackLeftAnim;
	Animation runningLeftAnim;
	Animation dieRightAnim;
	Animation dieLeftAnim;

	iPoint objective;

private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
	List<PhysBody*> attackCollisions;

	int state;
	bool facing;
	int attackTimer;
	bool isColliding;
	bool alive;
};

#endif // __TERRESTREENEMYSMALL_H__