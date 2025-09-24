#pragma once

#include "Core/Meta.h"

#include "Object.h"
#include "Settings.h"

// Manages all objects in the engine
class PGameInstance : public PObject
{
	SSettings mSettings;

	std::vector<std::shared_ptr<PObject>> mObjects;
	std::vector<PObject*>				  mDestroyableObjects;

	void DestroyObjectInternal(const PObject* Object)
	{
		if (!Object)
		{
			return;
		}

		std::shared_ptr<PObject> SharedObject;
		for (auto Ptr : mObjects)
		{
			if (Ptr.get() == Object)
			{
				SharedObject = Ptr;
				break;
			}
		}
		if (!SharedObject)
		{
			return;
		}

		mObjects.erase(std::ranges::find(mObjects, SharedObject));
	}

public:
	PGameInstance()
	{
		GSettings = &mSettings;
	};

	void Tick(float DeltaTime) override
	{
		for (auto Object : mDestroyableObjects)
		{
			DestroyObjectInternal(Object);
		}
		mDestroyableObjects.clear();
	}

	template <IS_SUBCLASS_OF(PObject), typename... ArgsType>
	T* ConstructObject(ArgsType&&... Args)
	{
		std::shared_ptr<T> Object = std::make_shared<T>(std::forward<ArgsType>(Args)...);
		mObjects.push_back(Object);
		Object->GenerateInternalName();
		return Object.get();
	}

	void DestroyObject(PObject* Object)
	{
		mDestroyableObjects.emplace_back(Object);
	}
};

extern PGameInstance* GGameInstance;