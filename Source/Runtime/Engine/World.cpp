#include "World.h"

#include "Actors/Character.h"
#include "Core/Logging.h"
#include "Engine/Input.h"
#include "Interface/Layout.h"

#include "../Interface/Game/GameHUD.h"

void PWorld::Start()
{
	LogDebug("Starting world");

	LogDebug("Starting {} actors.", GetActors().size());
	for (const auto& Actor : GetActors())
	{
		Actor->Start();
	}
	LogDebug("Starting {} components.", GetComponents().size());
	for (const auto& Component : GetComponents())
	{
		Component->Start();
	}
}

void PWorld::Tick(float DeltaTime)
{
	auto R = GetRenderer();

	// If there's a root widget, set it to span the width and height of the screen
	// and layout all of its children recursively.
	if (mHUD)
	{
		// Resize the main root widget to fit the screen size
		auto ScreenSize = R->GetScreenSize();
		mHUD->SetFixedSize({ScreenSize.X, ScreenSize.Y});

		// Recursively construct the layout of all widgets
		Layout::Layout(mHUD.get());
	}

	auto Comps = GetComponents();
	for (auto& Component : GetComponents())
	{
		Component->Tick(DeltaTime);
	}

	for (auto& Actor : GetActors())
	{
		Actor->Tick(DeltaTime);
	}
}

void PWorld::DestroyActor(PActor* Actor)
{
	mDestroyableActors.emplace_back(Actor);
	for (auto Child : Actor->GetChildren())
	{
		DestroyActor(Child);
	}
}

void PWorld::DestroyActorInternal(const PActor* Actor)
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

	for (auto Component : Actor->GetComponents())
	{
		DestroyComponentInternal(Component);
	}

	mActors.erase(std::ranges::find(mActors, SharedActor));
}

void PWorld::DestroyComponentInternal(const PComponent* Component)
{
	std::shared_ptr<PComponent> SharedComponent;
	for (auto Ptr : mComponents)
	{
		if (Ptr.get() == Component)
		{
			SharedComponent = Ptr;
			break;
		}
	}
	if (!SharedComponent)
	{
		return;
	}

	mComponents.erase(std::ranges::find(mComponents, SharedComponent));
}

std::vector<PActor*> PWorld::GetActors() const
{
	std::vector<PActor*> Actors;
	for (const auto& Actor : mActors)
	{
		Actors.push_back(Actor.get());
	}
	return Actors;
}

std::vector<PActor*> PWorld::GetDrawables(EDrawPriority Priority) const
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

std::vector<PComponent*> PWorld::GetComponents() const
{
	std::vector<PComponent*> Components;
	for (const auto& Component : mComponents)
	{
		Components.push_back(Component.get());
	}
	return Components;
}

std::vector<PWidget*> PWorld::GetWidgets() const
{
	std::vector<PWidget*> Widgets;
	for (const auto& Widget : mWidgets)
	{
		Widgets.push_back(Widget.get());
	}
	return Widgets;
}

PPlayerCharacter* PWorld::GetPlayerCharacter() const
{
	return mPlayerCharacter;
}

void PWorld::SetPlayerCharacter(PPlayerCharacter* PlayerCharacter)
{
	mPlayerCharacter = PlayerCharacter;
}

PMap* PWorld::GetMapAtPosition(const FVector2& Position) const
{
	for (const auto& Actor : GetActors())
	{
		if (const auto Map = dynamic_cast<PMap*>(Actor))
		{
			auto Bounds = Map->GetWorldBounds();
			Bounds.W *= 2.0f;
			Bounds.H *= 2.0f;
			if (Bounds.Contains(Position))
			{
				return Map;
			}
		}
	}
	return nullptr;
}

PActor* PWorld::GetActorAtPosition(const FVector2& Position) const
{
	for (const auto& Actor : mActors)
	{
		// Skip actors that are not characters
		if (dynamic_cast<PMap*>(Actor.get()))
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

std::vector<PActor*> PWorld::GetActorsAtPosition(const FVector2& Position) const
{
	std::vector<PActor*> OutActors;
	for (auto Actor : mActors)
	{
		if (dynamic_cast<PMap*>(Actor.get()))
		{
			continue;
		}
		if (Actor->GetWorldBounds().Contains(Position))
		{
			OutActors.push_back(Actor.get());
		}
	}
	return OutActors;
}

void PWorld::ProcessEvents(SInputEvent* Event)
{
	// First process all widget events as these are the layer 'above' the game view
	if (mHUD)
	{
		if (mHUD->ProcessEvents(Event))
		{
			return;
		}
	}

	// Next process all actors and their components
	for (const auto& Actor : GetActors())
	{
		if (Actor->ProcessEvents(Event))
		{
			return;
		}
		for (const auto& Component : Actor->GetComponents())
		{
			if (Component->ProcessEvents(Event))
			{
				return;
			}
		}
	}
}

void PWorld::Cleanup()
{
	for (auto Actor : mDestroyableActors)
	{
		DestroyActorInternal(Actor);
		Actor = nullptr;
	}
	mDestroyableActors.clear();
}
