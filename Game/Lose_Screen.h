#ifndef __LOSE__SCREEN_H__
#define __LOSE__SCREEN_H__

#include "../Module.h"
#include "../Animation.h"
#include "GuiControlButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "GuiManager.h"
#include "GuiControl.h"

struct SDL_Texture;

class LoseScreen : public Module {

public:
	LoseScreen();

	~LoseScreen();

	bool Start() override;

	bool Update(float dt);

	bool PostUpdate() override;

	bool OnGuiMouseClickEvent(GuiControl* control);
	public:
	// The scene sprite sheet loaded into an SDL_Texture

	SDL_Texture* intro;


	int frame = 0;
	GuiControlButton* exit2;

	private:
	uint windowW;
	uint windowH;
	bool hasToExit = false;
	
};
#endif
