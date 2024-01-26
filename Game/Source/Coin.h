#ifndef __COIN_H__
#define __COIN_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Coin : public Entity
{
public:

	Coin();
	virtual ~Coin();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void SetPosition(int x, int y);

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void TeleportToNewLocation(int newX, int newY);

public:

	bool isPicked = false;
	int pickCoinFxId;

	int newX = 5;
	int newY = 0;

private:

	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;
};

#endif // __ITEM_H__