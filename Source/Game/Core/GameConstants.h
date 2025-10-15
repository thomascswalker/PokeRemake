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

#define BATTLE_ACTION_FIGHT u"FIGHT"
#define BATTLE_ACTION_PARTY u"PKMN"
#define BATTLE_ACTION_ITEM	u"ITEM"
#define BATTLE_ACTION_RUN	u"RUN"

// Textures

#define TEX_DIALOG "DialogBox"
#define TEX_FONT   "Font"

#define TEX_ASH	 "Ash"
#define TEX_GARY "Gary"

// Positions

#define COORD(P) (SCREEN_TILE * (P))

/* Text */

constexpr auto CHAR_PK = u"Ƥ";					 // Ƥ, PK
constexpr auto CHAR_MN = u"Ɯ";					 // Ɯ, MN
constexpr auto CHAR_RIGHT_ARROW = u"▷";			 // ▷, 0x25B7
constexpr auto CHAR_RIGHT_ARROW_SELECTED = u"▶"; // ▶, 0x25B6
constexpr auto CHAR_DOWN_ARROW = u"▼";			 // ▼, 0x25BC
constexpr auto CHAR_SPECIAL_L = u"Ł";			 // Ł, 0x0141

constexpr int FONT_ROW_COUNT = 8;
constexpr int FONT_COL_COUNT = 16;
constexpr int GLYPH_COUNT = FONT_ROW_COUNT * FONT_COL_COUNT;
constexpr int GLYPH_WIDTH = COORD(1);
constexpr int GLYPH_HEIGHT = COORD(1);

/* Dialog */

constexpr float DIALOG_BOX_X = 0;
constexpr float DIALOG_BOX_Y = 12;
constexpr float DIALOG_BOX_H = 6;
constexpr float DIALOG_BOX_W = GBA_W;
constexpr float DIALOG_FONT_SIZE = COORD(1.5); // Kind of arbitrary, looks good
constexpr float DIALOG_FONT_GAP = COORD(2);
constexpr float DIALOG_LINE_X = COORD(1);
constexpr float DIALOG_LINE1_Y = COORD(14);
constexpr float DIALOG_LINE2_Y = COORD(16);

#define UI_INDEX_HP_LABEL_1				   0
#define UI_INDEX_HP_LABEL_2				   1
#define UI_INDEX_HP_EMPTY				   2  // | -------- |
#define UI_INDEX_HP_1					   3  // | +------- |
#define UI_INDEX_HP_2					   4  // | ++------ |
#define UI_INDEX_HP_3					   5  // | +++----- |
#define UI_INDEX_HP_4					   6  // | ++++---- |
#define UI_INDEX_HP_5					   7  // | +++++--- |
#define UI_INDEX_HP_6					   8  // | ++++++-- |
#define UI_INDEX_HP_7					   9  // | +++++++- |
#define UI_INDEX_HP_FULL				   10 // | ++++++++ |
#define UI_INDEX_HP_END					   11 // |>
#define UI_INDEX_STATUS_FRAME_CORNER_RIGHT 12 // ┘
#define UI_INDEX_ID						   14
#define UI_INDEX_NO						   15
#define UI_INDEX_LEVEL					   16
#define UI_INDEX_TO						   17
#define UI_INDEX_STATUS_FRAME_VERTICAL	   18 // │
#define UI_INDEX_STATUS_FRAME_ARROW_LEFT   19 // <|
#define UI_INDEX_STATUS_FRAME_HORIZONTAL   20 // ─
#define UI_INDEX_STATUS_FRAME_TOP		   21 // ─
#define UI_INDEX_STATUS_FRAME_ARROW_RIGHT  22 // |>
#define UI_INDEX_STATUS_FRAME_CORNER_LEFT  23 // └
#define UI_INDEX_FRAME_TOPLEFT			   24
#define UI_INDEX_FRAME_BOTTOMLEFT		   25
#define UI_INDEX_FRAME_TOPRIGHT			   26
#define UI_INDEX_FRAME_BOTTOMRIGHT		   27
#define UI_INDEX_FRAME_HORIZONTAL		   28
#define UI_INDEX_FRAME_VERTICAL			   29

/* Battles */

constexpr uint8_t MAX_BATTLE_MOVES = 4;