#include "Actor.h"

#include "Engine/CameraView.h"
#include "Engine/Components/Component.h"
#include "Engine/Serialization.h"
#include "Engine/World.h"
#include "Engine/Components/CollisionComponent.h"

void PActor::OnMouseEvent(SInputEvent* Event)
{
	const auto Renderer = GetRenderer();
	mMousePosition      = Event->MousePosition;

	// Convert the current world bounds of this actor to a screen position
	FRect WorldRect = GetWorldBounds();
	FRect ScreenRect;

	Renderer->WorldToScreen(WorldRect, &ScreenRect);

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
	}
}

void PActor::AddComponent(PComponent* Component)
{
	Component->SetOwner(this);
	mComponents.push_back(Component);
}

IDrawable* PActor::GetDrawableComponent() const
{
	for (auto Component : mComponents)
	{
		if (auto Inst = dynamic_cast<IDrawable*>(Component))
		{
			return Inst;
		}
	}
	return nullptr;
}

void PActor::MoveToTile(int32_t X, int32_t Y)
{
	auto NewPosition = FVector2(X * BLOCK_SIZE, Y * BLOCK_SIZE);
	mPosition.X      = NewPosition.X;
	mPosition.Y      = NewPosition.Y;
}

JSON PActor::Serialize() const
{
	JSON Result        = PObject::Serialize();
	Result["Position"] = {mPosition.X, mPosition.Y};
	Result["Depth"]    = mPosition.Z;
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
		mPosition.X   = Position[0].get<float>();
		mPosition.Y   = Position[1].get<float>();
	}

	if (Data.contains("Depth"))
	{
		mPosition.Z = Data.at("Depth").get<float>();
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
			if (auto NewComponent = Serialization::DeserializeComponent(Component, this))
			{
				AddComponent(NewComponent);
				LogDebug("Deserialized component: {}", NewComponent->GetClassName().c_str());
			}
		}
	}
	else
	{
		LogDebug("No components for {}.", mInternalName.c_str());
	}
	LogDebug("Finished deserializing {}", mInternalName.c_str());
}
