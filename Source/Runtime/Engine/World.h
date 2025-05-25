#pragma once

#include <memory>
#include <vector>

#include "Actors/Actor.h"
#include "Actors/Grid.h"

class PWorld : public PObject
{
	std::unique_ptr<PGrid>				 mGrid;
	std::vector<std::shared_ptr<PActor>> mActors;

public:
	PWorld();
	~PWorld() = default;

	PGrid* GetGrid() const { return mGrid.get(); }

	template <class T, class = std::enable_if_t<std::is_base_of_v<PActor, T>>, typename... ArgsType>
	void ConstructActor(ArgsType... Args)
	{
		auto Actor = std::make_shared<T>(std::forward<ArgsType>(Args)...);
		mActors.push_back(Actor);
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
};
