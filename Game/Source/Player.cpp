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
}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

bool Player::Update(float dt)
{
	
	/*vely = -GRAVITY_Y;
	velx = 0;*/
	b2Vec2 currentVel = pbody->body->GetLinearVelocity();

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		//
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		//
	}

	// Horizontal movement
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		currentVel.x = -8.0f; // Leftward
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		currentVel.x = 8.0f; // Rightward
	}
	else {
		currentVel.x = 0.0f; // Stop horizontal movement when no keys are pressed
	}

	// Apply the updated horizontal velocity
	pbody->body->SetLinearVelocity(b2Vec2(currentVel.x, pbody->body->GetLinearVelocity().y));

	// Jumping
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !IsJumping) {
		b2Vec2 jumpImpulse(0.0f, -4.3f); // Upward
		pbody->body->ApplyLinearImpulse(jumpImpulse, pbody->body->GetWorldCenter(), true);
		IsJumping = true;
	}

	//we don't want this for now, Instead of directly setting the linear velocity for movement, you can apply forces or impulses in the horizontal direction as well. This will allow both jumping and lateral movement to coexist.

	


	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		if (IsJumping) {
			IsJumping = false;
		}
		break;

	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}