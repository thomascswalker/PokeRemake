#include "Actor.h"

#include "Engine/CameraView.h"
#include "Engine/Components/Component.h"

void PActor::OnMouseEvent(SInputEvent* Event)
{
	const auto Renderer = GetRenderer();
	mMousePosition = Event->MousePosition;
	FVector2 ScreenPosition;
	Renderer->WorldToScreen(GetPosition(), &ScreenPosition);

	auto  CameraView = GetCameraView();
	auto  Rect = GetWorldBounds();
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

json PActor::Serialize() const
{
	json Result = {
		{ "Class",	   GetClassName()				  },
		{ "Position", { mPosition.X, mPosition.Y } },
	};
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
	return Result;
}

void PActor::Deserialize(const json& Data)
{
	PObject::Deserialize(Data);

	CHECK_PROPERTY(Data, Position);
	auto Position = Data.at("Position");
	mPosition.X = Position[0].get<int32_t>();
	mPosition.Y = Position[1].get<int32_t>();

	if (Data.contains("Children"))
	{
		for (auto& Child : Data.at("Children"))
		{
			if (auto ChildActor = PSerializer::Deserialize(Child))
			{
				AddChild(ChildActor);
			}
		}
	}
}
