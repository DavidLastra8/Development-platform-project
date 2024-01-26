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
#include "Lose_Screen.h"
#include "Win_Screen.h"
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
	app->Lose_Screen->active = false;
	app->Win_Screen->active = false;
	bool ret = true;
	
	SDL_Rect StartButton = { windowW / 2-450,windowH / 2-240, 240,80 };
	start = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Start", StartButton, this);

	SDL_Rect ExitButton = { windowW / 2 - 450,windowH / 2 + 240, 240, 80 };
	exit = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Exit", ExitButton, this);
	
	SDL_Rect CreditButton = { windowW / 2 - 450,windowH / 2 + 120, 240, 80 };
	credit = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "Credits", CreditButton, this);

	SDL_Rect SettingsButton = { windowW / 2 - 450,windowH / 2 , 240, 80 };
	setting = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Settings", SettingsButton, this);
	
	
	SDL_Rect MusicSlider = { windowW / 2 - 450,windowH/ 2 , 240,80 };
	volumen = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 4, "Volume", MusicSlider, this);
	volumen->state = GuiControlState::DISABLED;
	

	SDL_Rect GoBackButton = { windowW / 2 - 450,windowH  / 2 - 240, 240, 80 };
	turnBack = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "Go Back", GoBackButton, this);
	turnBack->state = GuiControlState::DISABLED;

	SDL_Rect Credits1 = { windowW / 2 - -60,windowH / 2 + 120, 240, 80 };
	credit2 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 8, "David Lastra", Credits1, this);
	credit2->state = GuiControlState::DISABLED;

	SDL_Rect Credits2 = { windowW / 2+60,windowH / 2 + 200, 240, 80 };
	credit3 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, "Yiwei Ye", Credits2, this);
	credit3->state = GuiControlState::DISABLED;

	SDL_Rect ContinueButton = { windowW / 2 - 450, windowH / 2 - 120, 240, 80 };
	continue_= (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "Continue", ContinueButton, this);

	SDL_Rect FullScreenCheck = { windowW / 2 - 450,windowH / 2 + 120, 240, 80 };
	FullScreen = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "Full Screen", FullScreenCheck, this);
	FullScreen->state = GuiControlState::DISABLED;

	SDL_Rect FullScreenCheckOff = { windowW / 2 -210,windowH / 2 + 120, 240, 80 };
	FullScreenOff = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "Full Screen Off", FullScreenCheckOff, this);
	FullScreenOff->state = GuiControlState::DISABLED;

	SDL_Rect VsincCheck = { windowW / 2 - 450,windowH / 2 - 120 , 240, 80 };
	Vsinc = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 13, "Vsinc", VsincCheck, this);
	Vsinc->state = GuiControlState::DISABLED;

	SDL_Rect VsincCheckOff = { windowW / 2 - 210,windowH / 2 -120 , 240, 80 };
	VsincOff = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 14, "Vsinc Off", VsincCheckOff, this);
	VsincOff->state = GuiControlState::DISABLED;
	app->render->camera.x = 0;
	app->render->camera.y = 0;
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
	bool ret = true;

	if (hasToExit== true)
		ret = false;

		return ret;

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

		exit->state = GuiControlState::DISABLED;
		start->state = GuiControlState::DISABLED;
		setting->state = GuiControlState::DISABLED;
		credit->state = GuiControlState::DISABLED;
		continue_->state = GuiControlState::DISABLED;
		app->scene->Clifes->state = GuiControlState::NORMAL;
		app->scene->Ccoins->state = GuiControlState::NORMAL;
	}
	if (control->id == 2) {
		hasToExit = true;
	}
	if (control->id==3){
		start->state = GuiControlState::DISABLED;
		credit->state = GuiControlState::DISABLED;
		continue_->state = GuiControlState::DISABLED;
		volumen->state = GuiControlState::NORMAL;
		setting->state = GuiControlState::DISABLED;
		turnBack->state = GuiControlState::NORMAL;
		Vsinc->state = GuiControlState::NORMAL;
		FullScreen->state = GuiControlState::NORMAL;
		
	}
	if (control->id == 4) {
		app->audio->ChangeMusicVolume(app->scene->volume);
	}
	if (control->id == 6) {
		start->state = GuiControlState::NORMAL;
		credit->state = GuiControlState::NORMAL;
		continue_->state = GuiControlState::NORMAL;
		volumen->state = GuiControlState::DISABLED;
		setting->state = GuiControlState::NORMAL;
		turnBack->state = GuiControlState::DISABLED;
		VsincOff->state = GuiControlState::DISABLED;
		Vsinc->state = GuiControlState::DISABLED;

	}
	if (control->id == 7) {
		credit2->state = GuiControlState::NORMAL;
		credit3->state = GuiControlState::NORMAL;
	}
	if (control->id == 8) {
		credit2->state = GuiControlState::DISABLED;
	}
	if (control->id == 9) {
		credit3->state = GuiControlState::DISABLED;
	}
	if (control->id == 10) {

		app->entityManager->active = true;
		app->entityManager->Start();
		app->map->active = true;
		app->map->Start();
		app->scene->active = true;
		app->scene->Start();
		app->LoadRequest();
		this->active = false;
		
		exit->state = GuiControlState::DISABLED;
		start->state = GuiControlState::DISABLED;
		setting->state = GuiControlState::DISABLED;
		credit->state = GuiControlState::DISABLED;
		continue_->state = GuiControlState::DISABLED;
		app->scene->Clifes->state = GuiControlState::NORMAL;
		app->scene->Ccoins->state = GuiControlState::NORMAL;
	}
	if (control->id == 11) {
		SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		FullScreenOff->state = GuiControlState::NORMAL;
		FullScreen->state = GuiControlState::DISABLED;
	}
	if (control->id == 12) {
		app->win->GetWindowSize(windowW, windowH);
		SDL_SetWindowSize(app->win->window, windowW, windowH);
		SDL_SetWindowFullscreen(app->win->window, 0);

		FullScreen->state = GuiControlState::NORMAL;
		FullScreenOff->state = GuiControlState::DISABLED;
	}
	if (control->id == 13) {
		app->IsVsincActive = true;
		Vsinc->state = GuiControlState::DISABLED;
		VsincOff->state = GuiControlState::NORMAL;
	}
	if (control->id == 14) {
		app->IsVsincActive = false;
		Vsinc->state = GuiControlState::NORMAL;
		VsincOff->state = GuiControlState::DISABLED;
	}
	return ret;
}
