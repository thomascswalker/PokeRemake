#pragma once

#include <memory>
#include <vector>

#include "Actors/Actor.h"
#include "Actors/Chunk.h"
#include "Components/Component.h"
#include "Interface/Widget.h"

#if _EDITOR
DECLARE_MULTICAST_DELEGATE(DActorSelected, PActor*);
#endif

class PWorld : public PObject
{
	// META(PWorld)

	// Chunks
	std::map<std::string, PChunk*> mChunks;

	std::vector<std::shared_ptr<PActor>>	 mActors;
	std::vector<std::shared_ptr<PComponent>> mComponents;

	std::vector<std::shared_ptr<PWidget>> mWidgets;
	PWidget*							  mRootWidget;

public:
#if _EDITOR
	DActorSelected ActorClicked;
#endif

	PWorld() = default;
	~PWorld() override = default;

	void Start() override;
	void Tick(float DeltaTime) override;

#if _EDITOR
	void OnActorClicked(PActor* Actor)
	{
		ActorClicked.Broadcast(Actor);
	}
#endif

	template <ENABLE_IF(PObject), typename... ArgsType>
	std::shared_ptr<T> ConstructObject(ArgsType&&... Args)
	{
		std::shared_ptr<T> Object = std::make_shared<T>(std::forward<ArgsType>(Args)...);
		Object->GenerateInternalName();
		return Object;
	}

	template <ENABLE_IF(PActor), typename... ArgsType>
	T* ConstructActor(ArgsType&&... Args)
	{
		auto Actor = ConstructObject<T>(std::forward<ArgsType>(Args)...);
		mActors.push_back(Actor);

#if _EDITOR
		Actor->Clicked.AddRaw(this, &PWorld::OnActorClicked);
#endif
		return Actor.get();
	}

	void DestroyActor(const PActor* Actor);

	template <typename T>
	void RegisterActor(T* Actor)
	{
		mActors.push_back(ConstructObject<T>(Actor));
	}

	template <ENABLE_IF(PActor), typename... ArgsType>
	T* SpawnActor(ArgsType&&... Args)
	{
		auto Actor = ConstructActor<T>(std::forward<ArgsType>(Args)...);
		Actor->Start();
		return Actor;
	}

	std::vector<PActor*> GetActors() const;

	template <typename T>
	std::vector<T*> GetActorsOfType() const
	{
		std::vector<T*> OutActors;
		for (const auto& Actor : mActors)
		{
			if (auto TypedActor = dynamic_cast<T*>(Actor.get()))
			{
				OutActors.push_back(TypedActor);
			}
		}
		return OutActors;
	}

	std::vector<PActor*> GetDrawables(EDrawPriority Priority) const;

	template <typename T, typename... ArgsType>
	T* ConstructComponent(PActor* Owner, ArgsType&&... Args)
	{
		auto Component = ConstructObject<T>(std::forward<ArgsType>(Args)...);
		Owner->AddComponent(Component.get());
		mComponents.push_back(Component);
		return Component.get();
	}

	std::vector<PComponent*> GetComponents() const;

	template <ENABLE_IF(PWidget), typename... ArgsType>
	T* ConstructWidget(ArgsType&&... Args)
	{
		auto Widget = ConstructObject<T>(std::forward<ArgsType>(Args)...);
		mWidgets.push_back(Widget);
		return Widget.get();
	}

	template <typename T>
	void RegisterWidget(T* Widget)
	{
		mWidgets.push_back(ConstructObject<T>(Widget));
	}

	std::vector<PWidget*> GetWidgets() const;

	void	 SetRootWidget(PWidget* Widget) { mRootWidget = Widget; }
	PWidget* GetRootWidget() const { return mRootWidget; }

	PChunk*				 GetChunkAtPosition(const FVector2& Position) const;
	PActor*				 GetActorAtPosition(const FVector2& Position) const;
	std::vector<PActor*> GetActorsAtPosition(const FVector2& Position) const;

	void ProcessEvents(SInputEvent* Event);
};

DECLARE_STATIC_GLOBAL_GETTER(World)

template <typename T, typename... ArgsType>
T* ConstructActor(ArgsType&&... Args)
{
	return GetWorld()->ConstructActor<T>(std::forward<ArgsType>(Args)...);
}

template <typename T>
T* ConstructActor(const json& Json)
{
	auto Actor = ConstructActor<T>();
	Actor->Deserialize(Json);
	return Actor;
}

template <typename T, typename... ArgsType>
T* SpawnActor(ArgsType&&... Args)
{
	auto Actor = ConstructActor<T>(std::forward<ArgsType>(Args)...);
	Actor->Start();
	return Actor;
}

template <typename T>
T* SpawnActor(const json& Json)
{
	auto Actor = ConstructActor<T>(Json);
	Actor->Start();
	return Actor;
}

template <typename T, typename... ArgsType>
T* ConstructComponent(PActor* Owner, ArgsType&&... Args)
{
	return GetWorld()->ConstructComponent<T>(Owner, std::forward<ArgsType>(Args)...);
}

template <typename T>
T* ConstructComponent(PActor* Owner, const json& Json)
{
	auto Component = ConstructComponent<T>(Owner);
	Component->Deserialize(Json);
	return Component;
}

template <typename T, typename... ArgsType>
T* ConstructWidget(ArgsType&&... Args)
{
	return GetWorld()->ConstructWidget<T>(std::forward<ArgsType>(Args)...);
}

template <typename T>
T* ConstructWidget(const json& Json)
{
	auto Widget = ConstructWidget<T>();
	Widget->Deserialize(Json);
	return Widget;
}
