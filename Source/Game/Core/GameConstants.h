#pragma once

#include <cstdint>

#include "Core/Constants.h"

// Ids

#define BATTLE_ID_GARY_OAK_LAB 1

// Data files

#define BATTLES_JSON "Battles.json"

// Strings

#define MAP_MODE	"MapMode"
#define BATTLE_MODE "BattleMode"

#define BATTLE_ACTION_FIGHT "FIGHT"
#define BATTLE_ACTION_PARTY "PKMN"
#define BATTLE_ACTION_ITEM	"ITEM"
#define BATTLE_ACTION_RUN	"RUN"

// Textures

#define TEX_DIALOG "DialogBox"
#define TEX_FONT   "Font"

#define TEX_ASH	 "Ash"
#define TEX_GARY "Gary"

// Positions

#define COORD(P) (SCREEN_TILE * P)

/* Text */

#define CHAR_PK				 128
#define CHAR_MN				 129
#define RIGHT_ARROW			 130
#define RIGHT_ARROW_SELECTED 131
#define DOWN_ARROW			 132

constexpr int FONT_ROW_COUNT = 8;
constexpr int FONT_COL_COUNT = 16;
constexpr int GLYPH_COUNT = FONT_ROW_COUNT * FONT_COL_COUNT;
constexpr int GLYPH_WIDTH = COORD(1);
constexpr int GLYPH_HEIGHT = COORD(1);

/* Dialog */

constexpr float DIALOG_X = 0;
constexpr float DIALOG_Y = 12;
constexpr float DIALOG_H = 6;
constexpr float DIALOG_W = GBA_W;
constexpr float DIALOG_FONT_SIZE = COORD(1.5); // Kind of arbitrary, looks good
constexpr float DIALOG_FONT_GAP = COORD(2);
constexpr float DIALOG_LINE_X = COORD(1);
constexpr float DIALOG_LINE1_Y = COORD(14);
constexpr float DIALOG_LINE2_Y = COORD(16);

/* Battles */

constexpr float BATTLE_MON_SIZE = COORD(12);

constexpr float PLAYER_ORIGIN_X = COORD(11);
constexpr float PLAYER_ORIGIN_Y = COORD(8);
constexpr float PLAYER_MON_X = COORD(-2);
constexpr float PLAYER_MON_Y = COORD(2);
constexpr float PLAYER_LEVEL_X = PLAYER_ORIGIN_X + COORD(5);
constexpr float PLAYER_LEVEL_Y = PLAYER_ORIGIN_Y + COORD(1);

constexpr float BATTLE_ORIGIN_X = COORD(1);
constexpr float BATTLE_ORIGIN_Y = COORD(0);
constexpr float BATTLE_MON_X = COORD(10);
constexpr float BATTLE_MON_Y = COORD(-1);
constexpr float BATTLE_LEVEL_X = BATTLE_ORIGIN_X + COORD(5);
constexpr float BATTLE_LEVEL_Y = BATTLE_ORIGIN_Y + COORD(1);

constexpr float BATTLE_ACTION_FIGHT_X = COORD(10);
constexpr float BATTLE_ACTION_FIGHT_Y = DIALOG_LINE1_Y;
constexpr float BATTLE_ACTION_PARTY_X = COORD(16);
constexpr float BATTLE_ACTION_PARTY_Y = DIALOG_LINE1_Y;
constexpr float BATTLE_ACTION_ITEM_X = BATTLE_ACTION_FIGHT_X;
constexpr float BATTLE_ACTION_ITEM_Y = DIALOG_LINE2_Y;
constexpr float BATTLE_ACTION_RUN_X = BATTLE_ACTION_PARTY_X;
constexpr float BATTLE_ACTION_RUN_Y = DIALOG_LINE2_Y;

constexpr int32_t MAX_BATTLE_MOVES = 4;