#ifndef __INITIAL__SCREEN_H__
#define __INITIAL__SCREEN_H__

#include "../Module.h"
#include "../Animation.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"

struct SDL_Texture;

class InitialScreen : public Module {

public:
	InitialScreen();

	~InitialScreen();

	bool Start() override;

	bool Update(float dt);

	bool PostUpdate() override;

	bool OnGuiMouseClickEvent(GuiControl* control);
public:
	// The scene sprite sheet loaded into an SDL_Texture

	SDL_Texture* intro;
	

	int frame = 0;

private:
	uint windowW;
	uint windowH;
	uint textW;
	uint textH;
	GuiControlButton* start;
};
#endif
