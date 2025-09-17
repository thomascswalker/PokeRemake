#pragma once

#include <string>

#define OFFSET_CAMERA

constexpr float EPSILON = 0.000000000001f;

constexpr int DEFAULT_SPRITE_WIDTH = 16;

constexpr float RENDER_SCALE = 1.0f;
constexpr float TILE_SIZE = 50.0f;
constexpr float BLOCK_SIZE = TILE_SIZE * 2.0f;
constexpr float TILE_ITEM_SIZE = 8.0f;
constexpr float BLOCK_ITEM_SIZE = TILE_ITEM_SIZE * 2.0f;
constexpr int	TILE_ROWS = 10;
constexpr int	TILE_COLUMNS = 9;

constexpr auto WINDOW_TITLE = "PokeRemake";
constexpr auto WINDOW_TITLE_EDITOR = "PokeRemake Editor";

#if _EDITOR
constexpr float WINDOW_DEFAULT_WIDTH = 1280;
constexpr float WINDOW_DEFAULT_HEIGHT = 720;
#else
constexpr float WINDOW_DEFAULT_WIDTH = 420;
constexpr float WINDOW_DEFAULT_HEIGHT = 360;
#endif

constexpr float GLOBAL_SPEED_MULTIPLIER = 5.0f;

// Visual character offset to add some 'depth'
constexpr float CHARACTER_OFFSET = TILE_SIZE / 2.0f;

// Units per second
constexpr int	TILES_PER_SECOND = 5;
constexpr float DEFAULT_CHAR_SPEED = TILE_SIZE * TILES_PER_SECOND * GLOBAL_SPEED_MULTIPLIER;

// How often a sprite animation will move to the next frame (in seconds)
// This is kind of arbitrary for now and is simply based on what looks best visually.
constexpr float DEFAULT_ANIM_SPEED = TILE_SIZE / DEFAULT_CHAR_SPEED;

constexpr float DEFAULT_TEXT_SPEED = 0.05f / GLOBAL_SPEED_MULTIPLIER;

constexpr int	FONT_ATLAS_SIZE = 1024;
constexpr int	FONT_CHAR_COUNT = 96;		   // ASCII 32..126
constexpr int	FONT_CHAR_START = 32;		   // ASCII 32 is the first printable character
constexpr float FONT_ATLAS_BAKE_SCALE = 64.0f; // Font size in pixels
constexpr float FONT_RENDER_SCALE = 16.0f;	   // Scale for rendering text
