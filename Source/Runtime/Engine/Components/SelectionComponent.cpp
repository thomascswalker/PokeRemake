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
    auto Depth = GetOwner()->GetPosition3D().Z;
    auto Text  = std::format("{}: {}", GetOwner()->GetClassName().c_str(), Depth);
    Renderer->DrawTextAt(Text, mOwner->GetCenter2D(), 24.0f, true);

    return true;
}
