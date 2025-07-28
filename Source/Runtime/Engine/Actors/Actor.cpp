#include "Actor.h"

#include "Engine/CameraView.h"
#include "Engine/Components/Component.h"
#include "Engine/Serialization.h"
#include "Engine/World.h"
#include "Engine/Components/CollisionComponent.h"
#include "Engine/Components/SpriteComponent.h"

void PActor::OnMouseEvent(SInputEvent* Event)
{
	const auto Renderer = GetRenderer();
	mMousePosition      = Event->MousePosition;
	FVector2 ScreenPosition;
	Renderer->WorldToScreen(GetPosition(), &ScreenPosition);

	auto CameraView  = GetCameraView();
	auto Rect        = GetWorldBounds();
	FRect ScreenRect = {
		ScreenPosition.X,
		ScreenPosition.Y,
		Rect.W * CameraView->GetZoom(),
		Rect.H * CameraView->GetZoom(),
	};
	const bool NewMouseState = ScreenRect.Contains(mMousePosition);

	if (!mMouseOver && NewMouseState)
	{
		HoverBegin.Broadcast();
		mMouseOver = true;
	}
	else if (mMouseOver && !NewMouseState)
	{
		HoverEnd.Broadcast();
		mMouseOver = false;
	}

	bool NewClickState = Renderer->GetMouseLeftDown();
	if (!mMouseDown && NewClickState && mMouseOver)
	{
		mMouseDown = true;
	}
	else if (mMouseDown && !NewClickState && mMouseOver)
	{
		mMouseDown = false;
		Clicked.Broadcast(this);
		Event->Consume();
	}
}

void PActor::AddComponent(PComponent* Component)
{
	Component->SetOwner(this);
	mComponents.push_back(Component);
}

void PActor::MoveToTile(int32_t X, int32_t Y)
{
	mPosition = FVector2(X * BLOCK_SIZE, Y * BLOCK_SIZE);
}

JSON PActor::Serialize() const
{
	JSON Result        = PObject::Serialize();
	Result["Position"] = {mPosition.X, mPosition.Y};
	if (mChildren.size() > 0)
	{
		Result["Children"] = {};
		for (auto Child : mChildren)
		{
			if (Child->IsSerializable())
			{
				Result["Children"].push_back(Child->Serialize());
			}
		}
	}
	if (mComponents.size() > 0)
	{
		Result["Components"] = {};
		for (auto Component : mComponents)
		{
			if (Component->IsSerializable())
			{
				Result["Components"].push_back(Component->Serialize());
			}
		}
	}
	return Result;
}

void PActor::Deserialize(const JSON& Data)
{
	PObject::Deserialize(Data);

	if (Data.contains("Position"))
	{
		auto Position = Data.at("Position");
		mPosition.X   = Position[0].get<int32_t>();
		mPosition.Y   = Position[1].get<int32_t>();
	}

	if (Data.contains("Children"))
	{
		auto Children = Data.at("Children");
		auto Count    = Children.size();
		LogDebug("Deserializing {} children for {}.", Count, mInternalName.c_str());
		for (auto& Child : Children)
		{
			if (auto ChildActor = Serialization::DeserializeActor(Child))
			{
				AddChild(ChildActor);
			}
		}
	}
	else
	{
		LogDebug("No children for {}.", mInternalName.c_str());
	}

	if (Data.contains("Components"))
	{
		auto Components = Data.at("Components");
		auto Count      = Components.size();
		LogDebug("Deserializing {} components for {}.", Count, mInternalName.c_str());
		for (auto& Component : Components)
		{
			Serialization::DeserializeComponent(Component, this);
			LogDebug("Deserialized component: {}", mInternalName.c_str());
		}
	}
	else
	{
		LogDebug("No components for {}.", mInternalName.c_str());
	}
	LogDebug("Finished deserializing {}", mInternalName.c_str());
}
