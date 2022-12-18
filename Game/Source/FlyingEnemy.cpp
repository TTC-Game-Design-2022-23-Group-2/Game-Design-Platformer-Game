#include "FlyingEnemy.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "SceneLevel1.h"
#include "SceneLevel2.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

#define FACING_LEFT false
#define FACING_RIGHT true
#define IDLE 1
#define CHASING 2
#define CHARGING 3
#define ATTACKING 4
#define DYING 5


FlyingEnemy::FlyingEnemy() : Entity(EntityType::PLAYER)
{
	name.Create("FlyingEnemy");

	// idle right
	idleRightAnim.PushBack({ 64 * 3 + 512, 64 * 2, 64, 64 });
	idleRightAnim.PushBack({ 64 * 4 + 512, 64 * 2, 64, 64 });
	idleRightAnim.PushBack({ 64 * 5 + 512, 64 * 2, 64, 64 });
	idleRightAnim.PushBack({ 64 * 6 + 512, 64 * 2, 64, 64 });
	idleRightAnim.PushBack({ 64 * 7 + 512, 64 * 2, 64, 64 });
	idleRightAnim.loop = true;
	idleRightAnim.speed = 0.2f;

	// idle left
	idleLeftAnim.PushBack({ 64 * 3, 64 * 2, 64, 64 });
	idleLeftAnim.PushBack({ 64 * 4, 64 * 2, 64, 64 });
	idleLeftAnim.PushBack({ 64 * 5, 64 * 2, 64, 64 });
	idleLeftAnim.PushBack({ 64 * 6, 64 * 2, 64, 64 });
	idleLeftAnim.PushBack({ 64 * 7, 64 * 2, 64, 64 });
	idleLeftAnim.loop = false;
	idleLeftAnim.speed = 0.1f;

	// running Right
	flyingRightAnim.PushBack({ 64 * 0, 64 * 0, 64, 64 });
	flyingRightAnim.PushBack({ 64 * 1, 64 * 1, 64, 64 });
	flyingRightAnim.PushBack({ 64 * 2, 64 * 2, 64, 64 });
	flyingRightAnim.PushBack({ 64 * 3, 64 * 3, 64, 64 });
	flyingRightAnim.loop = false;
	flyingRightAnim.speed = 0.1f;

	// running Left
	flyingLeftAnim.PushBack({ 64 * 0, 64 * 0, 64, 64 });
	flyingLeftAnim.PushBack({ 64 * 1, 64 * 1, 64, 64 });
	flyingLeftAnim.PushBack({ 64 * 2, 64 * 2, 64, 64 });
	flyingLeftAnim.PushBack({ 64 * 3, 64 * 3, 64, 64 });
	flyingLeftAnim.loop = false;
	flyingLeftAnim.speed = 0.1f;

	// attack right
	attackRightAnim.PushBack({ 64 * 0, 64 * 0, 64, 64 });
	attackRightAnim.PushBack({ 64 * 1, 64 * 1, 64, 64 });
	attackRightAnim.PushBack({ 64 * 2, 64 * 2, 64, 64 });
	attackRightAnim.PushBack({ 64 * 3, 64 * 3, 64, 64 });
	attackRightAnim.loop = false;
	attackRightAnim.speed = 0.1f;

	// attack left
	attackLeftAnim.PushBack({ 64 * 0, 64 * 0, 64, 64 });
	attackLeftAnim.PushBack({ 64 * 1, 64 * 1, 64, 64 });
	attackLeftAnim.PushBack({ 64 * 2, 64 * 2, 64, 64 });
	attackLeftAnim.PushBack({ 64 * 3, 64 * 3, 64, 64 });
	attackLeftAnim.loop = false;
	attackLeftAnim.speed = 0.1f;

	// Die right
	dieRightAnim.PushBack({ 64 * 0, 64 * 0, 64, 64 });
	dieRightAnim.PushBack({ 64 * 1, 64 * 1, 64, 64 });
	dieRightAnim.PushBack({ 64 * 2, 64 * 2, 64, 64 });
	dieRightAnim.PushBack({ 64 * 3, 64 * 3, 64, 64 });
	dieRightAnim.loop = false;
	dieRightAnim.speed = 0.1f;

	// Die left
	dieLeftAnim.PushBack({ 64 * 0, 64 * 0, 64, 64 });
	dieLeftAnim.PushBack({ 64 * 1, 64 * 1, 64, 64 });
	dieLeftAnim.PushBack({ 64 * 2, 64 * 2, 64, 64 });
	dieLeftAnim.PushBack({ 64 * 3, 64 * 3, 64, 64 });
	dieLeftAnim.loop = false;
	dieLeftAnim.speed = 0.1f;

}

FlyingEnemy::~FlyingEnemy() {

}

bool FlyingEnemy::Awake() {

	//L02: DONE 1: Initialize FlyingEnemy parameters
	//L02: DONE 5: Get FlyingEnemy parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool FlyingEnemy::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	//pbody = app->physics->CreateRectangle(position.x + 16, position.y + 16, 32, 50, bodyType::DYNAMIC);
	pbody = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	pbody->body->SetFixedRotation(1);
	pbody->body->SetGravityScale(0);
	//app->map->enemies.Add(pbody);

	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	//pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	currentAnim = &idleRightAnim;
	facing = FACING_RIGHT;
	state = IDLE;
	alive = true;
	/*dieLeftAnim.Reset();
	dieRightAnim.Reset();*/

	return true;
}

bool FlyingEnemy::Update()
{
	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;


	//ANIMATION STATE MACHINE
	switch (state)
	{
	case IDLE:
		if (facing == FACING_LEFT)
		{
			currentAnim = &idleLeftAnim;
		}
		else if (facing == FACING_RIGHT)
		{
			currentAnim = &idleRightAnim;
		}
		break;
	case CHASING:
		if (facing == FACING_LEFT)
		{
			currentAnim = &flyingLeftAnim;
		}
		else if (facing == FACING_RIGHT)
		{
			currentAnim = &flyingRightAnim;
		}
	case ATTACKING:
		if (facing == FACING_LEFT)
		{
			currentAnim = &attackLeftAnim;
		}
		else if (facing == FACING_RIGHT)
		{
			currentAnim = &attackRightAnim;
		}
		break;
		break;
	case DYING:
		if (facing == FACING_LEFT)
		{
			currentAnim = &dieLeftAnim;
		}
		else if (facing == FACING_RIGHT)
		{
			currentAnim = &dieRightAnim;
		}
		break;
	default:
		break;

	}

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);
	currentAnim->Update();

	return true;
}

bool FlyingEnemy::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void FlyingEnemy::OnCollision(PhysBody* physA, PhysBody* physB) {

	isColliding = true;

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}



}

void FlyingEnemy::EndCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::UNKNOWN:
		LOG("END Collision UNKNOWN");
		break;
	}
	isColliding = false;
}

PhysBody* FlyingEnemy::getpBody()
{
	return pbody;
}
