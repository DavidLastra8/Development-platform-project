#ifndef __WIN__SCREEN_H__
#define __WIN__SCREEN_H__

#include "../Module.h"
#include "../Animation.h"
#include "GuiControlButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "GuiManager.h"
#include "GuiControl.h"

struct SDL_Texture;

class WinScreen : public Module {

public:
	WinScreen();

	~WinScreen();

	bool Start() override;

	bool Update(float dt);

	bool PostUpdate() override;

	bool OnGuiMouseClickEvent(GuiControl* control);
public:
	// The scene sprite sheet loaded into an SDL_Texture

	SDL_Texture* intro;


	int frame = 0;
	GuiControlButton* exit3;

private:
	uint windowW;
	uint windowH;
	bool hasToExit = false;

};
#endif