#include "Win_Screen.h"

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

WinScreen::WinScreen() : Module()
{

}

WinScreen::~WinScreen()
{

}

// Load assets
bool WinScreen::Start()
{
	app->win->GetWindowSize(windowW, windowH);
	intro = app->tex->Load("Assets/Textures/Win_Screen.png");

	bool ret = true;

	SDL_Rect ExitButton3 = { windowW / 2 - 60,windowH / 2 + 240, 240, 80 };
	exit3 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Exit", ExitButton3, this);

	app->render->camera.x = 0;
	app->render->camera.y = 0;
	return ret;
}

bool WinScreen::Update(float dt)
{



	app->render->DrawTexture(intro, 0, 0, false);

	return true;
}

bool WinScreen::PostUpdate()
{
	// Draw everything --------------------------------------
	bool ret = true;

	if (hasToExit == true)
		ret = false;

	return ret;

}

bool WinScreen::OnGuiMouseClickEvent(GuiControl* control) {
	bool ret = true;
	if (control->id == 2) {
		hasToExit = true;
	}
	return ret;
}