#pragma once

#include "Engine/Object.h"
#include "Renderer/Renderer.h"
#include <algorithm>

#define WIDGET_TEXT 255, 255, 255, 255
#define WIDGET_LIGHT 150, 150, 150, 255
#define WIDGET_MED 100, 100, 100, 255
#define WIDGET_DARK 50, 50, 50, 255

#define WIDGET_WIDTH 50
#define WIDGET_HEIGHT 20
#define WIDGET_SPACING 5
#define WIDGET_FONT_SIZE 16.0f

enum ELayoutMode
{
	LM_Horizontal,
	LM_Vertical,
};

enum EResizeMode
{
	RM_FixedXY,
	RM_ExpandX,
	RM_ExpandY,
	RM_ExpandXY,
};

struct SWidgetEvent
{
	bool	 bConsumed = false; // Event was consumed by a widget
	bool	 bMouseDown;
	FVector2 MousePosition;
};

class PWidget : public PObject
{
protected:
	// Static pointer to the widget that sent the event.
	static PWidget* mSender;
	// Pointer to the parent of this widget.
	PWidget* mParent = nullptr;
	// List of child widgets.
	std::vector<PWidget*> mChildren;

	ELayoutMode mLayoutMode = LM_Vertical; // Default layout mode is vertical
	EResizeMode mResizeMode = RM_ExpandXY;

public:
	float X = 0.0f;
	float Y = 0.0f;
	float W = 0.0f;
	float H = 0.0f;

	PWidget() = default;
	// ReSharper disable once CppEnforceOverridingDestructorStyle
	virtual ~PWidget() override = default;

	virtual void Draw(const PRenderer* Renderer) const
	{
		for (const auto& Child : mChildren)
		{
			Child->Draw(Renderer);
		}
	}
	void		  Tick(float DeltaTime) override {}
	virtual FRect GetGeometry() const { return FRect{ X, Y, W, H }; }
	virtual void  ProcessEvents(SWidgetEvent* Event);

	PWidget* GetParent() const { return mParent; }
	void	 SetParent(PWidget* Parent);

	// Children

	virtual void				  AddChild(PWidget* Child);
	virtual void				  RemoveChild(PWidget* Child);
	virtual std::vector<PWidget*> GetChildren() const { return mChildren; }

	// Layout

	virtual void LayoutChildren();
	ELayoutMode	 GetLayoutMode() const { return mLayoutMode; }
	void		 SetLayoutMode(ELayoutMode LayoutMode) { mLayoutMode = LayoutMode; }

	EResizeMode GetResizeMode() const { return mResizeMode; }
	void		SetResizeMode(EResizeMode resizeMode) { mResizeMode = resizeMode; }

	// Returns a pointer to the widget that sent the event.
	static PWidget* GetSender() { return mSender; }
};
