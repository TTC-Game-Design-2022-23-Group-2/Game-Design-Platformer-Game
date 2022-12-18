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
#include "Map.h"

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
	idleLeftAnim.loop = true;
	idleLeftAnim.speed = 0.2f;

	// flying Right
	flyingRightAnim.PushBack({ 64 * 0 + 512, 64 * 3, 64, 64 });
	flyingRightAnim.PushBack({ 64 * 1 + 512, 64 * 3, 64, 64 });
	flyingRightAnim.PushBack({ 64 * 2 + 512, 64 * 3, 64, 64 });
	flyingRightAnim.PushBack({ 64 * 3 + 512, 64 * 3, 64, 64 });
	flyingRightAnim.loop = true;
	flyingRightAnim.speed = 0.2f;

	// flying Left
	flyingLeftAnim.PushBack({ 64 * 0, 64 * 3, 64, 64 });
	flyingLeftAnim.PushBack({ 64 * 1, 64 * 3, 64, 64 });
	flyingLeftAnim.PushBack({ 64 * 2, 64 * 3, 64, 64 });
	flyingLeftAnim.PushBack({ 64 * 3, 64 * 3, 64, 64 });
	flyingLeftAnim.loop = true;
	flyingLeftAnim.speed = 0.2f;

	// attack right
	attackRightAnim.PushBack({ 64 * 2 + 512, 64 * 5, 64, 64 });
	attackRightAnim.PushBack({ 64 * 3 + 512, 64 * 5, 64, 64 });
	attackRightAnim.PushBack({ 64 * 4 + 512, 64 * 5, 64, 64 });
	attackRightAnim.PushBack({ 64 * 5 + 512, 64 * 5, 64, 64 });
	attackRightAnim.PushBack({ 64 * 6 + 512, 64 * 5, 64, 64 });
	attackRightAnim.PushBack({ 64 * 7 + 512, 64 * 5, 64, 64 });
	attackRightAnim.PushBack({ 64 * 0 + 512, 64 * 6, 64, 64 });
	attackRightAnim.PushBack({ 64 * 1 + 512, 64 * 6, 64, 64 });
	attackRightAnim.PushBack({ 64 * 2 + 512, 64 * 6, 64, 64 });
	attackRightAnim.loop = false;
	attackRightAnim.speed = 0.2f;

	// attack left
	attackLeftAnim.PushBack({ 64 * 2, 64 * 5, 64, 64 });
	attackLeftAnim.PushBack({ 64 * 3, 64 * 5, 64, 64 });
	attackLeftAnim.PushBack({ 64 * 4, 64 * 5, 64, 64 });
	attackLeftAnim.PushBack({ 64 * 5, 64 * 5, 64, 64 });
	attackLeftAnim.PushBack({ 64 * 6, 64 * 5, 64, 64 });
	attackLeftAnim.PushBack({ 64 * 7, 64 * 5, 64, 64 });
	attackLeftAnim.PushBack({ 64 * 0, 64 * 6, 64, 64 });
	attackLeftAnim.PushBack({ 64 * 1, 64 * 6, 64, 64 });
	attackLeftAnim.PushBack({ 64 * 2, 64 * 6, 64, 64 });
	attackLeftAnim.loop = false;
	attackLeftAnim.speed = 0.2f;

	// Die right
	dieRightAnim.PushBack({ 64 * 7 + 512, 64 * 6, 64, 64 });
	dieRightAnim.PushBack({ 64 * 0 + 512, 64 * 7, 64, 64 });
	dieRightAnim.PushBack({ 64 * 1 + 512, 64 * 7, 64, 64 });
	dieRightAnim.PushBack({ 64 * 2 + 512, 64 * 7, 64, 64 });
	dieRightAnim.PushBack({ 64 * 3 + 512, 64 * 7, 64, 64 });
	dieRightAnim.PushBack({ 64 * 4 + 512, 64 * 7, 64, 64 });
	dieRightAnim.PushBack({ 64 * 5 + 512, 64 * 7, 64, 64 });
	dieRightAnim.PushBack({ 64 * 6 + 512, 64 * 7, 64, 64 });
	dieRightAnim.loop = false;
	dieRightAnim.speed = 0.2f;

	// Die left
	dieLeftAnim.PushBack({ 64 * 7, 64 * 6, 64, 64 });
	dieLeftAnim.PushBack({ 64 * 0, 64 * 7, 64, 64 });
	dieLeftAnim.PushBack({ 64 * 1, 64 * 7, 64, 64 });
	dieLeftAnim.PushBack({ 64 * 2, 64 * 7, 64, 64 });
	dieLeftAnim.PushBack({ 64 * 3, 64 * 7, 64, 64 });
	dieLeftAnim.PushBack({ 64 * 4, 64 * 7, 64, 64 });
	dieLeftAnim.PushBack({ 64 * 5, 64 * 7, 64, 64 });
	dieLeftAnim.PushBack({ 64 * 6, 64 * 7, 64, 64 });
	dieLeftAnim.loop = false;
	dieLeftAnim.speed = 0.2f;

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

	currentAnim = &idleLeftAnim;
	facing = FACING_LEFT;
	state = CHARGING;
	alive = true;
	dieLeftAnim.Reset();
	dieRightAnim.Reset();
	attackTimer = 90;

	return true;
}

