#include "Coin.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "SceneLevel1.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Coin::Coin() : Entity(EntityType::COIN)
{
	name.Create("item");

	coinAnim.PushBack({ 0 * 14,0 * 14,14,14 });
	coinAnim.PushBack({ 1 * 14,0 * 14,14,14 });
	coinAnim.PushBack({ 2 * 14,0 * 14,14,14 });
	coinAnim.PushBack({ 3 * 14,0 * 14,14,14 });
	coinAnim.PushBack({ 0 * 14,1 * 14,14,14 });
	coinAnim.PushBack({ 1 * 14,1 * 14,14,14 });
	coinAnim.PushBack({ 2 * 14,1 * 14,14,14 });
	coinAnim.PushBack({ 3 * 14,1 * 14,14,14 });
	coinAnim.loop = true;
	coinAnim.speed = 0.1;
}

Coin::~Coin() {}

bool Coin::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Coin::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	
	// L07 DONE 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircle(position.x + 7, position.y + 7, 7, bodyType::STATIC, true);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::COIN;

	return true;
}

bool Coin::Update()
{
	if (!pbody->body->IsActive())
	{
		pbody->body->DestroyFixture(pbody->body->GetFixtureList());
		active = false;
	}

	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 7;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 7;

	SDL_Rect rect = coinAnim.GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);
	coinAnim.Update();

	return true;
}

bool Coin::CleanUp()
{
	return true;
}