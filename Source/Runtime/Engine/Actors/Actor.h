#pragma once

#include "Core/Delegate.h"
#include "Core/Json.h"
#include "Core/Meta.h"
#include "Core/Vector.h"
#include "Engine/Input.h"
#include "Engine/Object.h"
#include "Engine/Sprite.h"
#include "Interface/Layout.h"
#include "Renderer/Drawable.h"

#include "Selectable.h"

class PActor;
class PComponent;

DECLARE_MULTICAST_DELEGATE(DClicked, PActor*);

struct SActorItem
{
	std::string Name;
	JSON Data;
};

class PActor : public PObject, public ISelectable, public IInputHandler
{
protected:
	PActor* mParent = nullptr;
	FVector2 mPosition;
	FVector2 mSize;
	bool mBlocking = true;
	std::vector<PActor*> mChildren;
	std::vector<PComponent*> mComponents;

	PCollisionComponent* mCollisionComponent = nullptr;

	void OnMouseEvent(SInputEvent* Event) override;

public:
	FVector2 mMousePosition;
	bool mMouseOver = false;
	bool mMouseDown = false;

	DHoverBegin HoverBegin;
	DHoverEnd HoverEnd;
	DClicked Clicked;

	PActor() = default;

	PActor(const PActor& other)
		: PObject{other}, mPosition{other.mPosition}, mSize{other.mSize} {}

	PActor(PActor&& other) noexcept
		: PObject{std::move(other)},
		  mPosition{std::move(other.mPosition)}, mSize{std::move(other.mSize)} {}

	PActor& operator=(const PActor& other)
	{
		if (this == &other)
			return *this;
		PObject::operator=(other);
		mPosition = other.mPosition;
		mSize     = other.mSize;
		return *this;
	}

	PActor& operator=(PActor&& other) noexcept
	{
		if (this == &other)
			return *this;
		PObject::operator=(std::move(other));
		mPosition = std::move(other.mPosition);
		mSize     = std::move(other.mSize);
		return *this;
	}

	PActor* GetParent() const
	{
		return mParent;
	}

	void SetParent(PActor* Parent)
	{
		mParent = Parent;
	}

	void AddChild(PActor* Child)
	{
		if (Child->GetParent() == this)
		{
			return;
		}
		Child->SetParent(this);
		Containers::Add(mChildren, Child);
	}

	void RemoveChild(PActor* Child)
	{
		if (Containers::Contains(mChildren, Child))
		{
			Containers::Remove(mChildren, Child);
		}
	}

	std::vector<PActor*> GetChildren() const
	{
		return mChildren;
	}

	void AddComponent(PComponent* Component);

	template <typename T>
	T* GetComponent()
	{
		for (auto Component : mComponents)
		{
			if (Component)
			{
				return dynamic_cast<T*>(Component);
			}
		}
		return nullptr;
	}

	template <typename T>
	bool HasComponent()
	{
		return GetComponent<T>() != nullptr;
	}

	std::vector<PComponent*> GetComponents() const
	{
		return mComponents;
	}

	virtual FRect GetLocalBounds() const
	{
		return FRect();
	}

	virtual FRect GetWorldBounds() const
	{
		auto Local = GetLocalBounds();
		return {mPosition.X, mPosition.Y, Local.W, Local.H};
	}

	virtual bool IsMouseOver() const
	{
		return mMouseOver;
	}

	virtual FVector2 GetPosition() const
	{
		return mPosition;
	}

	virtual FVector2 GetWorldPosition() const
	{
		if (mParent)
		{
			return mParent->GetWorldPosition() + mPosition;
		}
		return mPosition;
	}

	void SetPosition(const FVector2& Position)
	{
		mPosition = Position;
	}

	void AddPosition(const FVector2& Position)
	{
		mPosition.X += Position.X;
		mPosition.Y += Position.Y;
	}

	FVector2 GetCenter() const
	{
		auto Bounds = GetWorldBounds();
		return {Bounds.X + (Bounds.W / 2.0f), Bounds.Y + (Bounds.H / 2.0f)};
	}

	void MoveToTile(int32_t X, int32_t Y);

	virtual bool IsBlocking() const
	{
		return mBlocking;
	}

	JSON Serialize() const override;
	void Deserialize(const JSON& Data) override;

	// Overlap
	virtual void OnOverlapBegin(PActor* Actor) {}
	virtual void OnOverlapEnd(PActor* Actor) {}

	// Mouse events

	virtual void OnHoverBegin()
	{
		LogInfo("HoverBegin {}", GetInternalName().c_str());
	}

	virtual void OnHoverEnd()
	{
		LogInfo("HoverEnd {}", GetInternalName().c_str());
	}

	virtual void OnClicked()
	{
		LogInfo("Clicked {}", GetInternalName().c_str());
	}
};

#define BEGIN_CONSTRUCT_ACTOR \
auto ClassName   = Data.at("Class").get<std::string>();
#define CONSTRUCT_ACTOR(Class) if (ClassName == PREPEND(P, Class)) { if (PCLASS(Class)* NewActor = ConstructActor<PCLASS(Class)>()) {NewActor->Deserialize(Data); return NewActor;} }

#define CONSTRUCT_EACH_ACTOR(...) FOR_EACH(CONSTRUCT_ACTOR, __VA_ARGS__)

#define MAP_COMPONENT(Component) \
	m##Component = GetComponent<PCLASS(Component)>()
