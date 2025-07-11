#include "World.h"

#include "Core/Logging.h"
#include "Engine/InputManager.h"
#include "Interface/Layout.h"

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
	if (mRootWidget)
	{
		// Resize the main root widget to fit the screen size
		auto ScreenSize = R->GetScreenSize();
		mRootWidget->SetFixedSize({ ScreenSize.X, ScreenSize.Y });

		// Recursively construct the layout of all widgets
		Layout::Layout(mRootWidget);
	}

	for (auto& Actor : GetActors())
	{
		Actor->Tick(DeltaTime);
		for (auto& Component : Actor->GetComponents())
		{
			Component->Tick(DeltaTime);
		}
	}
}
void PWorld::DestroyActor(const PActor* Actor)
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

PChunk* PWorld::GetChunkAtPosition(const FVector2& Position) const
{
	for (const auto& Actor : GetActors())
	{
		if (const auto Chunk = dynamic_cast<PChunk*>(Actor))
		{
			auto Bounds = Chunk->GetWorldBounds();
			Bounds.W *= 2.0f;
			Bounds.H *= 2.0f;
			LogDebug("Chunk bounds: {}", Bounds.ToString().c_str());
			if (Bounds.Contains(Position))
			{
				return Chunk;
			}
		}
	}
	LogError("No chunk found at: {}", Position.ToString().c_str());
	return nullptr;
}
PActor* PWorld::GetCharacterAtPosition(const FVector2& Position) const
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

void PWorld::ProcessEvents(SInputEvent* Event)
{
	// First process all widget events as these are the layer 'above' the game view
	if (mRootWidget)
	{
		if (mRootWidget->ProcessEvents(Event))
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
