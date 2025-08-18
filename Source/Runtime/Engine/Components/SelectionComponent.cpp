#include "SelectionComponent.h"

bool PSelectionComponent::DebugDraw(const PRenderer* Renderer) const
{
    auto Bounds = mOwner->GetWorldBounds();
    if (mSelected)
    {
        Renderer->SetDrawColor(255, 255, 128, 128);
        Renderer->DrawFillRectAt(Bounds);
    }

    Renderer->SetDrawColor(PColor::UISecondary);
    Renderer->DrawRectAt(Bounds);

    auto Depth = GetOwner()->GetPosition3D().Z;
    auto Text  = std::format("{}: {}", GetOwner()->GetClassName().c_str(), Depth);
    Renderer->DrawTextAt(Text, Bounds.Max(), 24.0f);

    return true;
}
