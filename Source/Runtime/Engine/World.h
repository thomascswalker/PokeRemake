#pragma once

#include <memory>
#include <vector>

#include "Actors/Actor.h"
#include "Actors/Chunk.h"
#include "Components/Component.h"
#include "Interface/Widget.h"

#define ENABLE_IF(Class) class T, class = std::enable_if_t<std::is_base_of_v<Class, T>>

class PWorld : public PObject
{
	// Chunks
	std::map<std::string, PChunk*> mChunks;

	std::vector<std::shared_ptr<PActor>>	 mActors;
	std::vector<std::shared_ptr<PComponent>> mComponents;

	std::vector<std::shared_ptr<PWidget>> mWidgets;
	PWidget*							  mRootWidget;

public:
	PWorld() = default;
	~PWorld() override = default;

	void Start() override;
	void Tick(float DeltaTime) override;
	void ConstructChunks();

	template <ENABLE_IF(PActor), typename... ArgsType>
	T* ConstructActor(ArgsType&&... Args)
	{
		std::shared_ptr<T> Actor = std::make_shared<T>(std::forward<ArgsType>(Args)...);
		mActors.push_back(Actor);
		return Actor.get();
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

	std::vector<IDrawable*> GetDrawables(EDrawPriority Priority) const
	{
		std::vector<IDrawable*> Drawables;
		for (const auto& Actor : mActors)
		{
			if (auto Drawable = static_cast<IDrawable*>(Actor.get()))
			{
				if (Drawable->GetPriority() == Priority)
				{
					Drawables.push_back(Drawable);
				}
			}
		}
		return Drawables;
	}

	template <typename T, typename... ArgsType>
	T* ConstructComponent(PActor* Owner, ArgsType&&... Args)
	{
		auto Component = std::make_shared<T>(std::forward<ArgsType>(Args)...);
		static_assert(std::is_base_of_v<PComponent, T>, "T must be derived from PComponent");
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
		std::shared_ptr<T> Widget = std::make_shared<T>(std::forward<ArgsType>(Args)...);
		mWidgets.push_back(Widget);
		return Widget.get();
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
};

DECLARE_STATIC_GLOBAL_GETTER(World)
