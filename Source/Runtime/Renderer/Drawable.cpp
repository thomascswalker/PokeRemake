#include "Drawable.h"
#include "Engine/Actors/Actor.h"

bool DepthSort(const PActor* A, const PActor* B)
{
    return A->GetPosition3D().Z > B->GetPosition3D().Z;
}
