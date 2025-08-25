#pragma once

#include <memory>
#include <vector>

#include "Actors/GameMap.h"
#include "Actors/PlayerCharacter.h"
#include "Components/Component.h"
#include "Interface/Widget.h"
#include "Interface/HUD.h"

#if _EDITOR
#include "../../Editor/EditorView.h"
#include "Components/SelectionComponent.h"
DECLARE_MULTICAST_DELEGATE(DActorSelected, PActor*);
#endif

class PWorld : public PObject
{
	PPlayerCharacter* mPlayerCharacter = nullptr;
	std::vector<std::shared_ptr<PActor>> mActors;
	std::vector<std::shared_ptr<PComponent>> mComponents;

	std::vector<PActor*> mDestroyableActors;

	std::vector<std::shared_ptr<PWidget>> mWidgets;
	std::shared_ptr<PHUD> mHUD;

	void DestroyActorInternal(const PActor* Actor);
	void DestroyComponentInternal(const PComponent* Component);

public:
#if _EDITOR
	DActorSelected ActorClicked;
#endif

	~PWorld() override = default;

	void Start() override;
	void Tick(float DeltaTime) override;
	void PostTick() override;

#if _EDITOR
	void OnActorClicked(PActor* Actor)
	{
		ActorClicked.Broadcast(Actor);
	}
#endif

	template <IS_SUBCLASS_OF(PObject), typename... ArgsType>
	std::shared_ptr<T> ConstructObject(ArgsType&&... Args)
	{
		std::shared_ptr<T> Object = std::make_shared<T>(std::forward<ArgsType>(Args)...);
		Object->GenerateInternalName();
		return Object;
	}

	template <IS_SUBCLASS_OF(PActor), typename... ArgsType>
	T* ConstructActor(ArgsType&&... Args)
	{
		auto Actor = ConstructObject<T>(std::forward<ArgsType>(Args)...);
		mActors.push_back(Actor);

#if _EDITOR
		Actor->Clicked.AddRaw(this, &PWorld::OnActorClicked);

		if (Actor->GetSelectable())
		{
			auto SelectionComponent = ConstructComponent<PSelectionComponent>(Actor.get());
			Actor->SetSelectionComponent(SelectionComponent);
		}

#endif
		return Actor.get();
	}

	void DestroyActor(PActor* Actor);

	void DestroyAllActors();

	template <typename T>
	void RegisterActor(T* Actor)
	{
		mActors.push_back(ConstructObject<T>(Actor));
	}

	template <IS_SUBCLASS_OF(PActor), typename... ArgsType>
	T* SpawnActor(ArgsType&&... Args)
	{
		auto Actor = ConstructActor<T>(std::forward<ArgsType>(Args)...);
		Actor->Start();
		return Actor;
	}

	std::vector<PActor*> GetActors() const;

	template <typename T>
	std::vector<T*> GetActorsOfType() const
	{
		std::vector<T*> OutActors;
		for (const auto& Actor : mActors)
		{
			if (auto TypedActor = dynamic_cast<T*>(Actor.get()))
			{
				OutActors.push_back(TypedActor);
			}
		}
		return OutActors;
	}

	std::vector<IDrawable*> GetDrawables() const;

	template <typename T, typename... ArgsType>
	T* ConstructComponent(PActor* Owner, ArgsType&&... Args)
	{
		if (T* ExistingComponent = Owner->GetComponent<T>())
		{
			return ExistingComponent;
		}
		auto Component = ConstructObject<T>(std::forward<ArgsType>(Args)...);
		Owner->AddComponent(Component.get());
		mComponents.push_back(Component);
		return Component.get();
	}

	std::vector<PComponent*> GetComponents() const;

	template <IS_SUBCLASS_OF(PWidget), typename... ArgsType>
	T* ConstructWidget(ArgsType&&... Args)
	{
		auto Widget = ConstructObject<T>(std::forward<ArgsType>(Args)...);
		mWidgets.push_back(Widget);
		return Widget.get();
	}

	template <typename T>
	void RegisterWidget(T* Widget)
	{
		mWidgets.push_back(ConstructObject<T>(Widget));
	}

	std::vector<PWidget*> GetWidgets() const;

	template <typename T = PHUD>
	T* CreateHUD()
	{
		mHUD = std::make_shared<T>();
		return dynamic_cast<T*>(mHUD.get());
	}

	template <typename T = PHUD>
	T* GetHUD() const
	{
		return static_cast<T*>(mHUD.get());
	}

	PPlayerCharacter* GetPlayerCharacter() const;
	void SetPlayerCharacter(PPlayerCharacter* PlayerCharacter);
	PActor* GetActorAtPosition(const FVector2& Position) const;
	std::vector<PActor*> GetActorsAtPosition(const FVector2& Position) const;

	void ProcessEvents(SInputEvent* Event);

#if _EDITOR
	TArray<PActor*> GetSelectableActors() const;
#endif
};

DECLARE_STATIC_GLOBAL_GETTER(World)

template <typename T, typename... ArgsType>
T* ConstructActor(ArgsType&&... Args)
{
	return GetWorld()->ConstructActor<T>(std::forward<ArgsType>(Args)...);
}

template <typename T>
T* ConstructActor(const JSON& Json)
{
	auto Actor = ConstructActor<T>();
	Actor->Deserialize(Json);
	return Actor;
}

template <typename T, typename... ArgsType>
T* SpawnActor(ArgsType&&... Args)
{
	auto Actor = ConstructActor<T>(std::forward<ArgsType>(Args)...);
	Actor->Start();
	return Actor;
}

template <typename T>
T* SpawnActor(const JSON& Json)
{
	auto Actor = ConstructActor<T>(Json);
	Actor->Start();
	return Actor;
}

template <typename T, typename... ArgsType>
T* ConstructComponent(PActor* Owner, ArgsType&&... Args)
{
	return GetWorld()->ConstructComponent<T>(Owner, std::forward<ArgsType>(Args)...);
}

template <typename T>
T* ConstructComponent(PActor* Owner, const JSON& Json)
{
	auto Component = ConstructComponent<T>(Owner);
	Component->Deserialize(Json);
	return Component;
}

template <typename T, typename... ArgsType>
T* ConstructWidget(ArgsType&&... Args)
{
	return GetWorld()->ConstructWidget<T>(std::forward<ArgsType>(Args)...);
}

template <typename T>
T* ConstructWidget(const JSON& Json)
{
	auto Widget = ConstructWidget<T>();
	Widget->Deserialize(Json);
	return Widget;
}

template <typename T>
T* GetHUD()
{
	return GetWorld()->GetHUD<T>();
}
