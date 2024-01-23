#ifndef __COIN_H__
#define __COIN_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;


class Coin : public Entity {
public:
    Coin();
    virtual ~Coin();

    bool Awake();
    bool Start();
    bool Update(float dt);
    bool CleanUp();

    void Collected(); // Call this method when the coin is collected

private:
    bool collected;
    Point<int> position; // Assuming a Point class exists for position
};

#endif // __COIN_H__
