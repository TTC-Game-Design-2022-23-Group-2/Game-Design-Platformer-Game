#include "TerrestreEnemyBig.h"
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


TerrestreEnemyBig::TerrestreEnemyBig() : Entity(EntityType::PLAYER)
{
	name.Create("TerrestreEnemyBig");

	// idle right
	idleRightAnim.PushBack({ 80 * 0, 60 * 0, 80, 60 });
	idleRightAnim.PushBack({ 80 * 1, 60 * 1, 80, 60 });
	idleRightAnim.PushBack({ 80 * 2, 60 * 2, 80, 60 });
	idleRightAnim.PushBack({ 80 * 3, 60 * 3, 80, 60 });
	idleRightAnim.loop = false;
	idleRightAnim.speed = 0.1f;

	// idle left
	idleLeftAnim.PushBack({ 80 * 0, 60 * 0, 80, 60 });
	idleLeftAnim.PushBack({ 80 * 1, 60 * 1, 80, 60 });
	idleLeftAnim.PushBack({ 80 * 2, 60 * 2, 80, 60 });
	idleLeftAnim.PushBack({ 80 * 3, 60 * 3, 80, 60 });
	idleLeftAnim.loop = false;
	idleLeftAnim.speed = 0.1f;

	// static to dynamic Left
	statictodynamicleftanim.PushBack({ 80 * 0, 60 * 0, 80, 60 });
	statictodynamicleftanim.PushBack({ 80 * 1, 60 * 1, 80, 60 });
	statictodynamicleftanim.PushBack({ 80 * 2, 60 * 2, 80, 60 });
	statictodynamicleftanim.PushBack({ 80 * 3, 60 * 3, 80, 60 });
	statictodynamicleftanim.loop = false;
	statictodynamicleftanim.speed = 0.1f;

	// static to dynamic Right
	statictodynamicleftanim.PushBack({ 80 * 0, 60 * 0, 80, 60 });
	statictodynamicleftanim.PushBack({ 80 * 1, 60 * 1, 80, 60 });
	statictodynamicleftanim.PushBack({ 80 * 2, 60 * 2, 80, 60 });
	statictodynamicleftanim.PushBack({ 80 * 3, 60 * 3, 80, 60 });
	statictodynamicleftanim.loop = false;
	statictodynamicleftanim.speed = 0.1f;

	// running Right
	runningRightAnim.PushBack({ 80 * 0, 60 * 0, 80, 60 });
	runningRightAnim.PushBack({ 80 * 1, 60 * 1, 80, 60 });
	runningRightAnim.PushBack({ 80 * 2, 60 * 2, 80, 60 });
	runningRightAnim.PushBack({ 80 * 3, 60 * 3, 80, 60 });
	runningRightAnim.loop = false;
	runningRightAnim.speed = 0.1f;

	// running Left
	runningLeftAnim.PushBack({ 80 * 0, 60 * 0, 80, 60 });
	runningLeftAnim.PushBack({ 80 * 1, 60 * 1, 80, 60 });
	runningLeftAnim.PushBack({ 80 * 2, 60 * 2, 80, 60 });
	runningLeftAnim.PushBack({ 80 * 3, 60 * 3, 80, 60 });
	runningLeftAnim.loop = false;
	runningLeftAnim.speed = 0.1f;

	// attack right
	attackRightAnim.PushBack({ 80 * 0, 60 * 0, 80, 60 });
	attackRightAnim.PushBack({ 80 * 1, 60 * 1, 80, 60 });
	attackRightAnim.PushBack({ 80 * 2, 60 * 2, 80, 60 });
	attackRightAnim.PushBack({ 80 * 3, 60 * 3, 80, 60 });
	attackRightAnim.loop = false;
	attackRightAnim.speed = 0.1f;

	// attack left
	attackLeftAnim.PushBack({ 80 * 0, 60 * 0, 80, 60 });
	attackLeftAnim.PushBack({ 80 * 1, 60 * 1, 80, 60 });
	attackLeftAnim.PushBack({ 80 * 2, 60 * 2, 80, 60 });
	attackLeftAnim.PushBack({ 80 * 3, 60 * 3, 80, 60 });
	attackLeftAnim.loop = false;
	attackLeftAnim.speed = 0.1f;

	//// Die right
	//dieRightAnim.PushBack({ 80 * 0, 60 * 0, 80, 60 });
	//dieRightAnim.PushBack({ 80 * 1, 60 * 1, 80, 60 });
	//dieRightAnim.PushBack({ 80 * 2, 60 * 2, 80, 60 });
	//dieRightAnim.PushBack({ 80 * 3, 60 * 3, 80, 60 });
	//dieRightAnim.loop = false;
	//dieRightAnim.speed = 0.1f;

	//// Die left
	//dieLeftAnim.PushBack({ 80 * 0, 60 * 0, 80, 60 });
	//dieLeftAnim.PushBack({ 80 * 1, 60 * 1, 80, 60 });
	//dieLeftAnim.PushBack({ 80 * 2, 60 * 2, 80, 60 });
	//dieLeftAnim.PushBack({ 80 * 3, 60 * 3, 80, 60 });
	//dieLeftAnim.loop = false;
	//dieLeftAnim.speed = 0.1f;

}

TerrestreEnemyBig::~TerrestreEnemyBig() {

}

bool TerrestreEnemyBig::Awake() {

	//L02: DONE 1: Initialize TerrestreEnemyBig parameters
	//L02: DONE 5: Get TerrestreEnemyBig parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool TerrestreEnemyBig::Start() {

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

bool TerrestreEnemyBig::Update()
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

bool TerrestreEnemyBig::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void TerrestreEnemyBig::OnCollision(PhysBody* physA, PhysBody* physB) {

	isColliding = true;

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}



}

void TerrestreEnemyBig::EndCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::UNKNOWN:
		LOG("END Collision UNKNOWN");
		break;
	}
	isColliding = false;
}

PhysBody* TerrestreEnemyBig::getpBody()
{
	return pbody;
}
