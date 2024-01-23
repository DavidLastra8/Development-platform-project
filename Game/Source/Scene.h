#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "Coin.h"
#include "Physics.h"
#include "Enemy.h"
#include "FlyingEnemy.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

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

	// Declare the LoadState method
	bool LoadState(pugi::xml_node node) override;
	bool SaveState(pugi::xml_node node) override;

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

#endif // __SCENE_H__