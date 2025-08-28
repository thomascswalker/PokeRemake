#pragma once

#include "Core/Color.h"
#include "Core/CSS.h"

struct SStyle
{
    PColor Primary          = PColor::UIPrimary;
    PColor PrimaryHover     = PColor::UIPrimaryHover;
    PColor PrimaryClicked   = PColor::UIPrimaryClicked;
    PColor Secondary        = PColor::UISecondary;
    PColor SecondaryHover   = PColor::UISecondaryHover;
    PColor SecondaryClicked = PColor::UISecondaryClicked;
    PColor Background       = PColor::UIBackground;
    PColor Text             = PColor::UIText;
    PColor Border           = PColor::UIBorder;
};

class StyleManager
{
public:
    static void LoadAllStylesheets();
    static CSS::Stylesheet* LoadStylesheet(const std::string& FileName);
    static CSS::Stylesheet* GetStylesheet(const std::string& Name);
};
