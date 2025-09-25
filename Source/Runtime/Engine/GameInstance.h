#pragma once

#include "Core/Containers.h"
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

	void Cleanup()
	{
		for (auto Object : mDestroyableObjects)
		{
			DestroyObjectInternal(Object);
		}
		mDestroyableObjects.clear();
	}

public:
	PGameInstance()
	{
		GSettings = &mSettings;
	}

	void Tick(float DeltaTime) override
	{
		Cleanup();
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
		Object->MarkPendingDestruction();
		mDestroyableObjects.push_back(Object);
	}

	template <typename T = PObject>
	std::vector<T*> GetObjects()
	{
		std::vector<T*> Result;
		for (auto Object : mObjects)
		{
			if (T* CastObject = dynamic_cast<T*>(Object.get()))
			{
				Result.push_back(CastObject);
			}
		}
		return Result;
	}
};

extern PGameInstance* GGameInstance;