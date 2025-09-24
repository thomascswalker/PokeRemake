#include "World.h"

#include "../../Game/Interface/GameHUD.h"
#include "Actors/Character.h"
#include "Core/Logging.h"
#include "Engine/Input.h"
#include "Interface/Layout.h"

#include "MapManager.h"

PWorld* GWorld = nullptr;

bool PWorld::Start()
{
	LogDebug("Starting world");

	LogDebug("Starting {} actors.", GetActors().size());
	for (const auto& Actor : GetActors())
	{
		if (!Actor->Start())
		{
			LogError("Failed to start actor");
			return false;
		}
	}
	LogDebug("Starting {} components.", GetComponents().size());
	for (const auto& Component : GetComponents())
	{
		if (!Component->Start())
		{
			LogError("Failed to start component");
			return false;
		}
	}

	return true;
}

bool PWorld::End()
{
	mMapManager.ClearMaps();
	mTimerManager.ClearAllTimers();
	return true;
}

void PWorld::Tick(float DeltaTime)
{
	mTimerManager.Tick(DeltaTime);

	// Set the root widget to span the width and height of the screen
	// and layout all of its children recursively.
	auto ScreenSize = GRenderer->GetScreenSize();
	mRootWidget->SetFixedSize({ ScreenSize.X, ScreenSize.Y });

	// Recursively construct the layout of all widgets
	Layout::Layout(mRootWidget.get());

	// Tick each widget
	for (const auto& W : mWidgets)
	{
		W->Tick(DeltaTime);
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
	GGameInstance->DestroyObject(Actor);
	Actor->Destroyed.Broadcast(Actor);
	for (auto Component : Actor->GetComponents())
	{
		DestroyComponent(Component);
	}
	for (auto Child : Actor->GetChildren())
	{
		DestroyActor(Child);
	}
	mActors.erase(std::ranges::find(mActors, Actor));
}

void PWorld::DestroyAllActors()
{
	for (auto Actor : mActors)
	{
		DestroyActor(Actor);
	}
}

PWorld::PWorld()
{
	mRootWidget = std::make_shared<PWidget>();
}

std::vector<PActor*> PWorld::GetActors() const
{
	std::vector<PActor*> Actors;
	for (const auto& Actor : mActors)
	{
		Actors.push_back(Actor);
	}
	return Actors;
}

std::vector<IDrawable*> PWorld::GetDrawables() const
{
	std::vector<IDrawable*> Drawables;
	for (const auto& Actor : mActors)
	{
		if (auto Drawable = dynamic_cast<IDrawable*>(Actor))
		{
			Drawables.push_back(Drawable);
		}
	}

	for (const auto& Component : mComponents)
	{
		if (auto Drawable = dynamic_cast<IDrawable*>(Component))
		{
			Drawables.push_back(Drawable);
		}
	}

	auto DepthSort = [](const IDrawable* A, const IDrawable* B) {
		return A->GetDepth() < B->GetDepth();
	};

	std::ranges::sort(Drawables, DepthSort);
	return Drawables;
}

void PWorld::DestroyComponent(PComponent* Component)
{
	GGameInstance->DestroyObject(Component);
	mComponents.erase(std::ranges::find(mComponents, Component));
}
std::vector<PComponent*> PWorld::GetComponents() const
{
	std::vector<PComponent*> Components;
	for (const auto& Component : mComponents)
	{
		Components.push_back(Component);
	}
	return Components;
}

std::vector<PWidget*> PWorld::GetWidgets() const
{
	std::vector<PWidget*> Widgets;
	for (const auto& Widget : mWidgets)
	{
		Widgets.push_back(Widget);
	}
	return Widgets;
}

void PWorld::DestroyWidget(PWidget* Widget)
{
	GGameInstance->DestroyObject(Widget);
	for (auto Child : Widget->GetChildren())
	{
		DestroyWidget(Child);
	}
	mWidgets.erase(std::ranges::find(mWidgets, Widget));
}

PPlayerCharacter* PWorld::GetPlayerCharacter() const
{
	return mPlayerCharacter;
}

void PWorld::SetPlayerCharacter(PPlayerCharacter* PlayerCharacter)
{
	mPlayerCharacter = PlayerCharacter;
}

PActor* PWorld::GetActorAtPosition(const FVector2& Position) const
{
	for (const auto& Actor : mActors)
	{
		// Skip actors that are not characters
		if (dynamic_cast<PGameMap*>(Actor))
		{
			continue;
		}
		if (Actor->GetPosition2D() == Position)
		{
			return Actor;
		}
	}
	return nullptr;
}

std::vector<PActor*> PWorld::GetActorsAtPosition(const FVector2& Position) const
{
	std::vector<PActor*> OutActors;
	for (auto Actor : mActors)
	{
		if (Actor->GetWorldBounds().Contains(Position))
		{
			OutActors.push_back(Actor);
		}
	}
	return OutActors;
}

bool PWorld::ProcessEvents(SInputEvent* Event)
{
	// First process all widget events as these are the layer 'above' the game view
	if (mRootWidget)
	{
		if (mRootWidget->ProcessEvents(Event))
		{
			return true;
		}
	}

	// Next process all actors and their components
	for (const auto& Actor : GetActors())
	{
		if (Actor->ProcessEvents(Event))
		{
			return true;
		}
		for (const auto& Component : Actor->GetComponents())
		{
			if (Component->ProcessEvents(Event))
			{
				return true;
			}
		}
	}

	return false;
}

#if _EDITOR
TArray<PActor*> PWorld::GetSelectableActors() const
{
	TArray<PActor*> Actors;
	for (const auto& Actor : mActors)
	{
		if (!Actor->GetSelectable())
		{
			continue;
		}
		Actors.Add(Actor);
	}
	return Actors;
}
#endif
