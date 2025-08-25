#pragma once

#include <cstdint>

#include "Core/Bitmask.h"
#include "Interface/GridView.h"

#define NEW_GRID_SIZE 5

enum EEditMode
{
    EM_None,
    EM_Select,
    EM_Tile
};

enum EBrushSize
{
    BS_Small, // 1X1
    BS_Large, // 2X2
};

enum EBrushMode
{
    BM_Default, // Paint from the source tile into all four quadrants.
    BM_Copy,    // Paint from the source tile the three adjacent tiles, where the source tile is the top left.
    BM_Fill     // Fills the entire map with the tile
};

DECLARE_MULTICAST_DELEGATE(DEditModeChanged, EEditMode);
