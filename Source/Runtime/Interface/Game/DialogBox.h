#pragma once

#include "Engine/World.h"

#include "Interface/Box.h"
#include "Interface/Text.h"

class PDialogBox : public PBox
{
    PText* mText;
    PTexture* mTexture;

public:
    PDialogBox(const std::string& Text = "")
    {
        Padding = {0};
        SetFixedHeight(100);
        SetResizeModeH(RM_Fixed);

        mText = ConstructWidget<PText>(Text, 36.0f, PColor::Black);
        PWidget::AddChild(mText);
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
        mText->SetText(Text);
    }
};
