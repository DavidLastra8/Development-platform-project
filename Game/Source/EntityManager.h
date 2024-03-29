#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "List.h"

class EntityManager : public Module
{
public:

	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	

	// Declare the LoadState method
	bool LoadState(pugi::xml_node node) override;
	bool SaveState(pugi::xml_node node) override;



public:
	/*Entity* FindOrCreateEntity(EntityType type);*/

	List<Entity*> entities;
	
	int item;
	int coin;
	int enemy;
	int Flyenemy;
	int boss;
	



};

#endif // __ENTITYMANAGER_H__
