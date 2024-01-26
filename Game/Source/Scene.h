#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "Coin.h"
#include "Physics.h"
#include "Enemy.h"
#include "FlyingEnemy.h"
#include "../GuiControlButton.h"
#include "../GuiSlider.h"
#include "../GuiCheckBox.h"
#include "../GuiControlValueBox.h"
#include "Boss.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool CameraLock = true;
	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool OnGuiMouseClickEvent(GuiControl* control);
	bool GameSaved1 = false;
	bool GameSaved2 = false;
	bool GameSavedinit = false;

public:
	Player* player;
	Enemy* enemy;
	Enemy* enemy2;
	FlyEnemy* FlyingEnemy;
	FlyEnemy* FlyingEnemy2;
	Item* Potion;
	
	GuiControlValueBox* Ccoins;
	GuiControlValueBox* Clifes;
	float volume = 90;
    Item* Potion2;
	Coin* coin;
	Coin* coin2;
    Coin* coin3;
    Boss* boss;


private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	SDL_Texture* mouseTileTex = nullptr;
	GuiControlButton* exitScene;
	GuiControlButton* resumen;
	GuiControlButton* settingsScene;
	GuiControlButton* FullScreen;
	GuiControlButton* FullScreenOff;
	GuiControlButton* Vsinc;
	GuiControlButton* VsincOff;
	GuiControlButton* returned;
	GuiControlButton* Initial_Screen;
	GuiSlider* volumen;
	bool IsExiting = false;
	

	

};

#endif // __SCENE_H__