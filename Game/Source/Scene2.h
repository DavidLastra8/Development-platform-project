#pragma once
#ifndef __SCENE2_H__
#define __SCENE2_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "Coin.h"
#include "Physics.h"
#include "Enemy.h"
#include "FlyingEnemy.h"

struct SDL_Texture;

class Scene2 : public Module
{
public:

	Scene2();

	// Destructor
	virtual ~Scene2();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool CameraLock = true;
	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();



public:
	Player* player;
	Enemy* enemy;
	Enemy* enemy2;
	FlyEnemy* FlyingEnemy;
	FlyEnemy* FlyingEnemy2;
	Item* Potion;
	Coin* Coin;

private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	SDL_Texture* mouseTileTex = nullptr;


};

#endif // __SCENE2_H__