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
#include "Map.h"
#include "Player.h"

Enemy::Enemy() : Entity(EntityType::ENEMY)
{
	name.Create("Player");


}

Enemy::~Enemy() {

}

bool Enemy::Awake() {

	//L03: DONE 2: Initialize Player parameters
	position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());
	texture = app->tex->Load(config.attribute("texturePath").as_string());

	
	/*pbody->body->SetGravityScale(0);*/

	pathTexture = app->tex->Load("Assets/Maps/TileSelection.png");

	return true;
}

bool Enemy::Start() {

	/*texture = app->tex->Load(config.attribute("texturePath").as_string());
	pbody = app->physics->CreateCircle(position.x,position.y, 16, DYNAMIC);
	pbody->ctype = ColliderType::PLAYER;
	pbody->body->SetGravityScale(0);*/
	//initialize audio effect

	// Set the initial position
	position.x = 500; // Replace with your desired initial X coordinate
	position.y = 1100; // Replace with your desired initial Y coordinate

	groundEnemy = app->physics->CreateCircle(position.x, position.y, 16, DYNAMIC);
	groundEnemy->listener = this;
	groundEnemy->ctype = ColliderType::ENEMY;
	/*pbody->body->SetGravityScale(0);*/
	pickCoinFxId = app->audio->LoadFx(config.attribute("coinfxpath").as_string());



	return true;
}

bool Enemy::Update(float dt)
{
	// Render the enemy texture at its current position
	app->render->DrawTexture(texture, position.x, position.y);

	// Get enemy and player positions in map coordinates
	iPoint enemyPos = app->map->WorldToMap(position.x, position.y);
	iPoint playerPos = app->map->WorldToMap(app->scene->player->GetTilex(), app->scene->player->GetTiley());

	// Check if the player and enemy positions are different before creating a path
	if (enemyPos != playerPos) {
		// Create a path to the player
		if (app->map->pathfinding->CreatePath(enemyPos, playerPos) != -1) {
			const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();

			// Check if there is a next step in the path
			if (path->Count() > 1) {
				iPoint nextStep = *(path->At(1)); // Get the next step

				// Convert the next step from map coordinates to world coordinates
				iPoint nextPosition = app->map->MapToWorld(nextStep.x, nextStep.y);

				// Calculate the velocity needed to move towards the next step
				iPoint velocity = nextPosition - position;
				if (velocity.x != 0) velocity.x = (velocity.x > 0) ? 1 : -1;
				if (velocity.y != 0) velocity.y = (velocity.y > 0) ? 1 : -1;

				// Move the enemy towards the next step
				groundEnemy->body->SetLinearVelocity(b2Vec2(velocity.x * enemySpeed, velocity.y * enemySpeed));
			}
		}
	}

	// Update the enemy's position based on the physics body
	position.x = METERS_TO_PIXELS(groundEnemy->body->GetPosition().x);
	position.y = METERS_TO_PIXELS(groundEnemy->body->GetPosition().y);

	// Optionally visualize the path for debugging
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) { // Press F1 to display path
		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
		for (uint i = 0; i < path->Count(); i++) {
			iPoint pathPos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawTexture(pathTexture, pathPos.x, pathPos.y);
		}
	}

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