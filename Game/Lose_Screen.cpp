#include "Lose_Screen.h"

#include "../App.h"
#include "../Textures.h"
#include "../Render.h"
#include "../Audio.h"
#include "../Input.h"
#include "../Map.h"
#include "../Scene.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "../Window.h"

LoseScreen::LoseScreen() : Module()
{

}

LoseScreen::~LoseScreen()
{

}

// Load assets
bool LoseScreen::Start()
{
	app->win->GetWindowSize(windowW, windowH);
	intro = app->tex->Load("Assets/Textures/Lose_Screen.png");

	bool ret = true;

	SDL_Rect ExitButton2 = { windowW / 2 - 450,windowH / 2 + 240, 240, 80 };
	exit2 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Exit", ExitButton2, this);

	app->render->camera.x = 0;
	app->render->camera.y = 0;
	return ret;
}

bool LoseScreen::Update(float dt)
{



	app->render->DrawTexture(intro, 0, 0, false);

	return true;
}

bool LoseScreen::PostUpdate()
{
	// Draw everything --------------------------------------
	bool ret = true;

	if (hasToExit == true)
		ret = false;

	return ret;

}

bool LoseScreen::OnGuiMouseClickEvent(GuiControl* control) {
	bool ret = true;
	if (control->id == 2) {
		hasToExit = true;
	}
	return ret;
}