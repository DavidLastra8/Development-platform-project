#include "InitialScreen.h"

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


InitialScreen::InitialScreen() : Module()
{

}

InitialScreen::~InitialScreen()
{

}

// Load assets
bool InitialScreen::Start()
{
	app->win->GetWindowSize(windowW, windowH);
	intro = app->tex->Load("Assets/Textures/portada_development.png");
	app->entityManager->active = false;
	app->map->active = false;
	app->scene->active = false;

	bool ret = true;

	SDL_Rect StartButton = { windowW / 2-60,windowH / 2-120, 240,80 };
	start = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Start", StartButton, this);
	SDL_Rect ExitButton = { windowW / 2 - 60,windowH / 2 + 120, 240, 80 };
	exit = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Exit", ExitButton, this);
	SDL_Rect SettingsButton = { windowW / 2 - 60,windowH / 2 , 240, 80 };
	setting = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Settings", SettingsButton, this);
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	/*SDL_Rect MusicSlider = { windowW / 2 - 60,windowH / 2 +120, 240,80 };
	volumen = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 4, "Settings", MusicSlider, this);*/
	return ret;
}

bool InitialScreen::Update(float dt)
{

	

	app->render->DrawTexture(intro, 0, 0, false);

	return true;
}

// Update: draw background
bool InitialScreen::PostUpdate()
{
	// Draw everything --------------------------------------
	

	

	return true;
}

bool InitialScreen:: OnGuiMouseClickEvent(GuiControl* control) {

	bool ret = true;
	if (control->id==1){
		app->entityManager->active = true;
		app->entityManager->Start();
		app->map->active = true;
		app->map->Start();
		app->scene->active = true;
		app->scene->Start();
		this->active = false;
	}
	if (control->id == 2) {
		ret = false;
	}
	if (control->id==3){
		return ret;
	}
	/*if (control->id == 4) {
		return ret;
	}*/
	return ret;
}
