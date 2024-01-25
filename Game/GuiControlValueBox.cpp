#include "GuiControlValueBox.h"
#include "../Render.h"
#include "../App.h"
#include "../Audio.h"

// Constructor
GuiControlValueBox::GuiControlValueBox(uint32 id, SDL_Rect bounds, const char* text)
    : GuiControl(GuiControlType::VALUEBOX, id), value(text)
{
    this->bounds = bounds;
}

// Destructor
GuiControlValueBox::~GuiControlValueBox()
{

}

// Update method
bool GuiControlValueBox::Update(float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        app->input->GetMousePosition(mouseX, mouseY);

        // Check if the mouse is over the value box
        if (mouseX > bounds.x && mouseX < bounds.x + bounds.w &&
            mouseY > bounds.y && mouseY < bounds.y + bounds.h)
        {
            state = GuiControlState::FOCUSED;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
            {
                // Here you can handle the click event, perhaps to start editing the value
            }
        }
        else
        {
            state = GuiControlState::NORMAL;
        }
        app->render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
        app->render->DrawText(value.c_str(), bounds.x, bounds.y, bounds.w / 2, bounds.h);

    }


    return false;
}

// Optional: Render method

void GuiControlValueBox::SetValue(const std::string& value)
{
    this->value = value;
}

const std::string& GuiControlValueBox::GetValue() const
{
    return value;
}