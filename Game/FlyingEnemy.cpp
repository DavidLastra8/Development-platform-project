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

	enemyCollider = app->physics->CreateCircle(position.x + 10, position.y + 15, 25, bodyType::DYNAMIC);
	enemyCollider->listener = this;
	enemyCollider->ctype = ColliderType::ENEMY;

	enemyCollider->body->SetGravityScale(0);

	return true;
}

bool FlyEnemy::Update(float dt)
{
	position.x = METERS_TO_PIXELS(enemyCollider->body->GetTransform().p.x) - 23;
	position.y = METERS_TO_PIXELS(enemyCollider->body->GetTransform().p.y) - 35;
	app->render->DrawTexture(texture, position.x - 25, position.y - 35);

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


	if (path->Count() > 1 && app->map->pathfinding->CreatePath(enemPos, PlayerPos) != -1) {

		iPoint pos = app->map->MapToWorld(path->At(1)->x, path->At(1)->y);

		triX = position.x - pos.x;
		triY = position.y - pos.y;

		enemyCollider->body->SetLinearVelocity(b2Vec2(-(triX / 3), -(triY / 3)));

		if (abs(enemPos.x - PlayerPos.x) < 1) {
			enemyCollider->body->SetLinearVelocity(b2Vec2(0, 0));
			enemyCollider->body->SetLinearDamping(0);
		}
	}

	app->render->DrawTexture(texture, position.x - 80, position.y - 60);

	return true;
}

bool FlyEnemy::CleanUp()
{

	return true;
}

void FlyEnemy::OnCollision(PhysBody* physA, PhysBody* physB)
{

}