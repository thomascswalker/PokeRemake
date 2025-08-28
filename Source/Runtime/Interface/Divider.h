#pragma once

#include "Widget.h"

template <EDirection Direction>
class PDivider : public PWidget
{
public:
    PDivider()
    {
        mPadding = {0};
        switch (Direction)
        {
        case DR_Horizontal:
            {
                mResizeModeW = RM_Grow;
                mResizeModeH = RM_Fixed;
                mFixedSize.Y = 5;
                break;
            }
        case DR_Vertical:
            {
                mResizeModeW = RM_Fixed;
                mResizeModeH = RM_Grow;
                mFixedSize.X = 5;
                break;
            }
        }
    }

    void Draw(const PRenderer* Renderer) const override
    {
        auto Geo = GetGeometry();
        FVector2 LineStart, LineEnd;

        switch (Direction)
        {
        case DR_Horizontal:
            {
                LineStart.X = Geo.X;
                LineStart.Y = Geo.Y + (Geo.H / 2);
                LineEnd.X   = Geo.X + Geo.W;
                LineEnd.Y   = LineStart.Y;
                break;
            }
        case DR_Vertical:
            {
                LineStart.X = Geo.X + (Geo.W / 2);
                LineStart.Y = Geo.Y;
                LineEnd.X   = LineStart.X;
                LineEnd.Y   = Geo.Y + Geo.H;
                break;
            }
        }

        Renderer->SetDrawColor(PColor::UIBorder);
        Renderer->DrawLine(LineStart, LineEnd);
    }
};

using PHDivider = PDivider<DR_Horizontal>;
using PVDivider = PDivider<DR_Vertical>;
