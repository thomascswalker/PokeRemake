#pragma once
#include "Engine/Actors/Actor.h"

class PBattleView : public PActor
{
protected:
    PCameraComponent* mCameraComponent = nullptr;

public:
    void Start() override;
};
