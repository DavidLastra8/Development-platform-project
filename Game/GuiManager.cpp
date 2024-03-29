#include "GuiManager.h"
#include "../App.h"
#include "../Textures.h"

#include "GuiControlButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "GuiControlValueBox.h"
#include "../Audio.h"

GuiManager::GuiManager() :Module()
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	return true;
}

// L15: TODO1: Implement CreateGuiControl function that instantiates a new GUI control and add it to the list of controls
GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds)
{
	GuiControl* guiControl = nullptr;

	switch (type)
	{
	case GuiControlType::BUTTON:
		guiControl = new GuiControlButton(id, bounds, text);
		guiControl->texture = app->tex->Load("Assets/Textures/button_development.png");
		break;

	case GuiControlType::SLIDER:
		guiControl = new GuiSlider(id, bounds, text);
		break;

	case GuiControlType::CHECKBOX:
		guiControl = new GuiCheckBox(id, bounds, text);
		break;

	case GuiControlType::VALUEBOX:
		guiControl = new GuiControlValueBox(id, bounds, text);
		if (guiControl->id == 10) {
			guiControl->texture = app->tex->Load("Assets/Textures/lives.png");
		}
		else
		{
			guiControl->texture = app->tex->Load("Assets/Textures/coin.png");

		}
		break;
	}
	//Set the observer
	guiControl->observer = observer;

	// Created GuiControls are add it to the list of controls
	guiControlsList.Add(guiControl);
	//Call the constructor according to the GuiControlType

	//Set the observer

	// Created GuiControls are add it to the list of controls

	return guiControl;
}

bool GuiManager::Update(float dt)
{

	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		control->data->Update(dt);
		control = control->next;
	}

	return true;
}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		RELEASE(control);
	}

	return true;

	return false;
}



