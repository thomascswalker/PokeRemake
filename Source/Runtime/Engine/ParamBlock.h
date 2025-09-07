#pragma once

#include <string>

#include "Core/Decl.h"
#include "Core/Map.h"
#include "Core/Rect.h"
#include "Core/Vector.h"

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

class PParameter
{
	std::string			  Name;
	std::ptrdiff_t		  Ref;
	int32_t				  Size;
	EParamType			  Type;
	const std::type_info& TypeInfo;

public:
	PParameter() = delete;

	template <typename T>
	[[nodiscard]] PParameter(const std::string& InName, const T* InValue, EParamType InType)
		: Name(InName),
		  Ref(reinterpret_cast<size_t>(InValue)),
		  Size(sizeof(T) / 4),
		  Type(InType),
		  TypeInfo(typeid(T))
	{
		Size = sizeof(T) / 4;
	}

	~PParameter() = default;

	PParameter(const PParameter& other)
		: Name{ other.Name },
		  Ref{ other.Ref },
		  Size(other.Size),
		  Type{ other.Type },
		  TypeInfo{ other.TypeInfo } {}

	PParameter(PParameter&& other) noexcept
		: Name{ std::move(other.Name) },
		  Ref{ other.Ref },
		  Size(other.Size),
		  Type{ other.Type },
		  TypeInfo{ other.TypeInfo } {}

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

	bool IsValid()
	{
		return Ref != 0;
	}

	std::string GetName() const
	{
		return Name;
	}

	EParamType GetType() const
	{
		return Type;
	}

	void SetType(EParamType NewType)
	{
		Type = NewType;
	}

	template <typename T>
	void Set(T NewValue)
	{
		auto Ptr = reinterpret_cast<T*>(Ref);
		*Ptr = NewValue;
	}

	template <typename T>
	void Set(T NewValue, int32_t Index)
	{
		ASSERT(Index < Size, "Index out of range. Wanted <" << Size << ", got " << Index);
		auto Ptr = reinterpret_cast<T*>(Ref);
		Ptr[Index] = NewValue;
	}

	template <typename T>
	void SetRef(T* NewRef)
	{
		Ref = reinterpret_cast<size_t>(NewRef);
	}

	template <typename T>
	T Get() const
	{
		ASSERT(typeid(T).hash_code() == TypeInfo.hash_code(),
			   "Invalid type: wanted " << TypeInfo.name() << " got " << typeid(T).name());
		return *reinterpret_cast<T*>(Ref);
	}

	template <typename T>
	T Get(int32_t Index) const
	{
		ASSERT(Index < Size, "Index out of range. Wanted <" << Size << ", got " << Index);
		auto Ptr = reinterpret_cast<T*>(Ref);
		return static_cast<T>(Ptr[Index]);
	}

	void* GetRaw() const
	{
		return reinterpret_cast<void*>(Ref);
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
protected:
	std::map<std::string, PParameter> mParameters;

public:
	virtual ~IParamBlock()
	{
		mParameters.clear();
	}

	template <typename T>
	void AddParameter(const std::string& Name, T* Ref, EParamType Type)
	{
		mParameters.emplace(Name, PParameter(Name, Ref, Type));
	}

	PParameter* GetParameter(const std::string& Name)
	{
		if (mParameters.contains(Name))
		{
			return &mParameters.at(Name);
		}
		return nullptr;
	}

	std::map<std::string, PParameter*> GetAllParameters() const
	{
		std::map<std::string, PParameter*> Result;
		for (auto& [fst, snd] : mParameters)
		{
			Result.emplace(fst, const_cast<PParameter*>(&snd));
		}
		return Result;
	}

	virtual void InitializeParameters() = 0;
};

#define DECLARE_PARAMETER(Name, Type) AddParameter(#Name, &m##Name, PT_##Type)
#define PARAM(Object, Name)			  Object->GetParameter(#Name)
