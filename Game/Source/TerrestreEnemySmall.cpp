#include "TerrestreEnemySmall.h"
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


TerrestreEnemySmall::TerrestreEnemySmall() : Entity(EntityType::PLAYER)
{
	name.Create("TerrestreEnemySmall");

	// idle right
	idleRightAnim.PushBack({ 50 * 4, 40 * 0, 50, 40 });
	idleRightAnim.PushBack({ 50 * 5, 40 * 0, 50, 40 });
	idleRightAnim.PushBack({ 50 * 6, 40 * 0, 50, 40 });
	//idleRightAnim.PushBack({ 50 * 3, 40 * 0, 50, 40 });
	idleRightAnim.loop = false;
	idleRightAnim.speed = 0.1f;

	// idle left
	idleLeftAnim.PushBack({ 50 * 0, 40 * 0, 50, 40 });
	idleLeftAnim.PushBack({ 50 * 1, 40 * 0, 50, 40 });
	idleLeftAnim.PushBack({ 50 * 2, 40 * 0, 50, 40 });
	//idleLeftAnim.PushBack({ 50 * 3, 40 * 0, 50, 40 });
	idleLeftAnim.loop = false;
	idleLeftAnim.speed = 0.1f;

	// static to dynamic Left
	statictodynamicleftanim.PushBack({ 50 * 0, 40 * 0, 50, 40 });
	statictodynamicleftanim.PushBack({ 50 * 1, 40 * 0, 50, 40 });
	statictodynamicleftanim.PushBack({ 50 * 2, 40 * 0, 50, 40 });
	statictodynamicleftanim.PushBack({ 50 * 3, 40 * 0, 50, 40 });
	statictodynamicleftanim.loop = false;
	statictodynamicleftanim.speed = 0.1f;

	// static to dynamic Right
	statictodynamicleftanim.PushBack({ 50 * 0, 40 * 0, 50, 40 });
	statictodynamicleftanim.PushBack({ 50 * 1, 40 * 0, 50, 40 });
	statictodynamicleftanim.PushBack({ 50 * 2, 40 * 0, 50, 40 });
	statictodynamicleftanim.PushBack({ 50 * 3, 40 * 0, 50, 40 });
	statictodynamicleftanim.loop = false;
	statictodynamicleftanim.speed = 0.1f;

	// running Right
	runningRightAnim.PushBack({ 50 * 4, 40 * 1, 50, 40 });
	runningRightAnim.PushBack({ 50 * 7, 40 * 1, 50, 40 });
	runningRightAnim.PushBack({ 50 * 6, 40 * 1, 50, 40 });
	runningRightAnim.PushBack({ 50 * 5, 40 * 1, 50, 40 });
	runningRightAnim.loop = false;
	runningRightAnim.speed = 0.1f;

	// running Left
	runningLeftAnim.PushBack({ 50 * 0, 40 * 1, 50, 40 });
	runningLeftAnim.PushBack({ 50 * 3, 40 * 1, 50, 40 });
	runningLeftAnim.PushBack({ 50 * 2, 40 * 1, 50, 40 });
	runningLeftAnim.PushBack({ 50 * 1, 40 * 1, 50, 40 });
	runningLeftAnim.loop = false;
	runningLeftAnim.speed = 0.1f;

	// attack right	
	attackRightAnim.PushBack({ 50 * 4, 40 * 2, 50, 40 });
	attackRightAnim.PushBack({ 50 * 5, 40 * 2, 50, 40 });
	attackRightAnim.PushBack({ 50 * 6, 40 * 2, 50, 40 });
	attackRightAnim.PushBack({ 50 * 7, 40 * 2, 50, 40 });
	attackRightAnim.PushBack({ 50 * 4, 40 * 3, 50, 40 });
	attackRightAnim.loop = false;
	attackRightAnim.speed = 0.1f;

	// attack left 
	attackLeftAnim.PushBack({ 50 * 0, 40 * 2, 50, 40 });
	attackLeftAnim.PushBack({ 50 * 1, 40 * 2, 50, 40 });
	attackLeftAnim.PushBack({ 50 * 2, 40 * 2, 50, 40 });
	attackLeftAnim.PushBack({ 50 * 3, 40 * 2, 50, 40 });
	attackLeftAnim.PushBack({ 50 * 0, 40 * 3, 50, 40 });
	attackLeftAnim.loop = false;
	attackLeftAnim.speed = 0.1f;

	//// Die right
	//dieRightAnim.PushBack({ 50 * 0, 40 * 0, 50, 40 });
	//dieRightAnim.PushBack({ 50 * 1, 40 * 0, 50, 40 });
	//dieRightAnim.PushBack({ 50 * 2, 40 * 0, 50, 40 });
	//dieRightAnim.PushBack({ 50 * 3, 40 * 0, 50, 40 });
	//dieRightAnim.loop = false;
	//dieRightAnim.speed = 0.1f;

	//// Die left
	//dieLeftAnim.PushBack({ 50 * 0, 40 * 0, 50, 40 });
	//dieLeftAnim.PushBack({ 50 * 1, 40 * 0, 50, 40 });
	//dieLeftAnim.PushBack({ 50 * 2, 40 * 0, 50, 40 });
	//dieLeftAnim.PushBack({ 50 * 3, 40 * 0, 50, 40 });
	//dieLeftAnim.loop = false;
	//dieLeftAnim.speed = 0.1f;

}

TerrestreEnemySmall::~TerrestreEnemySmall() {

}

bool TerrestreEnemySmall::Awake() {

	//L02: DONE 1: Initialize TerrestreEnemySmall parameters
	//L02: DONE 5: Get TerrestreEnemySmall parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool TerrestreEnemySmall::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateRectangle(position.x+16, position.y+16, 32, 50, bodyType::DYNAMIC);
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

bool TerrestreEnemySmall::Update()
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
			currentAnim = &runningLeftAnim;
		}
		else if (facing == FACING_RIGHT)
		{
			currentAnim = &runningRightAnim;
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
			//currentAnim = &dieLeftAnim;
		}
		else if (facing == FACING_RIGHT)
		{
			//currentAnim = &dieRightAnim;
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

bool TerrestreEnemySmall::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void TerrestreEnemySmall::OnCollision(PhysBody* physA, PhysBody* physB) {

	isColliding = true;

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}



}

void TerrestreEnemySmall::EndCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::UNKNOWN:
		LOG("END Collision UNKNOWN");
		break;
	}
	isColliding = false;
}

PhysBody* TerrestreEnemySmall::getpBody()
{
	return pbody;
}
