#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "SceneLevel1.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

#define FACING_LEFT false
#define FACING_RIGHT true
#define IDLE_STATIC 1
#define IDLE 2
#define RUNNING 3
#define JUMPING 4
#define FALLING 5
#define CHARGING 6
#define DYING 7
#define INITCHARGING 8
#define ENDCHARGING 9
#define WINING 10


Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

	// idle static left
	idlestaticleftanim.PushBack({ 0, 0, 109, 71 });
	idlestaticleftanim.PushBack({ 109, 0, 109, 71 });
	idlestaticleftanim.PushBack({ 109*2, 0, 109, 71 });
	idlestaticleftanim.PushBack({ 109, 0, 109, 71 });
	idlestaticleftanim.loop = true;
	idlestaticleftanim.speed = 0.16f;

	// static to dynamic
	statictodynamicleftanim.PushBack({ 0, 0, 109, 71 });
	statictodynamicleftanim.PushBack({ 109, 0, 109, 71 });
	statictodynamicleftanim.PushBack({ 109 * 2, 0, 109, 71 });
	statictodynamicleftanim.PushBack({ 109, 0, 109, 71 });
	statictodynamicleftanim.loop = true;
	statictodynamicleftanim.speed = 0.10f;

	// running Right 
	/*runningRight.PushBack({109 * 9, 71 * 2 + 2, 109, 71});
	runningRight.PushBack({ 109 * 10, 71 * 2, 109, 71 });
	runningRight.PushBack({ 109 * 11, 71 * 2 - 2, 109, 71 });
	runningRight.PushBack({ 109 * 10, 71 * 2, 109, 71 });
	runningRight.loop = true;
	runningRight.speed = 0.20f;*/

	runningRightAnim.PushBack({ 109 * 11, 71 * 2 - 2, 109, 71 });
	runningRightAnim.PushBack({ 109 * 9, 71 * 2 + 2, 109, 71 });
	runningRightAnim.PushBack({ 109 * 10, 71 * 2, 109, 71 });
	runningRightAnim.PushBack({ 109 * 9, 71 * 2 + 2, 109, 71 });
	runningRightAnim.PushBack({ 109 * 11, 71 * 2 - 2, 109, 71 });
	runningRightAnim.PushBack({ 109 * 10, 71 * 2, 109, 71 });
	runningRightAnim.loop = true;
	runningRightAnim.speed = 0.28f;

	// running Left
	/*runningLeft.PushBack({109 * 5, 71 * 2 - 2, 109, 71});
	runningLeft.PushBack({ 109 * 4, 71 * 2, 109, 71 });
	runningLeft.PushBack({ 109 * 3, 71 * 2 + 2, 109, 71 });
	runningLeft.PushBack({ 109 * 4, 71 * 2, 109, 71 });
	runningLeft.loop = true;
	runningLeft.speed = 0.20f;*/

	runningLeftAnim.PushBack({ 109 * 5, 71 * 2 - 2, 109, 71 });
	runningLeftAnim.PushBack({ 109 * 3, 71 * 2 + 2, 109, 71 });
	runningLeftAnim.PushBack({ 109 * 4, 71 * 2, 109, 71 });
	runningLeftAnim.PushBack({ 109 * 3, 71 * 2 + 2, 109, 71 });
	runningLeftAnim.PushBack({ 109 * 5, 71 * 2 - 2, 109, 71 });
	runningLeftAnim.PushBack({ 109 * 4, 71 * 2, 109, 71 });
	runningLeftAnim.loop = true;
	runningLeftAnim.speed = 0.28f;

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

	// Jump Right
	jumpRightAnim.PushBack({ 109 * 11, 71*4, 109, 71 });
	jumpRightAnim.PushBack({ 109 * 6, 71*5, 109, 71 });
	jumpRightAnim.loop = true;
	jumpRightAnim.speed = 0.07f;

	// Jump Left
	jumpLeftAnim.PushBack({ 109 * 5, 71 * 4, 109, 71 });
	jumpLeftAnim.PushBack({ 0, 71 * 5, 109, 71 });
	jumpLeftAnim.loop = true;
	jumpLeftAnim.speed = 0.07f;

	// Fall Right
	fallRightAnim.PushBack({ 109 * 7, 71 * 5, 109, 71 });
	fallRightAnim.PushBack({ 109 * 8, 71 * 5, 109, 71 });
	fallRightAnim.loop = true;
	fallRightAnim.speed = 0.07f;

	// Fall Left
	fallLeftAnim.PushBack({ 109, 71 * 5, 109, 71 });
	fallLeftAnim.PushBack({ 109 * 2, 71 * 5, 109, 71 });
	fallLeftAnim.loop = true;
	fallLeftAnim.speed = 0.07f;

	// init charge
	initChargeAnim.PushBack({ 0, 71 * 3, 109, 71 });
	initChargeAnim.PushBack({ 109, 71 * 3, 109, 71 });
	initChargeAnim.PushBack({ 109 * 2, 71 * 3, 109, 71 });
	initChargeAnim.PushBack({ 109 * 3, 71 * 3, 109, 71 });
	initChargeAnim.PushBack({ 109 * 4, 71 * 3, 109, 71 });
	initChargeAnim.PushBack({ 109 * 5, 71 * 3, 109, 71 });
	initChargeAnim.loop = false;
	initChargeAnim.speed = 0.2f;

	// charge
	chargeAnim.PushBack({ 0, 71 * 4, 109, 71 });
	chargeAnim.PushBack({ 109, 71 * 4, 109, 71 });
	chargeAnim.loop = true;
	chargeAnim.speed = 0.2f;

	// end charge
	endChargeAnim.PushBack({ 109 * 2, 71 * 4, 109, 71 });
	endChargeAnim.PushBack({ 109 * 3, 71 * 4, 109, 71 });
	endChargeAnim.PushBack({ 109 * 4, 71 * 4, 109, 71 });
	endChargeAnim.loop = false;
	endChargeAnim.speed = 0.2f;

}

