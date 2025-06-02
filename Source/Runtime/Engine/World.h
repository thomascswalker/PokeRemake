#pragma once

#include <memory>
#include <vector>

#include "Actors/Actor.h"
#include "Actors/Grid.h"
#include "Components/CameraComponent.h"
#include "Components/Component.h"

#define ENABLE_IF(Class) class T, class = std::enable_if_t<std::is_base_of_v<Class, T>>

class PWorld : public PObject
{
	std::unique_ptr<PGrid>					 mGrid;
	std::vector<std::shared_ptr<PActor>>	 mActors;
	std::vector<std::shared_ptr<PComponent>> mComponents;

public:
	PWorld() {}
	~PWorld() override = default;

	void Tick(float DeltaTime) override;

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
	std::vector<IDrawable*> GetDrawables() const
	{
		std::vector<IDrawable*> Drawables;
		for (const auto& Actor : mActors)
		{
			if (auto Drawable = static_cast<IDrawable*>(Actor.get()))
			{
				Drawables.push_back(Drawable);
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
};

PWorld* GetWorld();