#pragma once

#include <initializer_list>
#include <limits>

#include "Core/Containers.h"
#include "Engine/Input.h"
#include "Engine/Object.h"
#include "Renderer/Renderer.h"

#define WIDGET_TEXT 255, 255, 255, 255
#define WIDGET_LIGHT 150, 150, 150, 255
#define WIDGET_MED 100, 100, 100, 255
#define WIDGET_DARK 50, 50, 50, 255

#define WIDGET_WIDTH 50
#define WIDGET_HEIGHT 20
#define WIDGET_SPACING 5
#define WIDGET_FONT_SIZE 16.0f

#define DEFAULT_WIDGET_HEIGHT 20

enum ELayoutMode
{
	LM_Horizontal,
	LM_Vertical,
	LM_Grid,
};

enum EResizeMode
{
	RM_Fixed,
	RM_Fit,
	RM_Grow,
};

enum EZDepth
{
	ZD_Floating,
	ZD_Default
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

class PWidget : public PObject, public IInputHandler
{
protected:
	// Static pointer to the widget that sent the event.
	static PWidget* mSender;
	// Pointer to the parent of this widget.
	PWidget* mParent = nullptr;
	// List of child widgets.
	std::vector<PWidget*> mChildren;
	// Custom data associated with this widget
	void* mCustomData = nullptr;

	ELayoutMode mLayoutMode = LM_Horizontal;

	EResizeMode mResizeModeW = RM_Grow;
	EResizeMode mResizeModeH = RM_Grow;

	FVector2 mFixedSize = { 0.0f, 0.0f };
	FVector2 mOffset = { 0.0f, 0.0f };
	FVector2 mMaxSize = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
	int		 mGridCount = 1;

	bool mMouseOver = false;
	bool mVisible = true;

	// Floating widgets

	bool	mFloating = false;
	EZDepth mDepth = ZD_Default;

public:
	float X = 0.0f;
	float Y = 0.0f;
	float W = 0.0f;
	float H = 0.0f;

	FPadding	Padding;
	DHoverBegin HoverBegin;
	DHoverEnd	HoverEnd;

	PWidget()
		: Padding(5.0f) { GenerateInternalName(); }
	// ReSharper disable once CppEnforceOverridingDestructorStyle
	virtual ~PWidget() override = default;

	// General

	void		 Tick(float DeltaTime) override {}
	bool		 ProcessEvents(SInputEvent* Event) override;
	virtual void OnLayout()
	{
		for (auto Child : mChildren)
		{
			Child->OnLayout();
		}
	}

	PWidget* GetParent() const { return mParent; }
	void	 SetParent(PWidget* Parent);

	bool GetVisible() const { return mVisible; }
	void SetVisible(bool State) { mVisible = State; }
	void ToggleVisible() { SetVisible(!mVisible); }

	bool GetFloating() const { return mFloating; }
	void SetFloating(bool State) { mFloating = State; }

	// Drawing

	virtual bool DrawChildren(const PRenderer* Renderer) const
	{
		if (!mVisible)
		{
			return true;
		}
		for (const auto& Child : mChildren)
		{
			if (!Child->GetVisible())
			{
				return true;
			}
			// Defer drawing floating children until after the main drawing loop
			if (Child->GetFloating())
			{
				continue;
			}
			// Draw this widget
			Child->PreDraw(Renderer);
			Child->Draw(Renderer);
			Child->PostDraw(Renderer);

			// Draw its children
			Child->DrawChildren(Renderer);
		}
		return true;
	}
	virtual void PreDraw(const PRenderer* Renderer) {}
	virtual void Draw(const PRenderer* Renderer) const {}
	virtual void PostDraw(const PRenderer* Renderer) {}

	// Children

	virtual void		  AddChild(PWidget* Child);
	virtual void		  RemoveChild(PWidget* Child);
	std::vector<PWidget*> GetChildren() const
	{
		return mChildren;
	}
	size_t GetChildCount() const { return mChildren.size(); }

	// Layout

	virtual FRect GetGeometry() const
	{
		return FRect{ X, Y, std::min(W, mMaxSize.X), std::min(H, mMaxSize.Y) };
	}

	FVector2 GetOffset() const { return mOffset; }
	void	 SetOffset(const FVector2& Offset, bool Propagate = true)
	{
		mOffset = Offset;
		if (!Propagate)
		{
			return;
		}
		for (auto Child : mChildren)
		{
			Child->SetOffset(Offset, Propagate);
		}
	}
	void SetOffsetX(float Value, bool Propagate = false)
	{
		mOffset.X = Value;
		if (!Propagate)
		{
			return;
		}
		for (auto Child : mChildren)
		{
			Child->SetOffsetX(Value, Propagate);
		}
	}
	void SetOffsetY(float Value, bool Propagate = false)
	{
		mOffset.Y = Value;
		if (!Propagate)
		{
			return;
		}
		for (auto Child : mChildren)
		{
			Child->SetOffsetY(Value, Propagate);
		}
	}

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
	void	 SetFixedSize(float Width, float Height) { mFixedSize = FVector2(Width, Height); }
	void	 SetFixedSize(float Value) { mFixedSize = FVector2(Value, Value); }
	void	 SetFixedWidth(float W) { mFixedSize.X = W; }
	void	 SetFixedHeight(float H) { mFixedSize.Y = H; }

	int	 GetGridCount() const { return mGridCount; }
	void SetGridCount(int Size) { mGridCount = Size; }

	FVector2 GetMaxSize() const { return mMaxSize; }
	void	 SetMaxSize(const FVector2& MaxSize) { mMaxSize = MaxSize; }
	void	 SetMaxWidth(float W) { mMaxSize.X = W; }
	void	 SetMaxHeight(float H) { mMaxSize.Y = H; }

	// Custom data
	template <typename T>
	T* GetCustomData()
	{
		return static_cast<T*>(mCustomData);
	}

	template <typename T>
	void SetCustomData(T* Data)
	{
		auto Size = sizeof(T);
		mCustomData = std::malloc(Size);
		std::memcpy(mCustomData, Data, Size);
	}

	// Returns a pointer to the widget that sent the event.
	template <typename T = PWidget>
	static T* GetSender()
	{
		return static_cast<T*>(mSender);
	}
};
