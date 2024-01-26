#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"
#include "Box2D/Box2D/Box2D.h"
#include "Player.h"
#include "Enemy.h"
#include "FlyingEnemy.h"
#include "EntityManager.h"
#include "../GuiControl.h"
#include "../GuiManager.h"
#include "../InitialScreen.h"
#include "../Lose_Screen.h"
#include "../Win_Screen.h"
#include "App.h"
#include "Physics.h"
#include "Boss.h"
#include "Coin.h"





Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}

	if (config.child("Potion")) {
		Potion = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		Potion->parameters = config.child("Potion");
	}


	if (config.child("Coin"))
	{
		coin = (Coin*)app->entityManager->CreateEntity(EntityType::COIN);
		coin->parameters = config.child("Coin");
	}
	if (config.child("Coin2"))
	{
		coin2 = (Coin*)app->entityManager->CreateEntity(EntityType::COIN);
		coin2->parameters = config.child("Coin2");
	}

	if (config.child("enemy")) {
		enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		enemy->parameters = config.child("enemy");
	}

	if (config.child("enemy2")) {
		enemy2 = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		enemy2->parameters = config.child("enemy2");
	}

	if (config.child("Flyenemy")) {
		FlyingEnemy = (FlyEnemy*)app->entityManager->CreateEntity(EntityType::FLYING_ENEMY);
		FlyingEnemy->parameters = config.child("Flyenemy");
	}
	if (config.child("Flyenemy2")) {
		FlyingEnemy2 = (FlyEnemy*)app->entityManager->CreateEntity(EntityType::FLYING_ENEMY);
		FlyingEnemy2->parameters = config.child("Flyenemy2");
	}
	////spawn a Boss
    if (config.child("Boss")) {
		boss = (Boss*)app->entityManager->CreateEntity(EntityType::BOSS);
		boss->parameters = config.child("Boss");
	}

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	//img = app->tex->Load("Assets/Textures/test.png");
	
	//Music is commented so that you can add your own music
	app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	SDL_Rect ExitButton = { windowW / 2 - 60,windowH / 2 + 120, 240, 80 };
	exitScene = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Exit", ExitButton, this);
	exitScene->state = GuiControlState::DISABLED;
	SDL_Rect ResumeButton = { windowW / 2 - 60,windowH / 2 , 240, 80 };
	resumen = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Resume", ResumeButton, this);
	resumen->state = GuiControlState::DISABLED;
	SDL_Rect SettingsSceneButton = { windowW / 2 - 60,windowH / 2 -120, 240, 80 };
	settingsScene = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Settings", SettingsSceneButton, this);
	settingsScene->state = GuiControlState::DISABLED;
	SDL_Rect Return_Initial = { windowW / 2 - 60,windowH / 2 -240, 340, 80 };
	Initial_Screen = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "Return to inicial screen", Return_Initial, this);
	Initial_Screen->state = GuiControlState::DISABLED;
	SDL_Rect FullScreenCheck = { windowW / 2 - 60,windowH / 2 + 120, 240, 80 };
	FullScreen = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Full Screen", FullScreenCheck, this);
	FullScreen->state = GuiControlState::DISABLED;
	SDL_Rect FullScreenCheckOff = { windowW / 2 - 60,windowH / 2, 240, 80 };
	FullScreenOff = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Full Screen Off", FullScreenCheckOff, this);
	FullScreenOff->state = GuiControlState::DISABLED;
	SDL_Rect VsincCheck = { windowW / 2 - 60,windowH / 2-120 , 240, 80 };
	Vsinc = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "Vsinc", VsincCheck, this);
	Vsinc->state = GuiControlState::DISABLED;
	SDL_Rect VsincCheckOff = { windowW / 2 - 60,windowH / 2 + 240 , 240, 80 };
	VsincOff = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "Vsinc Off", VsincCheckOff, this);
	VsincOff->state = GuiControlState::DISABLED;
	SDL_Rect Go_Back = { windowW / 2 - 60,windowH / 2 - 240 , 240, 80 };
	returned = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 8, "Go Back", Go_Back, this);
	returned->state = GuiControlState::DISABLED;
	SDL_Rect CoinBox  = { windowW / 2 +260,windowH / 2 - 240 , 240, 80 };
	Ccoins = (GuiControlValueBox*)app->guiManager->CreateGuiControl(GuiControlType::VALUEBOX, 9, "Coins:", CoinBox, this);
	Ccoins->state = GuiControlState::DISABLED;
	SDL_Rect LifesBox = { windowW / 2 + 260,windowH / 2 - 160 , 240, 80 };
	Clifes = (GuiControlValueBox*)app->guiManager->CreateGuiControl(GuiControlType::VALUEBOX, 10, "Lifes:", LifesBox, this);
	Clifes->state = GuiControlState::DISABLED;
	app->Lose_Screen->exit2->state= GuiControlState::DISABLED;
	app->Win_Screen->exit3->state= GuiControlState::DISABLED;
	mouseTileTex = app->tex->Load("Assets/Maps/tileSelection.png");
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	float camSpeed = 1; 
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		CameraLock = true;
	}
	if (CameraLock == true) {
		if (player->position.x < app->win->screenSurface->w / 2) {
			player->position.x = app->win->screenSurface->w / 2;
		}
		app->render->camera.x = -player->position.x + app->win->screenSurface->w / 2;
		if (player->position.y <= 150) {
			player->position.y += app->win->screenSurface->h + 13*2;
		}
		// if (player->position.y >= app->win->screenSurface->h) {
		//	//player->position.y -= app->win->screenSurface->h + 13 * 2;
		//}
		//else {
		//	//player->position.y = 0;
		//}
		app->render->camera.y = -player->position.y+275;
	}


		
	else
	{
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			app->render->camera.y -= (int)ceil(camSpeed * dt);

		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			app->render->camera.y += (int)ceil(camSpeed * dt);

		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			app->render->camera.x -= (int)ceil(camSpeed * dt);

		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			app->render->camera.x += (int)ceil(camSpeed * dt);
	}
	
	// Renders the image in the center of the screen 
	//app->render->DrawTexture(img, (int)textPosX, (int)textPosY);

	// L14: TODO 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		app->LoadRequest();
		
	}
	if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN) {
		app->scene->player->isOnPause = true;
		
		app->scene->exitScene->state = GuiControlState::NORMAL;
		app->scene->resumen->state = GuiControlState::NORMAL;
		app->scene->Initial_Screen->state = GuiControlState::NORMAL;
		app->scene->settingsScene->state = GuiControlState::NORMAL;
		Ccoins->state = GuiControlState::DISABLED;
		Clifes->state = GuiControlState::DISABLED;
	}
	std::string strPlayerLifes = std::to_string(player->lives);
	Clifes->SetValue(strPlayerLifes);
	std::string coins = std::to_string(player->coinCount);
	Ccoins->SetValue(coins);

	// at the beginning of the level, Save the Game only once
	if (player->position.x >= 300 && GameSavedinit == false) {
		app->SaveRequest();
		app->audio->PlayFx(player->endLevelFxId);
		GameSavedinit = true;
	}

	// when player x reaches 1672, Save the Game only once
	if (player->position.x >= 1672 && GameSaved1 == false) {
		app->SaveRequest();
		app->audio->PlayFx(player->endLevelFxId);
		GameSaved1 = true;
	}
	
	// when player reaches the end of level1, Save the Game only once
	if (player->position.x >= player->endLevelX && player->position.y >= player->endLevelY && !player->endLevelSoundPlayed && GameSaved2 == false)
	{
		// Play the end-level sound effect
		app->audio->PlayFx(player->endLevelFxId);

		app->SaveRequest();
		player->SetPosition(5050, 1102);
		player->endLevelSoundPlayed = true;  // Set the flag to true
		GameSaved2 = true;

	}
	if (player->lives == 0) {
		this->active = false;
		app->entityManager->active = false;
		app->scene->active = false;
		app->map->active = false;
		app->Lose_Screen->active = true;
		app->Lose_Screen->Start();
		Ccoins->state = GuiControlState::DISABLED;
		Clifes->state = GuiControlState::DISABLED;
		app->Lose_Screen->exit2->state = GuiControlState::NORMAL;

	}
	if (boss->lifeCount == 0 || app->input->GetKey(SDL_SCANCODE_K)==KEY_DOWN) {
		this->active = false;
		app->entityManager->active = false;
		app->scene->active = false;
		app->map->active = false;
		app->Win_Screen->active = true;
		app->Win_Screen->Start();
		Ccoins->state = GuiControlState::DISABLED;
		Clifes->state = GuiControlState::DISABLED;
		app->Win_Screen->exit3->state = GuiControlState::NORMAL;
	}
	iPoint mousePos;
	app->input->GetMousePosition(mousePos.x, mousePos.y);
	iPoint mouseTile = app->map->WorldToMap(mousePos.x - app->render->camera.x,
		mousePos.y - app->render->camera.y);

	// Render a texture where the mouse is over to highlight the tile, use the texture 'mouseTileTex'
	iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
	app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);
	int x = player->GetTilex();
	int y = player->GetTiley();
	LOG("%d", x);
	LOG("%d", y);
	iPoint origin = iPoint(x, y);

	//If mouse button is pressed modify player position
	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
		player->position = iPoint(highlightedTileWorld.x, highlightedTileWorld.y);
		app->map->pathfinding->CreatePath(origin, mouseTile);
	}

	// L13: Get the latest calculated path and draw
	const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
	}
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || IsExiting == true)
		ret = false;

	return ret;
}
bool Scene::OnGuiMouseClickEvent(GuiControl* control) {

	bool ret = true;
	if (control->id == 2) {
		IsExiting = true;
	}
	if (control->id == 1) {
		app->scene->player->isOnPause = false;
		exitScene->state = GuiControlState::DISABLED;
		settingsScene->state = GuiControlState::DISABLED;
		resumen->state = GuiControlState::DISABLED;
		Ccoins->state = GuiControlState::NORMAL;
		Clifes->state = GuiControlState::NORMAL;
	}
	if (control->id == 3) {
		resumen->state = GuiControlState::DISABLED;
		settingsScene->state = GuiControlState::DISABLED;
		FullScreen->state = GuiControlState::NORMAL;
		Vsinc->state = GuiControlState::NORMAL;
		returned->state = GuiControlState::NORMAL;
		exitScene->state = GuiControlState::DISABLED;
	}
	if (control->id == 4) {
		SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		FullScreenOff->state = GuiControlState::NORMAL;
		FullScreen->state = GuiControlState::DISABLED;
	}
	if (control->id == 5) {
		app->win->GetWindowSize(windowW, windowH);
		SDL_SetWindowSize(app->win->window, windowW, windowH);
		SDL_SetWindowFullscreen(app->win->window, 0);
		
		FullScreen->state = GuiControlState::NORMAL;
		FullScreenOff->state = GuiControlState::DISABLED;
	}
	if (control->id == 6) {
		app->IsVsincActive = true;
		Vsinc->state = GuiControlState::DISABLED;
		VsincOff->state = GuiControlState::NORMAL;
	}
	if (control->id == 7) {
		app->IsVsincActive = false;
		Vsinc->state = GuiControlState::NORMAL;
		VsincOff->state = GuiControlState::DISABLED;
	}
	if (control->id == 8) {
		FullScreen->state = GuiControlState::DISABLED;
		FullScreenOff->state = GuiControlState::DISABLED;
		Vsinc->state = GuiControlState::DISABLED;
		Initial_Screen->state = GuiControlState::DISABLED;
		VsincOff->state = GuiControlState::DISABLED;
		returned->state = GuiControlState::DISABLED;
		resumen->state = GuiControlState::NORMAL;
		exitScene->state = GuiControlState::NORMAL;
		settingsScene->state = GuiControlState::NORMAL;
	}
	if (control->id == 11) {
		app->scene->player->isOnPause = false;
		app->scene->player->SetPosition(400, 1102);
		app->initialScreen->active = true;
		app->initialScreen->Start();
		app->entityManager->active = false;
		app->scene->exitScene->state = GuiControlState::DISABLED;
		app->scene->resumen->state = GuiControlState::DISABLED;
		app->scene->Initial_Screen->state = GuiControlState::DISABLED;
		app->scene->settingsScene->state = GuiControlState::DISABLED;
		Ccoins->state = GuiControlState::DISABLED;
		Clifes->state = GuiControlState::DISABLED;

	}
	return ret;
}
// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	//Destroying all entities
	app->entityManager->DestroyEntity(coin);
	app->entityManager->DestroyEntity(player);
	app->entityManager->DestroyEntity(enemy);
	app->entityManager->DestroyEntity(enemy2);
	app->entityManager->DestroyEntity(FlyingEnemy);
	app->entityManager->DestroyEntity(FlyingEnemy2);
	app->entityManager->DestroyEntity(boss);
	app->entityManager->DestroyEntity(Potion);
	app->entityManager->DestroyEntity(coin);

	return true;
}

