#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Physics.h"

struct SDL_Texture;

class Enemy : public Entity
{
public:

	Enemy();

	virtual ~Enemy();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	//L02: DONE 2: Declare player parameters
	float speed = 0.2f;
	SDL_Texture* texture = NULL;
	SDL_Texture* pathTexture = NULL;
	pugi::xml_node config;
	uint texW, texH;
	PhysBody* groundEnemy;
	//Audio fx
	int pickCoinFxId;

};

#endif // __ENEMY_H__();