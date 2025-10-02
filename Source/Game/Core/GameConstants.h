#pragma once

#include "Core/Constants.h"

// Ids

#define BATTLE_ID_GARY_OAK_LAB 1

// Data files

#define BATTLES_JSON "Battles.json"

// Strings

#define MAP_MODE	"MapMode"
#define BATTLE_MODE "BattleMode"

// Positions

#define COORD(P) (SCREEN_TILE * P)

/* DIALOG */

constexpr float DIALOG_Y = COORD(12);
constexpr float DIALOG_HEIGHT = COORD(6);
constexpr float DIALOG_WIDTH = COORD(GBA_X);
constexpr float DIALOG_FONT_SIZE = COORD(1.5); // Kind of arbitrary, looks good
constexpr float DIALOG_FONT_GAP = COORD(2);
constexpr float DIALOG_LINE_X = COORD(1);
constexpr float DIALOG_LINE1_Y = COORD(15);
constexpr float DIALOG_LINE2_Y = COORD(17);

/* BATTLES */

constexpr float BATTLE_MON_SIZE = COORD(12);

constexpr float PLAYER_ORIGIN_X = COORD(11);
constexpr float PLAYER_ORIGIN_Y = COORD(9);
constexpr float PLAYER_MON_X = COORD(-2);
constexpr float PLAYER_MON_Y = COORD(3);
constexpr float PLAYER_LEVEL_X = PLAYER_ORIGIN_X + COORD(5);
constexpr float PLAYER_LEVEL_Y = PLAYER_ORIGIN_Y + COORD(1);

constexpr float BATTLE_ORIGIN_X = COORD(1);
constexpr float BATTLE_ORIGIN_Y = COORD(1); // Drawn from bottom left
constexpr float BATTLE_MON_X = COORD(10);
constexpr float BATTLE_MON_Y = COORD(-2);
constexpr float BATTLE_LEVEL_X = BATTLE_ORIGIN_X + COORD(5);
constexpr float BATTLE_LEVEL_Y = BATTLE_ORIGIN_Y + COORD(1);
