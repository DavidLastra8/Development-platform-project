#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

enum ItemType {
	// ...
	POTION,
	// ...
};

class Item : public Entity
{
public:

	Item();
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void SetPosition(int x, int y);

public:

	bool isPicked = false;

private:

	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;
};

#endif // __ITEM_H__