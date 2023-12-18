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
	name.Create("enemy");
}

Enemy::~Enemy() {

}

bool Enemy::Awake() {

	//L03: DONE 2: Initialize Player parameters
	position = iPoint(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());


	/*pbody->body->SetGravityScale(0);*/

	return true;
}

bool Enemy::Start() {

	SString tex = parameters.attribute("texturepath").as_string();
	texture = app->tex->Load(tex.GetString());

	/*texture = app->tex->Load(config.attribute("texturePath").as_string());
	pbody = app->physics->CreateCircle(position.x,position.y, 16, DYNAMIC);
	pbody->ctype = ColliderType::PLAYER;
	pbody->body->SetGravityScale(0);*/
	//initialize audio effect

	groundEnemy = app->physics->CreateCircle(position.x, position.y, 16, DYNAMIC);
	groundEnemy->ctype = ColliderType::ENEMY;
	/*pbody->body->SetGravityScale(0);*/
	pickCoinFxId = app->audio->LoadFx(config.attribute("coinfxpath").as_string());


	return true;
}

bool Enemy::Update(float dt)
{
	//L03: DONE 4: render the player texture and modify the position of the player using WSAD keys and render the texture

	//Update enemy position in pixels
	position.x = METERS_TO_PIXELS(groundEnemy->body->GetTransform().p.x) - 23;
	position.y = METERS_TO_PIXELS(groundEnemy->body->GetTransform().p.y) - 35;
	app->render->DrawTexture(texture, position.x, position.y);

	iPoint enemPos = app->map->WorldToMap(position.x, position.y);
	iPoint PlayerPos = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);
	app->map->pathfinding->CreatePath(enemPos, PlayerPos);

	const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); i++) {

		iPoint Pathpos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		//app->render->DrawTexture(pathTexture, Pathpos.x, Pathpos.y);
	}

	if (path->Count() > 1 && app->map->pathfinding->CreatePath(enemPos, PlayerPos) != -1) {

		if (enemPos.x - PlayerPos.x < 0 && abs(enemPos.x - PlayerPos.x)>1) {
			groundEnemy->body->SetLinearVelocity(b2Vec2(0.1 * dt, 0.2 * dt));
		}
		else if (abs(enemPos.x - PlayerPos.x) > 1) {
			groundEnemy->body->SetLinearVelocity(b2Vec2(-0.1 * dt, 0.2 * dt));
		}
		else if (abs(enemPos.x - PlayerPos.x) < 1) {
			groundEnemy->body->SetLinearVelocity(b2Vec2(0.1 * dt, 0.2 * dt));
			groundEnemy->body->SetLinearDamping(0);
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