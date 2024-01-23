#ifndef __INITIAL__SCREEN_H__
#define __INITIAL__SCREEN_H__

#include "../Module.h"
#include "../Animation.h"

struct SDL_Texture;

class InitialScreen : public Module {

public:
	InitialScreen();

	~InitialScreen();

	bool Start() override;

	bool Update(float dt);

	bool PostUpdate() override;

public:
	// The scene sprite sheet loaded into an SDL_Texture

	SDL_Texture* intro;

	int frame = 0;
};
#endif
