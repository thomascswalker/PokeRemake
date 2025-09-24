#pragma once

#include <memory>
#include <vector>

#include "Actors/PlayerCharacter.h"
#include "Components/Component.h"
#include "Interface/Widget.h"

#include "GameInstance.h"
#include "MapManager.h"
#include "Timer.h"

#if _EDITOR
#include "../../Editor/Actors/EditorView.h"
#include "Components/SelectionComponent.h"
DECLARE_MULTICAST_DELEGATE(DActorSelected, PActor*);
#endif

class PWorld : public PObject, public IInputHandler
{
	PPlayerCharacter*		 mPlayerCharacter = nullptr;
	std::vector<PActor*>	 mActors;
	std::vector<PComponent*> mComponents;

	PTimerManager mTimerManager;
	PMapManager	  mMapManager;

	std::shared_ptr<PWidget> mRootWidget;
	std::vector<PWidget*>	 mWidgets;

public:
	PWorld();
	~PWorld() override = default;

	bool Start() override;
	bool End() override;
	void Tick(float DeltaTime) override;

#if _EDITOR
	DActorSelected ActorClicked;
	void		   OnActorClicked(PActor* Actor)
	{
		ActorClicked.Broadcast(Actor);
	}
#endif

	template <IS_SUBCLASS_OF(PObject), typename... ArgsType>
	T* ConstructObject(ArgsType&&... Args)
	{
		T* Object = GGameInstance->ConstructObject<T>(std::forward<ArgsType>(Args)...);
		Object->GenerateInternalName();
		return Object;
	}

	template <IS_SUBCLASS_OF(PActor), typename... ArgsType>
	T* ConstructActor(ArgsType&&... Args)
	{
		auto Actor = ConstructObject<T>(std::forward<ArgsType>(Args)...);
		mActors.push_back(Actor);

#if _EDITOR
		Actor->InitializeParameters();

		Actor->Clicked.AddRaw(this, &PWorld::OnActorClicked);

		if (Actor->GetSelectable())
		{
			auto SelectionComponent = ConstructComponent<PSelectionComponent>(Actor);
			Actor->SetSelectionComponent(SelectionComponent);
		}

#endif
		return Actor;
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
			if (auto TypedActor = dynamic_cast<T*>(Actor))
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
		Owner->AddComponent(Component);

#if _EDITOR
		Component->InitializeParameters();
#endif
		Component->Start();
		mComponents.push_back(Component);
		return Component;
	}

	void DestroyComponent(PComponent* Component);

	std::vector<PComponent*> GetComponents() const;

	PWidget* GetRootWidget() const { return mRootWidget.get(); }

	template <IS_SUBCLASS_OF(PWidget), typename... ArgsType>
	T* ConstructWidget(ArgsType&&... Args)
	{
		auto Widget = ConstructObject<T>(std::forward<ArgsType>(Args)...);
		Widget->Start();
		mWidgets.push_back(Widget);
		return Widget;
	}

	template <typename T>
	void RegisterWidget(T* Widget)
	{
		mWidgets.push_back(ConstructObject<T>(Widget));
	}

	std::vector<PWidget*> GetWidgets() const;

	void DestroyWidget(PWidget* Widget);

	PPlayerCharacter*	 GetPlayerCharacter() const;
	void				 SetPlayerCharacter(PPlayerCharacter* PlayerCharacter);
	PActor*				 GetActorAtPosition(const FVector2& Position) const;
	std::vector<PActor*> GetActorsAtPosition(const FVector2& Position) const;

	PTimerManager* GetTimerManager()
	{
		return &mTimerManager;
	}

	PMapManager* GetMapManager()
	{
		return &mMapManager;
	}

	bool ProcessEvents(SInputEvent* Event) override;

#if _EDITOR
	TArray<PActor*> GetSelectableActors() const;
#endif
};

extern PWorld* GWorld;

template <typename T, typename... ArgsType>
T* ConstructActor(ArgsType&&... Args)
{
	return GWorld->ConstructActor<T>(std::forward<ArgsType>(Args)...);
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
	return GWorld->ConstructComponent<T>(Owner, std::forward<ArgsType>(Args)...);
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
	return GWorld->ConstructWidget<T>(std::forward<ArgsType>(Args)...);
}

template <typename T>
T* ConstructWidget(const JSON& Json)
{
	auto Widget = ConstructWidget<T>();
	Widget->Deserialize(Json);
	return Widget;
}

// Declared in Timer.h
PTimerManager* GetTimerManager() { return GWorld->GetTimerManager(); }
// Declared in MapManager.h
PMapManager* GetMapManager() { return GWorld->GetMapManager(); }