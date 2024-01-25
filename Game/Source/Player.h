#ifndef __PLAYER_H__
#define __PLAYER_H__
//include chrono for time
#include <chrono>
#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include <SDL_image/include/SDL_image.h>
#include "Module.h"
#include "EntityManager.h"

struct SDL_Texture;

enum Direction { LEFT, RIGHT, IDLE };


class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();
	
	bool IsJumping = false;

	bool Grounded = true;

	bool GodMode = false;

	bool Awake();

	bool Start();

	int GetTilex();

	int GetTiley();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void SetPosition(int x, int y);

	bool isAlive = true;

	void IncreaseLives(int amount);

	void DestroyEntity(Entity* entity);

public:
	
	float speed = 0.2f;
	const char* texturePath;
	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* texture = NULL;
	SDL_Texture* mouseTileTex = nullptr;
	PhysBody* pbody;
	int pickCoinFxId;

	unsigned int jumpFxId; // ID for the jumping sound effect
	unsigned int endLevelFxId;  // ID for the end-level sound effect
	unsigned int deathFxId; // ID for the death sound effect
	bool endLevelSoundPlayed;   // Flag to check if the sound has been played
	int endLevelX = 3939;              // X position where the player has to reach to end the level	
	int endLevelY = 1420;              // Y position where the player has to reach to end the level

	float velx;
	float gravity;
	float vely;
	int lives = 6;
	int coinCount;
	bool isOnPause = false;

	

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation = nullptr;
	// A set of animations
	Animation idleAnim;
	/*Animation leftAnim;
	Animation rightAnim;*/
	Animation jumpAnim;
	Animation death;
	// Additional animations
	Animation idleLeftAnim;
	Animation walkLeftAnim;
	Animation walkRightAnim;
	Animation fallingLeftAnim;
	Animation fallingRightAnim;

	Direction lastDirection = IDLE;

private:

	
	

	std::chrono::steady_clock::time_point lastDamageTime;  // Time of last damage
	

};

#endif // __PLAYER_H__