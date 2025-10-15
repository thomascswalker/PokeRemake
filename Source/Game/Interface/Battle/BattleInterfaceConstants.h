#pragma once

#include "../Game/Core/GameConstants.h"

constexpr float BATTLE_ACTION_MENU_X = 8;
constexpr float BATTLE_ACTION_MENU_Y = DIALOG_BOX_Y;
constexpr float BATTLE_ACTION_MENU_W = DIALOG_BOX_W - BATTLE_ACTION_MENU_X;
constexpr float BATTLE_ACTION_MENU_H = DIALOG_BOX_H;

constexpr float BATTLE_ACTION_FIGHT_X = COORD(10);
constexpr float BATTLE_ACTION_FIGHT_Y = DIALOG_LINE1_Y;
constexpr float BATTLE_ACTION_PARTY_X = COORD(16);
constexpr float BATTLE_ACTION_PARTY_Y = DIALOG_LINE1_Y;
constexpr float BATTLE_ACTION_ITEM_X = BATTLE_ACTION_FIGHT_X;
constexpr float BATTLE_ACTION_ITEM_Y = DIALOG_LINE2_Y;
constexpr float BATTLE_ACTION_RUN_X = BATTLE_ACTION_PARTY_X;
constexpr float BATTLE_ACTION_RUN_Y = DIALOG_LINE2_Y;

constexpr auto	   CHAR_LEVEL = u":Ł"; // Ł
constexpr float	   BATTLE_MON_SIZE = COORD(12);
static const FRect PLAYER_MON_RECT = { COORD(-1), COORD(3), BATTLE_MON_SIZE, BATTLE_MON_SIZE };
static const FRect BATTLE_MON_RECT = { COORD(10), COORD(-3), BATTLE_MON_SIZE, BATTLE_MON_SIZE };

constexpr float PLAYER_ORIGIN_X = COORD(10);
constexpr float PLAYER_ORIGIN_Y = COORD(7);
constexpr float BATTLE_ORIGIN_X = COORD(1);
constexpr float BATTLE_ORIGIN_Y = COORD(0);

constexpr float PLAYER_LEVEL_X = COORD(14);
constexpr float PLAYER_LEVEL_Y = COORD(8);

constexpr float PLAYER_HP_X = COORD(11);
constexpr float PLAYER_HP_Y = COORD(10);

constexpr float BATTLE_LEVEL_X = COORD(5);
constexpr float BATTLE_LEVEL_Y = COORD(1);

// Maximum number of characters in the display HP text. (NNN/NNN)
constexpr int32_t MAX_HP_TEXT_LENGTH = 7;
