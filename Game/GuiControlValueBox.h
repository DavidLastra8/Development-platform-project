#ifndef GUICONTROLVALUEBOX_H
#define GUICONTROLVALUEBOX_H

#include "GuiControl.h"
#include "SDL/include/SDL.h"
#include <string>

class GuiControlValueBox : public GuiControl
{
public:
    GuiControlValueBox(uint32 id, SDL_Rect bounds, const char* text);

    virtual ~GuiControlValueBox();

    bool Update(float dt) override;

    void SetValue(const std::string& value);
    const std::string& GetValue() const;

public:
    std::string value; // The current value displayed in the box

};

#endif // GUICONTROLVALUEBOX_H