#pragma once
#include "Component.h"

class PSelectionComponent : public PComponent, public IDrawable
{
    bool mSelected;

public:
    PSelectionComponent(): mSelected(false)
    {
        mSerializable = false;
    }

    float GetDepth() const override
    {
        return mOwner->GetPosition3D().Z;
    }

    bool DebugDraw(const PRenderer* Renderer) const override;

    bool GetSelected() const
    {
        return mSelected;
    }

    void SetSelected(bool State)
    {
        mSelected = State;
    }

    void ToggleSelected()
    {
        mSelected = !mSelected;
    }
};
