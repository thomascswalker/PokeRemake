#pragma once

#include "Interface/Box.h"
#include "Interface/Text.h"

class PDialogBox : public PBox
{
    PText mText;

public:
    PDialogBox(const std::string& Text = "") : mText(Text, 36.0f, PColor::Black)
    {
        SetFixedHeight(100);
        SetResizeModeH(RM_Fixed);
        PWidget::AddChild(&mText);
    }

    void Draw(const PRenderer* Renderer) const override
    {
        FRect Geometry = GetGeometry();
        Renderer->SetDrawColor(PColor::White);
        Renderer->DrawFillRect(Geometry);
        Renderer->SetDrawColor(PColor::Black);
        Renderer->DrawRect(Geometry.Shrunk(4), 4.0f);

        mText.Draw(Renderer);
    }

    void SetText(const std::string& Text)
    {
        mText.SetText(Text);
    }
};
