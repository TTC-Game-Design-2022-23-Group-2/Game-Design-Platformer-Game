#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

	// idle left
	idlestaticleftanim.PushBack({ 0, 0, 109, 71 });
	idlestaticleftanim.PushBack({ 109, 0, 109, 71 });
	idlestaticleftanim.PushBack({ 109*2, 0, 109, 71 });
	idlestaticleftanim.PushBack({ 109, 0, 109, 71 });
	idlestaticleftanim.loop = true;
	idlestaticleftanim.speed = 0.10f;

	// idle left
	statictodynamicleftanim.PushBack({ 0, 0, 109, 71 });
	statictodynamicleftanim.PushBack({ 109, 0, 109, 71 });
	statictodynamicleftanim.PushBack({ 109 * 2, 0, 109, 71 });
	statictodynamicleftanim.PushBack({ 109, 0, 109, 71 });
	statictodynamicleftanim.loop = true;
	statictodynamicleftanim.speed = 0.10f;

	// idle left
	idleLeftAnim.PushBack({ 0, 0, 109, 71 });
	idleLeftAnim.PushBack({ 109, 0, 109, 71 });
	idleLeftAnim.PushBack({ 109 * 2, 0, 109, 71 });
	idleLeftAnim.PushBack({ 109, 0, 109, 71 });
	idleLeftAnim.loop = true;
	idleLeftAnim.speed = 0.10f;

	// idle left
	idleLeftAnim.PushBack({ 0, 0, 109, 71 });
	idleLeftAnim.PushBack({ 109, 0, 109, 71 });
	idleLeftAnim.PushBack({ 109 * 2, 0, 109, 71 });
	idleLeftAnim.PushBack({ 109, 0, 109, 71 });
	idleLeftAnim.loop = true;
	idleLeftAnim.speed = 0.10f;
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
	pbody = app->physics->CreateRectangle(position.x+16, position.y+16, 16, 50, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this; 

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

bool Player::Update()
{
	currentAnim = &idleLeftAnim;
	// L07 DONE 5: Add physics to the player - updated player position using physics

	int speed = 5; 
	b2Vec2 vel = pbody->body->GetLinearVelocity(); 

	//L02: DONE 4: modify the position of the player using arrow keys and render the texture
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		//
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		//
	}
		
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		vel.x = -speed;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		vel.x = speed;
	}
	else { vel.x = 0; }

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		float impulse = pbody->body->GetMass() * 10;
		pbody->body->ApplyLinearImpulse(b2Vec2(0, -impulse), pbody->body->GetWorldCenter(), true);
	}
	else{ pbody->body->SetLinearVelocity(vel); }

	//Set the velocity of the pbody of the player

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

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
			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
	}
	


}
