#include "GuiControlButton.h"
#include "../Render.h"
#include "../App.h"
#include "../Audio.h"

GuiControlButton::GuiControlButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;
}

GuiControlButton::~GuiControlButton()
{

}

bool GuiControlButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// L15: TODO 3: Update the state of the GUiButton according to the mouse position

		//L15: TODO 4: Draw the button according the GuiControl State

	}

	return false;
}

