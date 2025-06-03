#pragma once

constexpr float EPSILON = 0.000000000001f;

constexpr int SPRITE_WIDTH = 16;

constexpr float TILE_SIZE = 100.0f;
constexpr float HALF_TILE_SIZE = TILE_SIZE / 2.0f;
constexpr float PLAYER_SPEED = 0.5f;

constexpr int TILE_ROWS = 10;
constexpr int TILE_COLUMNS = 9;

constexpr float WINDOW_DEFAULT_WIDTH = HALF_TILE_SIZE * TILE_ROWS;
constexpr float WINDOW_DEFAULT_HEIGHT = HALF_TILE_SIZE * TILE_COLUMNS;
constexpr auto	WINDOW_TITLE = "PokeRemake";
