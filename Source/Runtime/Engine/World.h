#pragma once

#include <memory>
#include <vector>

#include "Actors/Actor.h"
#include "Actors/Character.h"
#include "Actors/Chunk.h"
#include "Components/Component.h"
#include "Interface/Widget.h"

#define ENABLE_IF(Class) class T, class = std::enable_if_t<std::is_base_of_v<Class, T>>

#if _EDITOR
DECLARE_MULTICAST_DELEGATE(DActorSelected, PActor*);
#endif

class PWorld : public PObject
{
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

	void OnActorClicked(PActor* Actor)
	{
		ActorClicked.Broadcast(Actor);
	}

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
#ifdef _EDITOR
		Actor->Clicked.AddRaw(this, &PWorld::OnActorClicked);
#endif
		return Actor.get();
	}

	void DestroyActor(const PActor* Actor)
	{
		if (!Actor)
		{
			return;
		}

		std::shared_ptr<PActor> SharedActor;
		for (auto Ptr : mActors)
		{
			if (Ptr.get() == Actor)
			{
				SharedActor = Ptr;
				break;
			}
		}
		if (!SharedActor)
		{
			return;
		}

		mActors.erase(std::ranges::find(mActors, SharedActor));
	}

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

	std::vector<PActor*> GetActors() const
	{
		std::vector<PActor*> Actors;
		for (const auto& Actor : mActors)
		{
			Actors.push_back(Actor.get());
		}
		return Actors;
	}

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

	std::vector<PActor*> GetDrawables(EDrawPriority Priority) const
	{
		std::vector<PActor*> Drawables;
		for (const auto& Actor : mActors)
		{
			if (auto Drawable = static_cast<IDrawable*>(Actor.get()))
			{
				if (Drawable->GetPriority() == Priority)
				{
					Drawables.push_back(Actor.get());
				}
			}
		}
		return Drawables;
	}

	template <typename T, typename... ArgsType>
	T* ConstructComponent(PActor* Owner, ArgsType&&... Args)
	{
		auto Component = ConstructObject<T>(std::forward<ArgsType>(Args)...);
		Component->SetOwner(Owner);
		mComponents.push_back(Component);
		return Component.get();
	}

	std::vector<PComponent*> GetComponents() const
	{
		std::vector<PComponent*> Components;
		for (const auto& Component : mComponents)
		{
			Components.push_back(Component.get());
		}
		return Components;
	}

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

	std::vector<PWidget*> GetWidgets() const
	{
		std::vector<PWidget*> Widgets;
		for (const auto& Widget : mWidgets)
		{
			Widgets.push_back(Widget.get());
		}
		return Widgets;
	}

	void	 SetRootWidget(PWidget* Widget) { mRootWidget = Widget; }
	PWidget* GetRootWidget() const { return mRootWidget; }

	PChunk* GetChunkAtPosition(const FVector2& Position) const;
	PActor* GetCharacterAtPosition(const FVector2& Position) const
	{
		for (const auto& Actor : mActors)
		{
			// Skip actors that are not characters
			if (!dynamic_cast<PCharacter*>(Actor.get()))
			{
				continue;
			}
			if (Actor->GetPosition() == Position)
			{
				return Actor.get();
			}
		}
		return nullptr;
	}
};

DECLARE_STATIC_GLOBAL_GETTER(World)
