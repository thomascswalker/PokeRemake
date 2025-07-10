#include "World.h"

#include "Core/Logging.h"
#include "Engine/InputManager.h"
#include "Interface/Layout.h"

#include <ranges>

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

	// Resize the main root widget to fit the screen size
	auto ScreenSize = R->GetScreenSize();
	mRootWidget->SetFixedSize({ ScreenSize.X, ScreenSize.Y });

	// Recursively construct the layout of all widgets
	Layout::Layout(mRootWidget);

	for (auto& Actor : GetActors())
	{
		Actor->Tick(DeltaTime);
	}
}

PChunk* PWorld::GetChunkAtPosition(const FVector2& Position) const
{
	for (const auto& Actor : GetActors())
	{
		if (const auto Chunk = dynamic_cast<PChunk*>(Actor))
		{
			auto Bounds = Chunk->GetWorldBounds();
			if (Bounds.Contains(Position))
			{
				return Chunk;
			}
		}
	}
	return nullptr;
}

void PWorld::ProcessEvents(SInputEvent* Event)
{
	// First process all widget events as these are the layer 'above' the game view
	if (mRootWidget->ProcessEvents(Event))
	{
		LogInfo("Event consumed from {}", PWidget::GetSender()->GetInternalName().c_str());
		return;
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
