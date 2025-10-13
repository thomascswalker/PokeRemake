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
