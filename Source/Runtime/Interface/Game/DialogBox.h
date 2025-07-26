#pragma once

#include "Interface/Box.h"
#include "Interface/Text.h"

class PDialogBox : public PBox
{
    PText mText;
    PTexture* mTexture;

public:
    PDialogBox(const std::string& Text = "") : mText(Text, 36.0f, PColor::Black)
    {
        Padding = {0};
        SetFixedHeight(100);
        SetResizeModeH(RM_Fixed);
        PWidget::AddChild(&mText);
    }

    void Draw(const PRenderer* Renderer) const override
    {
        PTexture* Tex  = PTextureManager::Get("DialogBox");
        FRect Geometry = GetGeometry();
        Renderer->DrawTexture(Tex, Tex->GetRect(), Geometry);
        // mText.Draw(Renderer);
    }

    void SetText(const std::string& Text)
    {
        mText.SetText(Text);
    }
};
