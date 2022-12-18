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

	// running Right
	flyingRightAnim.PushBack({ 109 * 11, 71 * 2 - 2, 109, 71 });
	flyingRightAnim.PushBack({ 109 * 9, 71 * 2 + 2, 109, 71 });
	flyingRightAnim.PushBack({ 109 * 10, 71 * 2, 109, 71 });
	flyingRightAnim.PushBack({ 109 * 9, 71 * 2 + 2, 109, 71 });
	flyingRightAnim.PushBack({ 109 * 11, 71 * 2 - 2, 109, 71 });
	flyingRightAnim.PushBack({ 109 * 10, 71 * 2, 109, 71 });
	flyingRightAnim.loop = true;
	flyingRightAnim.speed = 0.28f;

	// running Left
	flyingLeftAnim.PushBack({ 109 * 5, 71 * 2 - 2, 109, 71 });
	flyingLeftAnim.PushBack({ 109 * 3, 71 * 2 + 2, 109, 71 });
	flyingLeftAnim.PushBack({ 109 * 4, 71 * 2, 109, 71 });
	flyingLeftAnim.PushBack({ 109 * 3, 71 * 2 + 2, 109, 71 });
	flyingLeftAnim.PushBack({ 109 * 5, 71 * 2 - 2, 109, 71 });
	flyingLeftAnim.PushBack({ 109 * 4, 71 * 2, 109, 71 });
	flyingLeftAnim.loop = true;
	flyingLeftAnim.speed = 0.28f;

	// idle right
	idleRightAnim.PushBack({ 109 * 10, 0, 109, 71 });
	idleRightAnim.PushBack({ 109 * 11, 0, 109, 71 });
	idleRightAnim.PushBack({ 109 * 6, 71, 109, 71 });
	idleRightAnim.PushBack({ 109 * 11, 0, 109, 71 });
	idleRightAnim.loop = true;
	idleRightAnim.speed = 0.12f;

	// idle left
	idleLeftAnim.PushBack({ 109 * 4, 0, 109, 71 });
	idleLeftAnim.PushBack({ 109 * 5, 0, 109, 71 });
	idleLeftAnim.PushBack({ 0, 71, 109, 71 });
	idleLeftAnim.PushBack({ 109 * 5, 0, 109, 71 });
	idleLeftAnim.loop = true;

	// attack right
	attackRightAnim.PushBack({ 109 * 6, 71 * 2, 109, 71 });
	attackRightAnim.PushBack({ 109 * 7, 71 * 2, 109, 71 });
	attackRightAnim.PushBack({ 109 * 8, 71 * 2, 109, 71 });
	attackRightAnim.loop = false;
	attackRightAnim.speed = 0.3f;

	// attack left
	attackLeftAnim.PushBack({ 0, 71 * 2, 109, 71 });
	attackLeftAnim.PushBack({ 109, 71 * 2, 109, 71 });
	attackLeftAnim.PushBack({ 109 * 2, 71 * 2, 109, 71 });
	attackLeftAnim.loop = false;
	attackLeftAnim.speed = 0.3f;
	idleLeftAnim.speed = 0.12f;

	// Die right
	dieRightAnim.PushBack({ 109 * 5, 71, 109, 71 });
	dieRightAnim.PushBack({ 109 * 5, 71, 109, 71 });
	dieRightAnim.PushBack({ 109 * 5, 71, 109, 71 });
	dieRightAnim.PushBack({ 0, 71 * 3, 109, 71 });
	dieRightAnim.loop = false;
	dieRightAnim.speed = 0.12f;

	// Die left
	dieLeftAnim.PushBack({ 109 * 11, 71, 109, 71 });
	dieLeftAnim.PushBack({ 109 * 11, 71, 109, 71 });
	dieLeftAnim.PushBack({ 109 * 11, 71, 109, 71 });
	dieLeftAnim.PushBack({ 0, 71 * 3, 109, 71 });
	dieLeftAnim.loop = false;
	dieLeftAnim.speed = 0.12f;

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
	pbody = app->physics->CreateRectangle(position.x + 16, position.y + 16, 32, 50, bodyType::DYNAMIC);
	//pbody = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	pbody->body->SetFixedRotation(1);
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
