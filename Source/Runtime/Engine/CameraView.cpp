#include "CameraView.h"
#include "Actors/Actor.h"

PComponent* PCameraView::GetComponent() const
{
    return mComponent;
}

void PCameraView::SetComponent(PComponent* Component)
{
    mComponent = Component;
}
