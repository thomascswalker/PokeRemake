#pragma once
#include "../Core/GameConstants.h"
#include "Engine/Sprite.h"
#include "Interface/Widget.h"

struct SFrameTile
{
	FRect	Dest;
	int32_t Index;

	SFrameTile(int32_t X, int32_t Y, int32_t Index)
		: Dest(FRect(COORD(X), COORD(Y), COORD(1), COORD(1))), Index(Index) {}
	SFrameTile(const FRect& Dest, int32_t Index)
		: Dest(Dest), Index(Index) {}
};

/**
 * @brief Generic frame for most interface boxes.
 */
class PFrame : public PWidget
{
protected:
	/* Texture atlas of all interface components. */
	std::shared_ptr<PSprite> mSprite = nullptr;
	/* Pre-computed vector of each tile component in the frame border.*/
	std::vector<SFrameTile> mFrameTiles;

	/**
	 * @brief Computes and stores each tile component in the frame border.
	 * @param Geometry Tile count in X.
	 */
	void Initialize(const FRect& Geometry);

public:
	/**
	 * @param Geometry Geometry of the frame in screen tiles.
	 */
	PFrame(const FRect& Geometry);
	void Draw(const PRenderer* Renderer) const override;
};