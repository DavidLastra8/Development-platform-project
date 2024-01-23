#include "InitialScreen.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "Map.h"
#include "Scene.h"

#include "ModuleFadeToBlack.h"

InitialScreen::InitialScreen() : Module()
{

}

InitialScreen::~InitialScreen()
{

}

// Load assets
bool InitialScreen::Start()
{
	intro =app->tex->Load("Assets/Textures/portada_development.png");
	app->entityManager->active = false;
	app->map->active = false;
	app->scene->active = false;

	bool ret = true;

	

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return ret;
}

bool InitialScreen::Update(float dt)
{
	
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		app->entityManager->active = true;
		app->entityManager->Start();
		app->map->active = true;
		app->map->Start();
		app->scene->active = true;
		app->scene->Start();
		this->active = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->moduleFadeToBlack->FadeToBlack(this, (ModuleFadeToBlack*)app->scene, 90);
		
	}

	return true;
}

// Update: draw background
bool InitialScreen::PostUpdate()
{
	// Draw everything --------------------------------------
	app->render->DrawTexture(intro, 0, 0, false);
	

	return true;
}