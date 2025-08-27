#pragma once

#include "Widget.h"
#include "Engine/Input.h"

template <class T>
class IPopupInstigator
{
protected:
    T* mView = nullptr;

public:
    virtual ~IPopupInstigator() = default;
    virtual void ShowView(bool State) = 0;
    virtual void HideView() = 0;
};

class IPopup : public PWidget
{
public:
    IPopup() : PWidget()
    {
        mFloating = true;
    }

    ~IPopup() override = default;

    void OnMouseEvent(SInputEvent* Event) override
    {
        bool OldMouseOver = mMouseOver;
        mMouseOver        = GetGeometry().Contains(Event->MousePosition);
        bool ParentOver   = mParent->GetGeometry().Contains(Event->MousePosition);
        if (OldMouseOver && !mMouseOver && !ParentOver)
        {
            HoverEnd.Broadcast();
        }
        Event->Consume();
    }
};
