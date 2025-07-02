#pragma once

#include "Engine/Object.h"
#include "Renderer/Renderer.h"
#include <initializer_list>
#include <limits>

#define WIDGET_TEXT 255, 255, 255, 255
#define WIDGET_LIGHT 150, 150, 150, 255
#define WIDGET_MED 100, 100, 100, 255
#define WIDGET_DARK 50, 50, 50, 255

#define WIDGET_WIDTH 50
#define WIDGET_HEIGHT 20
#define WIDGET_SPACING 5
#define WIDGET_FONT_SIZE 16.0f

#define DEFAULT_BUTTON_HEIGHT 20

enum ELayoutMode
{
	LM_Horizontal,
	LM_Vertical,
};

enum EResizeMode
{
	RM_Fixed,
	RM_Fit,
	RM_Grow,
};

struct SWidgetEvent
{
	bool	 Consumed = false; // Event was consumed by a widget
	bool	 MouseDown;
	FVector2 MousePosition;
};

template <typename T>
struct TPadding
{
	T Left;
	T Bottom;
	T Right;
	T Top;

	explicit TPadding(T Value)
		: Left(Value), Bottom(Value), Right(Value), Top(Value) {}
	explicit TPadding(T InLeft, T InBottom, T InRight, T InTop)
		: Left(InLeft), Bottom(InBottom), Right(InRight), Top(InTop) {}
	TPadding(std::initializer_list<T> Values)
	{
		T* P = &Left;
		for (auto it = Values.begin(); it != Values.end(); ++it)
		{
			*P++ = *it;
		}
	}
};
using FPadding = TPadding<float>;

class PWidget : public PObject
{
protected:
	// Static pointer to the widget that sent the event.
	static PWidget* mSender;
	// Pointer to the parent of this widget.
	PWidget* mParent = nullptr;
	// List of child widgets.
	std::vector<PWidget*> mChildren;

	ELayoutMode mLayoutMode = LM_Horizontal;

	EResizeMode mResizeModeW = RM_Grow;
	EResizeMode mResizeModeH = RM_Grow;

	FVector2 mFixedSize = { 0.0f, 0.0f };
	FVector2 mMaxSize = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };

public:
	float X = 0.0f;
	float Y = 0.0f;
	float W = 0.0f;
	float H = 0.0f;

	FPadding Padding;

	PWidget()
		: Padding(5.0f) {}
	// ReSharper disable once CppEnforceOverridingDestructorStyle
	virtual ~PWidget() override = default;

	void DrawChildren(const PRenderer* Renderer) const
	{
		for (const auto& Child : mChildren)
		{
			// Draw this widget
			Child->Draw(Renderer);
			// Draw its children
			Child->DrawChildren(Renderer);
		}
	}
	virtual void  Draw(const PRenderer* Renderer) const {}
	void		  Tick(float DeltaTime) override {}
	virtual FRect GetGeometry() const { return FRect{ X, Y, std::min(W, mMaxSize.X), std::min(H, mMaxSize.Y) }; }
	virtual void  ProcessEvents(SWidgetEvent* Event);

	PWidget* GetParent() const { return mParent; }
	void	 SetParent(PWidget* Parent);

	// Children

	virtual void		  AddChild(PWidget* Child);
	virtual void		  RemoveChild(PWidget* Child);
	std::vector<PWidget*> GetChildren() const { return mChildren; }
	size_t				  GetChildCount() const { return mChildren.size(); }

	// Layout

	ELayoutMode GetLayoutMode() const { return mLayoutMode; }
	void		SetLayoutMode(ELayoutMode LayoutMode) { mLayoutMode = LayoutMode; }

	EResizeMode GetResizeModeW() const { return mResizeModeW; }
	EResizeMode GetResizeModeH() const { return mResizeModeH; }
	void		SetResizeModeW(EResizeMode resizeMode) { mResizeModeW = resizeMode; }
	void		SetResizeModeH(EResizeMode resizeMode) { mResizeModeH = resizeMode; }
	void		SetResizeMode(EResizeMode InW, EResizeMode InH)
	{
		mResizeModeW = InW;
		mResizeModeH = InH;
	}

	FVector2 GetFixedSize() const { return mFixedSize; }
	void	 SetFixedSize(const FVector2& Size) { mFixedSize = Size; }
	void	 SetFixedWidth(float W) { mFixedSize.X = W; }
	void	 SetFixedHeight(float H) { mFixedSize.Y = H; }

	FVector2 GetMaxSize() const { return mMaxSize; }
	void	 SetMaxSize(const FVector2& MaxSize) { mMaxSize = MaxSize; }
	void	 SetMaxWidth(float W) { mMaxSize.X = W; }
	void	 SetMaxHeight(float H) { mMaxSize.Y = H; }

	// Returns a pointer to the widget that sent the event.
	static PWidget* GetSender() { return mSender; }
};
