#pragma once

#include <string>

#define OFFSET_CAMERA

constexpr float EPSILON = 0.000000000001f;

constexpr int SPRITE_WIDTH = 16;

constexpr float RENDER_SCALE = 1.0f;
constexpr float TILE_SIZE = 50.0f;
constexpr float BLOCK_SIZE = TILE_SIZE * 2.0f;
constexpr int	TILE_ROWS = 10;
constexpr int	TILE_COLUMNS = 9;

constexpr auto	WINDOW_TITLE = "PokeRemake";
constexpr auto	WINDOW_TITLE_EDITOR = "PokeRemake Editor";
constexpr float WINDOW_DEFAULT_WIDTH = BLOCK_SIZE * TILE_ROWS;
constexpr float WINDOW_DEFAULT_HEIGHT = BLOCK_SIZE * TILE_COLUMNS;

constexpr float GLOBAL_SPEED_MULTIPLIER = 1.0f;

// Visual character offset to add some 'depth'
constexpr float CHARACTER_OFFSET = TILE_SIZE / 2.0f;

// Tiles per second
constexpr float DEFAULT_CHAR_SPEED = GLOBAL_SPEED_MULTIPLIER / 2.0f;

// How often a sprite animation will move to the next frame (in milliseconds)
// This is kind of arbitrary for now and is simply based on what looks best visually.
constexpr float DEFAULT_ANIM_SPEED = TILE_SIZE / GLOBAL_SPEED_MULTIPLIER * 2.0f;

const std::string FONT_NAME = "Pokemon";
constexpr int	  FONT_ATLAS_SIZE = 1024;
constexpr int	  FONT_CHAR_COUNT = 96;			 // ASCII 32..126
constexpr int	  FONT_CHAR_START = 32;			 // ASCII 32 is the first printable character
constexpr float	  FONT_ATLAS_BAKE_SCALE = 64.0f; // Font size in pixels
constexpr float	  FONT_RENDER_SCALE = 16.0f;	 // Scale for rendering text
