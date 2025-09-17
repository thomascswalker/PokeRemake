#include "World.h"

#include "../Interface/Game/GameHUD.h"
#include "Actors/Character.h"
#include "Core/Logging.h"
#include "Engine/Input.h"
#include "Interface/Layout.h"

void PWorld::Start()
{
	LogDebug("Starting world");
	//
	// LogDebug("Constructing timer manager.");
	// mTimerManager = std::make_shared<PTimerManager>();

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
		mHUD->SetFixedSize({ ScreenSize.X, ScreenSize.Y });

		// Recursively construct the layout of all widgets
		Layout::Layout(mHUD.get());

		for (const auto& W : mWidgets)
		{
			W->Tick(DeltaTime);
		}
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

void PWorld::PostTick()
{
	for (auto Actor : mDestroyableActors)
	{
		DestroyActorInternal(Actor);
	}
	mDestroyableActors.clear();

	for (auto Widget : mDestroyableWidgets)
	{
		DestroyWidgetInternal(Widget);
	}
	mDestroyableWidgets.clear();
}

void PWorld::DestroyObject(PObject* Object)
{
	mDestroyableObjects.emplace_back(Object);
}

void PWorld::DestroyActor(PActor* Actor)
{
	mDestroyableActors.emplace_back(Actor);
	Actor->Destroyed.Broadcast(Actor);
	for (auto Child : Actor->GetChildren())
	{
		DestroyActor(Child);
	}
}

void PWorld::DestroyAllActors()
{
	for (auto Actor : mActors)
	{
		DestroyActor(Actor.get());
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
void PWorld::DestroyWidgetInternal(PWidget* Widget)
{
	std::shared_ptr<PWidget> SharedWidget;
	for (auto Ptr : mWidgets)
	{
		if (Ptr.get() == Widget)
		{
			SharedWidget = Ptr;
			break;
		}
	}
	if (!SharedWidget)
	{
		return;
	}

	mWidgets.erase(std::ranges::find(mWidgets, SharedWidget));
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

std::vector<IDrawable*> PWorld::GetDrawables() const
{
	std::vector<IDrawable*> Drawables;
	for (const auto& Actor : mActors)
	{
		if (auto Drawable = dynamic_cast<IDrawable*>(Actor.get()))
		{
			Drawables.push_back(Drawable);
		}
	}

	for (const auto& Component : mComponents)
	{
		if (auto Drawable = dynamic_cast<IDrawable*>(Component.get()))
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

void PWorld::DestroyWidget(PWidget* Widget)
{
	mDestroyableWidgets.emplace_back(Widget);
	for (auto Child : Widget->GetChildren())
	{
		DestroyWidget(Child);
	}
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
		if (dynamic_cast<PGameMap*>(Actor.get()))
		{
			continue;
		}
		if (Actor->GetPosition2D() == Position)
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
		if (Actor->GetWorldBounds().Contains(Position))
		{
			OutActors.push_back(Actor.get());
		}
	}
	return OutActors;
}

bool PWorld::ProcessEvents(SInputEvent* Event)
{
	// First process all widget events as these are the layer 'above' the game view
	if (mHUD)
	{
		if (mHUD->ProcessEvents(Event))
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
		Actors.Add(Actor.get());
	}
	return Actors;
}
#endif
