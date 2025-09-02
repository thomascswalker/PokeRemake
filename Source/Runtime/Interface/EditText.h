#pragma once

#include "Text.h"
#include "Widget.h"

class PEditText : public PWidget
{
protected:
    /** Text which is displayed in the widget. */
    PText* mText;
    /** Current position of the edit cursor. */
    int32_t mCursorPos = 0;

public:
    PEditText(): mText()
    {
        mResizeModeH = RM_Fixed;
        mFixedSize.Y = DEFAULT_WIDGET_HEIGHT;
        mResizeModeW = RM_Grow;

        mText = ConstructWidget<PText>();
        mText->SetAlignment(AL_Left);
        PWidget::AddChild(mText);
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
            float XOffset = Renderer->GetTextWidth(mText->GetText().substr(0, mCursorPos));
            CursorStart.X += XOffset + 5;
            FVector2 CursorEnd = {CursorStart.X, CursorStart.Y + G.H};
            CursorStart.Y += 3;
            CursorEnd.Y -= 3;
            Renderer->DrawLine(CursorStart, CursorEnd);
        }
    }

    void HandleClick(const FVector2& MousePos)
    {
        if (GetGeometry().Contains(MousePos))
        {
            mFocused = !mFocused;
            SetInputContext(IC_Text);
        }
        else if (mFocused)
        {
            mFocused = false;
            RestoreInputContext();
        }
    }

    void OnMouseEvent(SInputEvent* Event) override
    {
        switch (Event->Type)
        {
        case IET_MouseUp:
            {
                HandleClick(Event->MousePosition);
                Event->Consume();
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
            mText->Remove(mCursorPos);
        }
        else
        {
            mText->Add(Key, mCursorPos);
            mCursorPos++;
        }
        Event->Consume();
    }
};
