#include "Title_Screen.h"

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
#include "InitialScreen.h"
#include "../Window.h"

TitleScreen::TitleScreen() : Module()
{

}

TitleScreen::~TitleScreen()
{

}

// Load assets
bool TitleScreen::Start()
{
	app->win->GetWindowSize(windowW, windowH);
	intro = app->tex->Load("Assets/Textures/Title_Screen.png");
	app->entityManager->active = false;
	app->map->active = false;
	app->scene->active = false;
	app->Lose_Screen->active = false;
	app->Win_Screen->active = false;
	app->initialScreen->active = false;
	
	
	bool ret = true;

	app->render->camera.x = 0;
	app->render->camera.y = 0;
	return ret;
}

bool TitleScreen::Update(float dt)
{

	app->render->DrawTexture(intro, 0, 0, false);
	app->initialScreen->start->state = GuiControlState::DISABLED;
	app->initialScreen->exit->state = GuiControlState::DISABLED;
	app->initialScreen->setting->state = GuiControlState::DISABLED;
	app->initialScreen->credit->state = GuiControlState::DISABLED;
	app->initialScreen->continue_->state = GuiControlState::DISABLED;
	app->initialScreen->VsincOff->state = GuiControlState::DISABLED;

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		this->active = false;
		app->initialScreen->active = true;
		app->initialScreen->Start();
	}

	return true;
}

bool TitleScreen::PostUpdate()
{
	// Draw everything --------------------------------------
	bool ret = true;

	if (hasToExit == true)
		ret = false;

	return ret;

}

bool TitleScreen::OnGuiMouseClickEvent(GuiControl* control) {
	bool ret = true;

	return ret;
}