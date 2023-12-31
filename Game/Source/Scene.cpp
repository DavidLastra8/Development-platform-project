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
#include "App.h"
#include "Physics.h"





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

	if (config.child("enemy")) {
		enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		enemy->parameters = config.child("enemy");
	}

	if (config.child("enemy2")) {
		enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		enemy->parameters = config.child("enemy2");
	}

	if (config.child("Flyenemy")) {
		FlyingEnemy = (FlyEnemy*)app->entityManager->CreateEntity(EntityType::FLYING_ENEMY);
		FlyingEnemy->parameters = config.child("Flyenemy");
	}
	if (config.child("Flyenemy2")) {
		FlyingEnemy = (FlyEnemy*)app->entityManager->CreateEntity(EntityType::FLYING_ENEMY);
		FlyingEnemy->parameters = config.child("Flyenemy2");
	}

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	//img = app->tex->Load("Assets/Textures/test.png");
	
	//Music is commented so that you can add your own music
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

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

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

//// L14: TODO 6: Implement a method to load the state
//// for now load camera's x and y
//bool Scene::LoadState(pugi::xml_node node) {
//
//	player->position.x = node.child("player").attribute("x").as_int();
//	player->position.y = node.child("player").attribute("y").as_int();
//	b2Vec newPos(PIXEL_TO_METERS(player->position.x), PIXEL_TO_METERS(player.position.y));
//	return true;
//}
//
//
//// L14: TODO 8: Create a method to save the state of the renderer
//// using append_child and append_attribute
//bool Render::SaveState(pugi::xml_node node) {
//
//	pugi::xml_node camNode = node.append_child("camera");
//	camNode.append_attribute("x").set_value(camera.x);
//	camNode.append_attribute("y").set_value(camera.y);
//
//	return true;
//}

bool Scene::LoadState(pugi::xml_node node)
{
	player->position.x = node.child("player").attribute("x").as_int();
	player->position.y = node.child("player").attribute("y").as_int();
	b2Vec2 newPos(PIXEL_TO_METERS(player->position.x), PIXEL_TO_METERS(player->position.y));
	player->pbody->body->SetTransform(newPos, player->pbody->body->GetAngle());
	return true;

};

bool Scene::SaveState(pugi::xml_node node)
{
	pugi::xml_node playerNode = node.append_child("player");
	playerNode.append_attribute("x").set_value(player->position.x);
	playerNode.append_attribute("y").set_value(player->position.y);


	return true;
};
