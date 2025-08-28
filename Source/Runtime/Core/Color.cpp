#include "Color.h"

PColor PColor::Red      = PColor("FF0000");
PColor PColor::Green    = PColor("00FF00");
PColor PColor::Blue     = PColor("0000FF");
PColor PColor::White    = PColor("FFFFFF");
PColor PColor::Black    = PColor("000000");
PColor PColor::OffWhite = PColor("EEEEEE");

PColor PColor::UIPrimary          = PColor("575757");
PColor PColor::UIPrimaryHover     = PColor("8B8B8B");
PColor PColor::UIPrimaryClicked   = PColor("282828");
PColor PColor::UISecondary        = PColor("3399FF");
PColor PColor::UISecondaryHover   = PColor("99CCFF");
PColor PColor::UISecondaryClicked = PColor("0059B3");
PColor PColor::UIBackground       = PColor("121212");
PColor PColor::UIText             = PColor("FFFFFF");
PColor PColor::UIBorder           = PColor("717171");
PColor PColor::UIPanel            = PColor("3F3F3F");
PColor PColor::UIDebug1           = PColor("FF88DD");
PColor PColor::UIDebug2           = PColor("DDFF88");
PColor PColor::UIDebug3           = PColor("88DDFF");

PColor PColor::Selection       = PColor("FCBA03").WithAlpha(128);
PColor PColor::SelectionOpaque = PColor("FCBA03");