Player::~Player() {

}

bool Player::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";

	//L02: DONE 5: Get Player parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	//pbody = app->physics->CreateRectangle(position.x+16, position.y+16, 32, 50, bodyType::DYNAMIC);
	pbody = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this; 

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	currentAnim = &idlestaticleftanim;
	facing = FACING_RIGHT;
	dieLeftAnim.Reset();
	dieRightAnim.Reset();
	winLoseTimer = 0;
	endLevel = 0;
	remainJumps = 0;
	isCharging = false;

	return true;
}

bool Player::Update()
{
	// L07 DONE 5: Add physics to the player - updated player position using physics
	b2Vec2 vel;
	float speed = 100.0f; 
	vel = pbody->body->GetLinearVelocity() + b2Vec2(0, -GRAVITY_Y * 0.05f); 


	if (godMode) {
		speed = 300.f;
		vel = b2Vec2(0, 0);
		pbody->body->SetGravityScale(0);
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			//vel.x = speed;
			b2Vec2 force = { 0, speed };
			pbody->body->ApplyForceToCenter(force, true);
			if (vel.y > 5)
			{
				vel.y = 5;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			//vel.x = speed;
			b2Vec2 force = { 0, -speed / 2 };
			pbody->body->ApplyForceToCenter(force, true);
			if (vel.y > -5)
			{
				vel.y = -5;
			}
		}
	}
	if (!godMode) { pbody->body->SetGravityScale(1); }

	//PLAYER MOVEMENT
	if ((state != DYING) && (state != WINING))
	{
		if (state != ENDCHARGING)
		{
			if ((state != JUMPING) && (state != FALLING) && (state != CHARGING))
			{
				state = IDLE;
				canJump = true;
			}

			//L02: DONE 4: modify the position of the player using arrow keys and render the texture
			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
			{
				vel.x = 0;
				chargeTimer++;
				isCharging = true;
				if (chargeTimer < 40)
				{
					state = INITCHARGING;
				}
				else if (chargeTimer <= 59)
				{
					state = CHARGING;
				}
				else if (chargeTimer > 60)
				{
					chargeTimer = 59;
					state = CHARGING;
				}
			}
			else if ((app->input->GetKey(SDL_SCANCODE_E)) == KEY_IDLE && (state == CHARGING))
			{
					state = ENDCHARGING;
			}
			else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

				b2Vec2 force = { -speed, 0 };
				//el.x = -speed;
				pbody->body->ApplyForceToCenter(force, true);
				if (vel.x < -4)
				{
					vel.x = -4;
				}
				facing = FACING_LEFT;
				if ((state != JUMPING) && (state != FALLING))
				{
					state = RUNNING;
					canJump = true;
					remainJumps = 2;
				}
			}
			else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
				//vel.x = speed;
				b2Vec2 force = { speed, 0 };
				pbody->body->ApplyForceToCenter(force, true);
				if (vel.x > 4)
				{
					vel.x = 4;
				}

				facing = FACING_RIGHT;
				if ((state != JUMPING) && (state != FALLING))
				{
					state = RUNNING;
					canJump = true;
					remainJumps = 2;
				}
			}
			else if ((app->input->GetKey(SDL_SCANCODE_E) == KEY_IDLE) && (state != JUMPING) && (state != FALLING))
			{
				state = IDLE;
				initChargeAnim.Reset();
				endChargeAnim.Reset();
				chargeTimer = 0;
				vel.x = 0;
				remainJumps = 2;
			}

			if (((app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)) && ((canJump == true) || (remainJumps > 0))) {
				vel.y = -7.5f;
				/*float impulse = pbody->body->GetMass() * 10;
				pbody->body->ApplyLinearImpulse(b2Vec2(0, -impulse), pbody->body->GetWorldCenter(), true);*/
				state = JUMPING;
				remainJumps--;
				canJump = false;
			}
		}
		else
		{
			chargeTimer++;
			if (chargeTimer > 80)
			{
				chargeTimer = 0;
				isCharging = false;
				state = IDLE;
			}
		}
	}
	
	//DEATH SEQUENCE
	if (state == DYING)
	{
		winLoseTimer++;
		if ((state == DYING) && (winLoseTimer > 30))
		{
			endLevel = true;
		}
	}

	if (state == WINING)
	{
		vel.x = 0;
		winLoseTimer++;
		if ((state == WINING) && (winLoseTimer > 30))
		{
			endLevel = true;
		}
	}

	pbody->body->SetLinearVelocity(vel);

	//Set the velocity of the pbody of the player

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;


	//ANIMATION STATE MACHINE
	switch(state)
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
	case RUNNING:
		if (facing == FACING_LEFT)
		{
			currentAnim = &runningLeftAnim;
		}
		else if(facing == FACING_RIGHT)
		{
			currentAnim = &runningRightAnim;
		}
		break;
	case JUMPING:
		if (facing == FACING_LEFT)
		{
			currentAnim = &jumpLeftAnim;
		}
		else if (facing == FACING_RIGHT)
		{
			currentAnim = &jumpRightAnim;
		}
		break;
	case FALLING:
		if (facing == FACING_LEFT)
		{
			currentAnim = &fallLeftAnim;
		}
		else if (facing == FACING_RIGHT)
		{
			currentAnim = &fallRightAnim;
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
	case INITCHARGING:
			currentAnim = &initChargeAnim;
		break;
	case CHARGING:
		currentAnim = &chargeAnim;
		break;
	case ENDCHARGING:
		currentAnim = &endChargeAnim;
		break;
	default:
		break;

	}

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x -35, position.y-27, &rect);
	currentAnim->Update();

	return true;
}

bool Player::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{
		case ColliderType::ITEM:
			LOG("Collision ITEM");
			app->audio->PlayFx(pickCoinFxId);
			break;
		case ColliderType::PLATFORM:
			LOG("Collision PLATFORM");
			if ((state != DYING) && (state != WINING))
			{
				state = IDLE;
				canJump = true;
			}
			break;
		case ColliderType::DEATH:
			LOG("Collision DEATH");
			if (!godMode) {
				if (state != DYING) { app->audio->PlayFx(app->audio->executedFx); }
				state = DYING;
			}
			break;
		case ColliderType::WIN:
			LOG("Collision WIN");
			state = WINING;
			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
	}
	


}

void Player::EndCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("END Collision ITEM");
		break;
	case ColliderType::PLATFORM:
		LOG("END Collision PLATFORM");
		canJump = false;
	    if ((app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) && (state != DYING))
			{
				state = JUMPING;
			}
		else if (state != DYING)
			{
				state = FALLING;
			}
		
		
		break;
	case ColliderType::UNKNOWN:
		LOG("END Collision UNKNOWN");
		break;
	}
}

PhysBody* Player::getpBody()
{
	return pbody;
}
