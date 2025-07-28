#pragma once
#include "Core/Json.h"

namespace ActorManager
{
    void LoadActorDefs();
    JSON& GetActorDefs();
    JSON* GetActorDef(const std::string& Name);
}
