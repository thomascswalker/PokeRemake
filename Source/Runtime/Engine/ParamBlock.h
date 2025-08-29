#pragma once

#include <string>

#include "Core/Vector.h"
#include "Core/Map.h"

using TParam = std::ptrdiff_t;

class PParameter
{
    std::string Name;
    TParam Ref;

public:
    PParameter() = default;
    PParameter(const std::string& InName, const TParam InValue) : Name(InName), Ref(InValue) {}
    ~PParameter() = default;

    std::string GetName() const
    {
        return Name;
    }

    void Set(float NewValue)
    {
        auto Ptr = reinterpret_cast<float*>(Ref);
        *Ptr     = NewValue;
    }

    template <typename T>
    void Set(T NewValue)
    {
        auto Ptr = reinterpret_cast<T*>(Ref);
        *Ptr     = NewValue;
    }

    template <typename T>
    T Get() const
    {
        return *reinterpret_cast<T*>(Ref);
    }
};

class PParamBlock
{
    std::map<std::string, PParameter> Parameters;

public:
    PParamBlock()  = default;
    ~PParamBlock() = default;

    template <typename T>
    void AddParameter(const std::string& Name, T* Ref)
    {
        size_t Ptr       = reinterpret_cast<size_t>(Ref);
        Parameters[Name] = PParameter(Name, Ptr);
    }

    PParameter* GetParameter(const std::string& Name)
    {
        if (Parameters.contains(Name))
        {
            return &Parameters[Name];
        }
        return nullptr;
    }
};
