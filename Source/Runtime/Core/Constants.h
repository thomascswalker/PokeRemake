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

// Tiles per second
constexpr float DEFAULT_CHAR_SPEED = 0.5f * RENDER_SCALE;
// Corresponds to distance per tile per second
constexpr float DEFAULT_ANIM_SPEED = TILE_SIZE / DEFAULT_CHAR_SPEED;