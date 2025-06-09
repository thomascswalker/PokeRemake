#pragma once

#define OFFSET_CAMERA

constexpr float EPSILON = 0.000000000001f;

constexpr int SPRITE_WIDTH = 16;

constexpr float RENDER_SCALE = 1.0f;
constexpr float TILE_SIZE = 100.0f * RENDER_SCALE;
constexpr float HALF_TILE_SIZE = TILE_SIZE / 2.0f;
constexpr float QUARTER_TILE_SIZE = TILE_SIZE / 4.0f;
constexpr int	TILE_ROWS = 10;
constexpr int	TILE_COLUMNS = 9;

constexpr auto	WINDOW_TITLE = "PokeRemake";
constexpr float WINDOW_DEFAULT_WIDTH = HALF_TILE_SIZE * TILE_ROWS;
constexpr float WINDOW_DEFAULT_HEIGHT = HALF_TILE_SIZE * TILE_COLUMNS;

constexpr float GLOBAL_SPEED_MULTIPLIER = 2.0f;

// Visual character offset to add some 'depth'
constexpr float CHARACTER_OFFSET = TILE_SIZE / 4.0f;

// Tiles per second
constexpr float DEFAULT_CHAR_SPEED = RENDER_SCALE / 4.0f * GLOBAL_SPEED_MULTIPLIER;

// How often a sprite animation will move to the next frame (in milliseconds)
// This is kind of arbitrary for now and is simply based on what looks best visually.
constexpr float DEFAULT_ANIM_SPEED = TILE_SIZE * 1.50f / GLOBAL_SPEED_MULTIPLIER;