#include "SelectionComponent.h"

bool PSelectionComponent::DebugDraw(const PRenderer* Renderer) const
{
    if (!mSelected)
    {
        return true;
    }

    auto Bounds = mOwner->GetWorldBounds();
    Renderer->SetDrawColor(PColor::SelectionOpaque);
    Renderer->DrawRectAt(Bounds, 4.0f);
    Renderer->DrawTextAt(GetOwner()->GetDisplayName(), mOwner->GetCenter2D(), 24.0f, true);

    return true;
}
