#pragma once
#include "Component.h"

class PSelectionComponent : public PComponent, public IDrawable
{
    bool mSelected = false;

public:
    PSelectionComponent()
    {
        mSerializable = false;
    }

    bool DebugDraw(const PRenderer* Renderer) const override
    {
        if (!mSelected)
        {
            return true;
        }
        auto Rect = mOwner->GetWorldBounds();
        Renderer->SetDrawColor(255, 255, 128, 128);
        Renderer->DrawFillRectAt(Rect);
        return true;
    }

    bool GetSelected()
    {
        return mSelected;
    }

    void SetSelected(bool value)
    {
        mSelected = value;
    }

    void ToggleSelected()
    {
        mSelected = !mSelected;
    }
};
