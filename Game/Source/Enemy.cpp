#include "Enemy.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Enemy::Enemy() : Entity(EntityType::ENEMY)
{
	name.Create("Player");
}

Enemy::~Enemy() {

}

bool Enemy::Awake() {

	//L03: DONE 2: Initialize Player parameters
	position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	return true;
}

bool Enemy::Start() {

	/*texture = app->tex->Load(config.attribute("texturePath").as_string());
	pbody = app->physics->CreateCircle(position.x,position.y, 16, DYNAMIC);
	pbody->ctype = ColliderType::PLAYER;
	pbody->body->SetGravityScale(0);*/
	//initialize audio effect
	pickCoinFxId = app->audio->LoadFx(config.attribute("coinfxpath").as_string());


	return true;
}

bool Enemy::Update(float dt)
{
	//L03: DONE 4: render the player texture and modify the position of the player using WSAD keys and render the texture

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		position.x += -0.2 * dt;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		position.x += 0.2 * dt;
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		position.y += -0.2 * dt;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		position.y += 0.2 * dt;
	}

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Enemy::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}