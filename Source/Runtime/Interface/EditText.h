#pragma once

#include "Text.h"
#include "Widget.h"

DECLARE_MULTICAST_DELEGATE(DOnTextChange, std::string);

class PEditText : public PText
{
	EInputContext mPrevInputContext;

protected:
	/** Current position of the edit cursor. */
	int32_t mCursorPos = 0;

public:
	DOnTextChange OnTextChange;

	PEditText()
	{
		mResizeModeH = RM_Fixed;
		mFixedSize.Y = DEFAULT_WIDGET_HEIGHT;
		mResizeModeW = RM_Grow;

		SetAlignment(AL_Left);
	}

	~PEditText() override = default;

	void Draw(const PRenderer* Renderer) const override
	{
		auto G = GetGeometry();

		// Background
		Renderer->SetDrawColor(PColor::UIBackground);
		Renderer->DrawFillRect(G);

		// Outline
		Renderer->SetDrawColor(mFocused ? PColor::UISecondary : PColor::UIBorder);
		Renderer->DrawRect(G);

		// Cursor
		if (mFocused)
		{
			Renderer->SetDrawColor(PColor::UIText);
			FVector2 CursorStart = G.Min();
			// X offset for char position
			float XOffset = Renderer->GetTextWidth(mText.substr(0, mCursorPos));
			CursorStart.X += XOffset + 2;
			FVector2 CursorEnd = { CursorStart.X, CursorStart.Y + G.H };
			CursorStart.Y += 3;
			CursorEnd.Y -= 3;
			Renderer->DrawLine(CursorStart, CursorEnd);
		}

		// Text
		PText::Draw(Renderer);
	}

	void HandleClick(SInputEvent* Event)
	{
		if (GetGeometry().Contains(Event->MousePosition))
		{
			mCursorPos = mText.size();
			mFocused = !mFocused;
			mPrevInputContext = GetInputContext()->Type;
			SetInputContext(IC_Text);
			Event->Consume();
		}
		else if (mFocused)
		{
			mFocused = false;
			SetInputContext(mPrevInputContext);
		}
	}

	void OnMouseEvent(SInputEvent* Event) override
	{
		switch (Event->Type)
		{
			case IET_MouseUp:
				{
					HandleClick(Event);
					break;
				}
			default:
				{
					break;
				}
		}
	}

	void OnKeyDown(SInputEvent* Event) override
	{
		auto Key = Event->KeyDown;
		if (!mFocused && isascii(Key))
		{
			return;
		}
		if (Key == SDLK_DELETE || Key == SDLK_BACKSPACE)
		{
			mCursorPos = std::max(0, mCursorPos - 1);
			Remove(mCursorPos);
		}
		else
		{
			Add(Key, mCursorPos);
			mCursorPos++;
		}
		OnTextChange.Broadcast(mText);
		Event->Consume();
	}

#if _EDITOR
	void Bind(PParameter* Param) override
	{
		// Set the parameter reference
		PWidget::Bind(Param);

		// Override the existing text with the parameter's current text
		mText = Param->Get<std::string>();

		// Bind events from this widget to set the parameter value
		OnTextChange.AddRaw(Param, &PParameter::Set<std::string>);
	}
#endif
};
