#include "Coin.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Player.h"

Coin::Coin() : Entity(EntityType::COIN)
{
	name.Create("coin");

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
	texture = app->tex->Load("Assets/Textures/coin.png");
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 1, bodyType::STATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::COIN;

	return true;
}

bool Coin::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x-150, position.y-70);

	return true;
}

bool Coin::CleanUp()
{
	return true;
}

void Coin::SetPosition(int x, int y) {
	position.x = x;
	position.y = y;
	b2Vec2 newPos(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	pbody->body->SetTransform(newPos, pbody->body->GetAngle());
}

void Coin::TeleportToNewLocation(int newX, int newY) {
	// Set the new position for rendering
	position.x = newX;
	position.y = newY;

	// Convert the new position to physics world coordinates (if necessary)
	b2Vec2 newPos(PIXEL_TO_METERS(newX), PIXEL_TO_METERS(newY));

	// Teleport the physics body to the new position
	pbody->body->SetTransform(newPos, pbody->body->GetAngle());
}


void Coin::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	
}