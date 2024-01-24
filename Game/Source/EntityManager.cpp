#include "Coin.h"
#include "EntityManager.h"
#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Item.h"
#include "Enemy.h"
#include "Box2D/Box2D/Box2D.h"
#include "Physics.h"


#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

//Entity* EntityManager::FindOrCreateEntity(EntityType type) {
//	// Search for an existing entity of the given type
//	for (ListItem<Entity*>* item = entities.start; item != nullptr; item = item->next) {
//		if (item->data->type == type) {
//			return item->data;  // Return the existing entity
//		}
//	}
//
//	// If not found, create a new entity
//	Entity* newEntity = nullptr;
//	switch (type) {
//	case EntityType::PLAYER:
//		newEntity = new Player();
//		break;
//		// ... handle other entity types
//	}
//
//	if (newEntity != nullptr) {
//		entities.Add(newEntity);  // Add the new entity to the manager
//	}
//	return newEntity;
//}


Entity* EntityManager::CreateEntity(EntityType type)
{
	//// Search for an existing entity of the given type
	//for (ListItem<Entity*>* item = entities.start; item != nullptr; item = item->next) {
	//	if (item->data->type == type) {
	//		return item->data;  // Return the existing entity
	//	}
	//}

	Entity* entity = nullptr; 

	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player();
		break;
	case EntityType::ITEM:
		entity = new Item();
		break;
	case EntityType::ENEMY:
		entity = new Enemy();
		break;
	case EntityType::FLYING_ENEMY:
		entity = new FlyEnemy();
		break;
	case EntityType::COIN:
		entity = new Coin();
		break;
	
	default:
		break;
	}
	
	if (entity != nullptr)
	{
		entities.Add(entity);
	}
	

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
		
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
	
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update(dt);
	}
	/*if (!app->scene->player->isOnPause || item->data == app->scene->player) {
		ret = item->data->Update(dt);
	}*/
	return ret;
}

// Declare the LoadState method
bool EntityManager::LoadState(pugi::xml_node node) {

	bool ret = true;

	//PLayer
	iPoint posPLayer = iPoint(node.child("Player").attribute("x").as_int(), node.child("Player").attribute("y").as_int());
	app->scene->player->SetPosition(posPLayer.x,posPLayer.y);

	//WALKING Enemies
	int enemyCtr = 1;
	for (pugi::xml_node enemyNode = node.child("enemy"); enemyNode; enemyNode = enemyNode.next_sibling("enemy")) {

		iPoint posEnemy = iPoint(enemyNode.attribute("x").as_int(), enemyNode.attribute("y").as_int());
		if (enemyCtr == 1) app->scene->enemy->SetPosition(posEnemy.x, posEnemy.y);
		if (enemyCtr == 2) app->scene->enemy2->SetPosition(posEnemy.x, posEnemy.y);
		enemyCtr++;
	}

	//flying Enemies
	int flyenemyCtr = 1;
	for (pugi::xml_node flyenemyNode = node.child("Flyenemy"); flyenemyNode; flyenemyNode = flyenemyNode.next_sibling("Flyenemy")) {

		iPoint posFlyEnemy = iPoint(flyenemyNode.attribute("x").as_int(), flyenemyNode.attribute("y").as_int());
		if (flyenemyCtr == 1) app->scene->FlyingEnemy->SetPosition(posFlyEnemy.x, posFlyEnemy.y);
		if (flyenemyCtr == 2) app->scene->FlyingEnemy2->SetPosition(posFlyEnemy.x, posFlyEnemy.y);
		flyenemyCtr++;
	}
	
	//Posions
	int posionCtr = 1;
    for (pugi::xml_node posionNode = node.child("item"); posionNode; posionNode = posionNode.next_sibling("item")) {

		iPoint posPosion = iPoint(posionNode.attribute("x").as_int(), posionNode.attribute("y").as_int());
		if (posionCtr == 1) app->scene->Potion->SetPosition(posPosion.x, posPosion.y);
		
		posionCtr++;
	}

	//Coins
    int coinCtr = 1;
    for (pugi::xml_node coinNode = node.child("item"); coinNode; coinNode = coinNode.next_sibling("item")) {
    iPoint posCoin = iPoint(coinNode.attribute("x").as_int(), coinNode.attribute("y").as_int());
		if (coinCtr == 1) app->scene->Coin->SetPosition(posCoin.x, posCoin.y);
		
		coinCtr++;
	}



	//Example
	/*pugi::xml_node entitymanager;
    for (entitymanager = node.child("entitymanager"); entitymanager && ret; entitymanager = entitymanager.next_sibling("entitymanager"))
    {
        EntityManager* entities = new EntityManager();


		entities->Player = entitymanager.attribute("Player").as_int();
		entities->item = entitymanager.attribute("item").as_int();
		entities->enemy = entitymanager.attribute("enemy").as_int();
		entities->Flyenemy = entitymanager.attribute("Flyenemy").as_int();


    }*/


	// Iterate over each entity node


	return ret;

	
}



bool EntityManager::SaveState(pugi::xml_node node) {
	
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;
	

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;

		pugi::xml_node entityNode = node.append_child(item->data->name.GetString());
		entityNode.append_attribute("x").set_value(item->data->position.x);
		entityNode.append_attribute("y").set_value(item->data->position.y);

	}

	

	return true;
}