bool FlyingEnemy::Update()
{
	b2Vec2 vel, force;
	float speed = 50.0f;
	vel = pbody->body->GetLinearVelocity();

	if (state != DYING)
	{
		switch (state)
		{
		case IDLE:
			vel.x = 0; vel.y = 0;
			if (position.DistanceTo(app->sceneLevel2->player->position) <= 200 && position.DistanceTo(app->sceneLevel2->player->position) > 40) {
				state = CHASING;
			}
			break;
		case CHASING:
			//movimiento
			if (abs(objective.x + app->map->mapData.tileWidth / 2 - pbody->body->GetPosition().x) <= 1)
			{
				facing = FACING_LEFT;
				vel.x = 0;
			}
			else if (objective.x + app->map->mapData.tileWidth / 2 <= METERS_TO_PIXELS(pbody->body->GetPosition().x)) {
				facing = FACING_LEFT;
				force = { -speed, 0 };
				pbody->body->ApplyForceToCenter(force, true);
				if (vel.x < -3) {
					vel.x = -3;
				}
			}
			else if (objective.x + app->map->mapData.tileWidth / 2 > METERS_TO_PIXELS(pbody->body->GetPosition().x)) {
				facing = FACING_RIGHT;
				force = { speed, 0 };
				pbody->body->ApplyForceToCenter(force, true);
				if (vel.x > 3) {
					vel.x = 3;
				}
			}

			if (abs(objective.y + app->map->mapData.tileHeight / 2 - pbody->body->GetPosition().y) <= 1)
			{
				facing = FACING_LEFT;
				vel.y = 0;
			}
			else if (objective.y + app->map->mapData.tileHeight / 2 <= METERS_TO_PIXELS(pbody->body->GetPosition().y)) {
				
				force = { 0, -speed };
				pbody->body->ApplyForceToCenter(force, true);
				if (vel.y < -3) {
					vel.y = -3;
				}
			}
			else if (objective.y + app->map->mapData.tileHeight / 2 > METERS_TO_PIXELS(pbody->body->GetPosition().y)) {
				
				force = { 0, speed };
				pbody->body->ApplyForceToCenter(force, true);
				if (vel.y > 3) {
					vel.y = 3;
				}

			}

			if (position.DistanceTo(app->sceneLevel2->player->position) <= 40) {
				state = CHARGING;
			}
			else if (position.DistanceTo(app->sceneLevel2->player->position) > 200) {
				state = IDLE;
			}
			break;
		case CHARGING:
			vel.x = 0; vel.y = 0;
			if (attackTimer>50) {
				attackTimer--;
			}
			else{ state = ATTACKING; }
			break;
		case ATTACKING:
			if (attackTimer > 0) {
				attackTimer--;
			}
			else if (position.DistanceTo(app->sceneLevel2->player->position) > 200) {
				state = IDLE;
				attackTimer = 90;
				attackLeftAnim.Reset();
				attackRightAnim.Reset();
			}
			else {
				state = CHASING;
				attackTimer = 90;
				attackLeftAnim.Reset();
				attackRightAnim.Reset();
			}
			break;
		default:
			break;
		}
	}
	else {
		pbody->body->SetActive(false);
		attackTimer--;
		if ((state == DYING) && (attackTimer < 0))
		{
			pbody->body->DestroyFixture(pbody->body->GetFixtureList());
			active = false;
		}
	}

	pbody->body->SetLinearVelocity(vel);

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
		break;
	case CHARGING:
		if (facing == FACING_LEFT)
		{
			currentAnim = &idleLeftAnim;
		}
		else if (facing == FACING_RIGHT)
		{
			currentAnim = &idleRightAnim;
		}
		break;
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
	app->render->DrawTexture(texture, position.x - 16, position.y - 16, &rect);
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
		LOG("ENEMY Collision UNKNOWN");
		break;
	case ColliderType::ATTACK:
		LOG("ENEMY Collision ATTACK");
		app->audio->PlayFx(app->audio->enemy_deathFx);
		state = DYING;
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
