#pragma once

#include "Input.h"
#include "Object.h"

class PGameMode : public PObject, public IInputHandler
{
protected:
    JSON mSaveState;

public:
    virtual std::string GetName()
    {
        return "DefaultGameMode";
    }

    void Start() override;

    virtual bool Load() = 0;
    virtual bool Unload() = 0;
};
