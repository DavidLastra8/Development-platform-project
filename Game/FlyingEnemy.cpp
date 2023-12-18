#include "FlyingEnemy.h"
#include "../App.h"
#include "../Textures.h"
#include "../Audio.h"
#include "../Input.h"
#include "../Render.h"
#include "../Scene.h"
#include "../Log.h"
#include "../Point.h"
#include "../Physics.h"
#include "../Window.h"
#include "../Pathfinding.h"
#include "../Map.h"

FlyEnemy::FlyEnemy() : Entity(EntityType::FLYING_ENEMY)
{
	name.Create("FlyEnemy");
}

FlyEnemy::~FlyEnemy()
{

}

void FlyEnemy::InitAnims()
{

}

bool FlyEnemy::Awake()
{
	position = iPoint(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());

	

	return true;
}

bool FlyEnemy::Start()
{

	//initilize textures
	SString tex = parameters.attribute("texturepath").as_string();
	texture = app->tex->Load(tex.GetString());

	bound.x = position.x - 120;
	bound.y = position.y - 60;
	bound.w = 240;
	bound.h = 120;

	int enemy[8] = {
		0, 0,
		12, 0,
		12, 10,
		0, 10,
	};

	enemyFlying = app->physics->CreateCircle(position.x, position.y, 16, DYNAMIC);
	enemyFlying->ctype = ColliderType::ENEMY;
	enemyFlying->listener = this;
	

	enemyFlying->body->SetGravityScale(0);

	return true;
}

bool FlyEnemy::Update(float dt)
{
	position.x = METERS_TO_PIXELS(enemyFlying->body->GetTransform().p.x) - 23;
	position.y = METERS_TO_PIXELS(enemyFlying->body->GetTransform().p.y) - 35;
	app->render->DrawTexture(texture, position.x, position.y);

	// Activate or deactivate debug mode
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	if (app->scene->player->GetTilex() >= bound.x
		&& app->scene->player->GetTilex() <= bound.x + bound.w
		&& app->scene->player->GetTiley() >= bound.y
		&& app->scene->player->GetTiley() <= bound.y + bound.h)
	{
		iPoint enemPos = app->map->WorldToMap(position.x, position.y);
		iPoint PlayerPos = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);
		app->map->pathfinding->CreatePath(enemPos, PlayerPos);

		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();

		if (debug) {
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				app->render->DrawTexture(pathTest, pos.x, pos.y);
			}
		}


		if (path->Count() > 2 && app->map->pathfinding->CreatePath(enemPos, PlayerPos) != -1) {

			iPoint pos = app->map->MapToWorld(path->At(2)->x, path->At(2)->y);

			triX = position.x - pos.x;
			triY = position.y - pos.y;

			enemyFlying->body->SetLinearVelocity(b2Vec2(-(triX / 10), -(triY / 10)));

			if (abs(enemPos.x - PlayerPos.x) < 2) {
				enemyFlying->body->SetLinearVelocity(b2Vec2(0, 0));
				enemyFlying->body->SetLinearDamping(0);
			}
		}
	}

	position.x = METERS_TO_PIXELS(enemyFlying->body->GetTransform().p.x - 8);
	position.y = METERS_TO_PIXELS(enemyFlying->body->GetTransform().p.y - 8);

	bound.x = position.x - 120;
	bound.y = position.y - 60;
	bound.w = 240;
	bound.h = 120;

	if (debug) {
		app->render->DrawRectangle(bound, 0, 255, 0, 80);
	}

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool FlyEnemy::CleanUp()
{

	return true;
}

void FlyEnemy::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}