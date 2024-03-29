#pragma once
#ifndef __BOSS_H__
#define __BOSS_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
//include chrono for time
#include <chrono>

struct SDL_Texture;

class Boss : public Entity
{
public:

	Boss();

	~Boss();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void InitAnims();

	void SetPosition(int x, int y);

	void DecreaseLives(int lifeCount);

	void Deactivate();

	

	Animation* currentAnim;

	Animation Idle;

public:
	int pickCoinFxId;
	const int ATTACK_COOLDOWN_MS = 3000; // 3000 milliseconds (3 seconds)
	int lastAttackTime = 0;
	unsigned int deathFxId;

	const char* texturePath;

	SDL_Texture* texture = NULL;
	SDL_Texture* pathTest = NULL;

	int counterPath = 0;
	int timer = 0;

	float triX, triY;

	PhysBody* enemyCollider;

	SDL_Rect bound;

	bool debug = false;
	int lifeCount;

	Animation* currentAnimation = nullptr;
	// A set of animations
	Animation idleAnim;
	

private:
	
};

#endif // __BOSS_H__
