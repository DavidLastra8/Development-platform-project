#include "EntityManager.h"
#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Item.h"
#include "Enemy.h"



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
	// Search for an existing entity of the given type
	for (ListItem<Entity*>* item = entities.start; item != nullptr; item = item->next) {
		if (item->data->type == type) {
			return item->data;  // Return the existing entity
		}
	}

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

	return ret;
}

// Declare the LoadState method
bool EntityManager::LoadState(pugi::xml_node node) {

	bool ret = true;

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
    for (pugi::xml_node entityNode = node.child("entity"); entityNode; entityNode = entityNode.next_sibling("entityNode")) {
        // Read the type of entity (assuming there's a type attribute or similar)
        EntityType entityType = static_cast<EntityType>(entityNode.attribute("type").as_int());

        // Depending on your game structure, find or create the entity
		Entity* entity = CreateEntity(entityType);
        
       

        if (entity != nullptr) {
            // Now load the data for the entity from the XML node
            entity->position.x = entityNode.attribute("x").as_int();
            entity->position.y = entityNode.attribute("y").as_int();
			

            // ... load other relevant data for the entity
        }


    }

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