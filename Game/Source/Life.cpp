#include "Life.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "SceneLevel1.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Life::Life() : Entity(EntityType::LIFE)
{
	name.Create("life");

	lifeAnim.PushBack({ 0 * 22,0 * 22,22,22 });
	lifeAnim.PushBack({ 1 * 22,0 * 22,22,22 });
	lifeAnim.loop = true;
	lifeAnim.speed = 0.1;
}

Life::~Life() {}

bool Life::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Life::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	
	// L07 DONE 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircle(position.x + 11, position.y + 11, 11, bodyType::STATIC, true);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::LIFE;

	return true;
}

bool Life::Update()
{
	if (!pbody->body->IsActive())
	{
		pbody->body->DestroyFixture(pbody->body->GetFixtureList());
		active = false;
	}

	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 11;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 11;

	SDL_Rect rect = lifeAnim.GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);
	lifeAnim.Update();

	return true;
}

bool Life::CleanUp()
{
	return true;
}