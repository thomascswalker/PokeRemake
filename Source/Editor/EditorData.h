#pragma once

#include <cstdint>

#include "Core/Bitmask.h"
#include "Interface/GridView.h"

#define NEW_GRID_SIZE 5

enum EInputContext : uint8_t
{
    IC_None   = 1U << 0,
    IC_Select = 1U << 2,
    IC_Tile   = 1U << 3,
    IC_Actor  = 1U << 4,
};

DEFINE_BITMASK_OPERATORS(EInputContext);

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

struct SInputModeData
{
    EInputContext InputContext;
    PGroup* Group;
};

DECLARE_MULTICAST_DELEGATE(DEditModeChanged, EEditMode);
