#include "Boss.h"
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
#include "Animation.h"
#include "EntityManager.h"
#include "Player.h"
#include <chrono>
#include "SDL/include/SDL.h"
Boss::Boss() : Entity(EntityType::BOSS)
{
	name.Create("Boss");
	lifeCount = 3;
}

Boss::~Boss()
{

}

void Boss::InitAnims()
{

}

bool Boss::Awake()
{

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Boss::Start()
{

	//initilize textures
	texture = app->tex->Load(texturePath);
	pathTest = app->tex->Load("Assets/Textures/testPathTile.png");

	enemyCollider = app->physics->CreateCircle(position.x + 16, position.y + 16, 40, bodyType::DYNAMIC);
	enemyCollider->listener = this;
	enemyCollider->ctype = ColliderType::BOSS;

	enemyCollider->body->SetGravityScale(0);

	return true;
}

void Boss::DecreaseLives(int lifeCount)
{
	lifeCount--;
	if (lifeCount <= 0) {
		Deactivate();
		app->entityManager->DestroyEntity(this);
		
	}
}



bool Boss::Update(float dt)
{
	position.x = METERS_TO_PIXELS(enemyCollider->body->GetTransform().p.x) - 23;
	position.y = METERS_TO_PIXELS(enemyCollider->body->GetTransform().p.y) - 35;
	app->render->DrawTexture(texture, position.x - 30, position.y - 30);

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

	

	return true;
}

bool Boss::CleanUp()
{
	
	return true;
}

void Boss::OnCollision(PhysBody* physA, PhysBody* physB)
{
	/*using namespace std::chrono;
	steady_clock::time_point now = steady_clock::now();
	std::chrono::steady_clock::time_point lastAttackTime;*/

	Player* player = (Player*)physB->listener;
	if (physB->ctype == ColliderType::PLAYER)
	{

		LOG("Collision PLAYER - BOSS");
		if (!player->GodMode)
		{
			// Get the current time in milliseconds
			Uint32 now = SDL_GetTicks();

			// Calculate the time difference in milliseconds
			/*auto duration = duration_cast<milliseconds>(now - lastAttackTime).count();*/
			if (now - lastAttackTime > ATTACK_COOLDOWN_MS) {
				//if the player position in y is greater than the enemy's
				if (physB->body->GetLinearVelocity().y >= 0.5)
				{
					LOG("PLAYER ATTACKED BOSS");
					DecreaseLives(lifeCount);
					app->audio->PlayFx(pickCoinFxId);
					app->entityManager->DestroyEntity(this);
				}
				/*else if (physB->body->GetLinearVelocity().y < 0.5)
				{
					LOG("BOSS ATTACKED PLAYER");
					player->DecreaseLives(player->lifeCount);
					app->audio->PlayFx(pickCoinFxId);
					lastDamageTime = now;
				}*/
			}

			
		}

	}
}

void Boss::SetPosition(int x, int y) {
	position.x = x;
	position.y = y;
	b2Vec2 newPos(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	enemyCollider->body->SetTransform(newPos, enemyCollider->body->GetAngle());
}

void Boss::Deactivate()
{
	this->Disable();
	app->entityManager->DestroyEntity(this);
	enemyCollider->body->SetActive(false);
}
