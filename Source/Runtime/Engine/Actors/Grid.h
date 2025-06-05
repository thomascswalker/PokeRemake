#pragma once

#include "Actor.h"

struct PTile
{
	int32_t X;
	int32_t Y;

	PTile(int32_t inX, int32_t inY) : X(inX), Y(inY) {}

	FVector2 GetPosition() const { return FVector2(X * TILE_SIZE, Y * TILE_SIZE); }
	void	 Draw(const PRenderer* Renderer, const FVector2& Offset) const;
	PActor*	 GetActor() const;
};

class PGrid : public PActor
{
	std::vector<PTile> mTiles;
	bool			   bDebugDraw = false;

public:
	~PGrid() override = default;
	void OnKeyUp(uint32_t KeyCode);
	void Start() override;
	void Draw(const PRenderer* Renderer) const override;

	bool   GetDebugDraw() const { return bDebugDraw; }
	void   SetDebugDraw(bool State) { bDebugDraw = State; }
	PTile* GetTileAtPosition(const FVector2& Position);
};

DECLARE_STATIC_GLOBAL_GETTER(Grid)
