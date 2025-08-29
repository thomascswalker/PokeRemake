#pragma once

#include <string>

#include "Core/Decl.h"
#include "Core/Vector.h"
#include "Core/Map.h"
#include "Core/Rect.h"

enum EParamType
{
    PT_Int,
    PT_Float,
    PT_String,
    PT_FVector2,
    PT_FVector3,
    PT_FVector4,
    PT_FRect
};

using TParam = std::ptrdiff_t;

class PParameter
{
    std::string Name;
    TParam Ref;
    EParamType Type;
    const std::type_info& TypeInfo;

public:
    PParameter() = delete;

    template <typename T>
    [[nodiscard]] PParameter(const std::string& InName, const T* InValue, EParamType InType)
        : Name(InName),
          Ref(reinterpret_cast<size_t>(InValue)),
          Type(InType),
          TypeInfo(typeid(T)) {}

    ~PParameter() = default;

    PParameter(const PParameter& other)
        : Name{other.Name},
          Ref{other.Ref},
          Type{other.Type},
          TypeInfo{other.TypeInfo} {}

    PParameter(PParameter&& other) noexcept
        : Name{std::move(other.Name)},
          Ref{other.Ref},
          Type{other.Type},
          TypeInfo{other.TypeInfo} {}

    PParameter& operator=(const PParameter& other)
    {
        if (this == &other)
        {
            return *this;
        }
        *this = PParameter(other);
        return *this;
    }

    PParameter& operator=(PParameter&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }
        *this = PParameter(other);
        return *this;
    }

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
        ASSERT(typeid(T).hash_code() == TypeInfo.hash_code(),
               "Invalid type: wanted " << TypeInfo.name() << " got " << typeid(T).name());
        return *reinterpret_cast<T*>(Ref);
    }

    std::string ToString() const
    {
        switch (Type)
        {
        case PT_Int:
            {
                return std::to_string(*reinterpret_cast<int*>(Ref));
            }
        case PT_Float:
            {
                return std::to_string(*reinterpret_cast<float*>(Ref));
            }
        case PT_String:
            {
                return *reinterpret_cast<std::string*>(Ref);
            }
        case PT_FVector2:
            {
                return reinterpret_cast<FVector2*>(Ref)->ToString();
            }
        case PT_FVector3:
            {
                return reinterpret_cast<FVector3*>(Ref)->ToString();
            }
        case PT_FVector4:
            {
                return reinterpret_cast<FVector4*>(Ref)->ToString();
            }
        case PT_FRect:
            {
                return reinterpret_cast<FRect*>(Ref)->ToString();
            }
        default:
            {
                return "Unknown";
            }
        }
    }

    bool operator==(const PParameter& Other) const
    {
        return Name == Other.Name && Ref == Other.Ref && Type == Other.Type;
    }

    bool operator!=(const PParameter& Other) const
    {
        return Name != Other.Name || Ref != Other.Ref || Type != Other.Type;
    }

    bool operator<(const PParameter& Other) const
    {
        return Name < Other.Name || Ref < Other.Ref || Type < Other.Type;
    }

    bool operator>(const PParameter& Other) const
    {
        return Name > Other.Name || Ref > Other.Ref || Type > Other.Type;
    }
};

class IParamBlock
{
    std::map<std::string, PParameter> Parameters;

public:
    template <typename T>
    void AddParameter(const std::string& Name, T* Ref, EParamType Type)
    {
        Parameters.emplace(Name, PParameter{Name, Ref, Type});
    }

    PParameter* GetParameter(const std::string& Name)
    {
        if (Parameters.contains(Name))
        {
            return &Parameters.at(Name);
        }
        return nullptr;
    }
};

#define DECLARE_PARAMETER(Name, Type) AddParameter(#Name, &m##Name, PT_##Type)
#define PARAM(Object, Name) Object->GetParameter(#Name)
