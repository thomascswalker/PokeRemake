#pragma once

struct SSaveState
{
    virtual ~SSaveState() = default;
    virtual bool Save() = 0;
    virtual bool Load() = 0;
};
