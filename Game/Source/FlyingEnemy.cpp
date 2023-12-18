#include "FlyingEnemy.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Window.h"
#include "Pathfinding.h"
#include "Map.h"

FlyEnemy::FlyEnemy() : Entity(EntityType::FLYING_ENEMY)
{
	name.Create("Flyenemy");
}

FlyEnemy::~FlyEnemy()
{

}

void FlyEnemy::InitAnims()
{

}

bool FlyEnemy::Awake()
{

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool FlyEnemy::Start()
{

	//initilize textures
	texture = app->tex->Load(texturePath);
	pathTest = app->tex->Load("Assets/Textures/testPathTile.png");

	bound.x = position.x - 120;
	bound.y = position.y - 60;
	bound.w = 240;
	bound.h = 120;

	int enemy[8] = {
		0, 0,
		24, 0,
		24, 20,
		0, 20,
	};

	enemyCollider = app->physics->CreateCircle(position.x + 10, position.y + 15, 6, bodyType::DYNAMIC);
	enemyCollider->listener = this;
	enemyCollider->ctype = ColliderType::ENEMY;

	enemyCollider->body->SetGravityScale(0);

	return true;
}

bool FlyEnemy::Update(float dt)
{
	// Activate or deactivate debug mode
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	if (app->scene->player->GetTilex() >= bound.x
		&& app->scene->player->GetTilex() <= bound.x + bound.w
		&& app->scene->player->GetTiley() >= bound.y
		&& app->scene->player->GetTiley() <= bound.y + bound.h)
	{
		iPoint enemyPos = app->map->WorldToMap(position.x + 8, position.y + 16);
		iPoint playerPos = app->map->WorldToMap(app->scene->player->GetTilex() + 8, app->scene->player->GetTiley());

		app->map->pathfinding->CreatePath(enemyPos, playerPos);

		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();

		if (debug) {
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				app->render->DrawTexture(pathTest, pos.x, pos.y);
			}
		}


		if (path->Count() > 1 && app->map->pathfinding->CreatePath(enemyPos, playerPos) != -1) {

			iPoint pos = app->map->MapToWorld(path->At(1)->x, path->At(1)->y);

			triX = position.x - pos.x;
			triY = position.y - pos.y;

			enemyCollider->body->SetLinearVelocity(b2Vec2(-(triX / 10), -(triY / 10)));

			if (abs(enemyPos.x - playerPos.x) < 1) {
				enemyCollider->body->SetLinearVelocity(b2Vec2(0, 0));
				enemyCollider->body->SetLinearDamping(0);
			}
		}
	}

	position.x = METERS_TO_PIXELS(enemyCollider->body->GetTransform().p.x - 8);
	position.y = METERS_TO_PIXELS(enemyCollider->body->GetTransform().p.y - 8);

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

}