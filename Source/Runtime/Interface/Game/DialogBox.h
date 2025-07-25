#pragma once
#include "Interface/Box.h"
#include "Interface/Text.h"

class PDialogBox : public PBox
{
    PText mText;

public:
    PDialogBox(const std::string& Text = "") : mText(Text)
    {
        PWidget::AddChild(&mText);
        mText.SetFontSize(32.0f);
    }

    void SetText(const std::string& Text)
    {
        mText.SetText(Text);
    }
};
