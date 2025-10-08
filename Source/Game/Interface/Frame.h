#pragma once
#include "Interface/Widget.h"

struct SFrameTile
{
	FRect Source;
	FRect Dest;
};

/**
 * @brief Generic frame for most interface boxes.
 */
class PFrame : public PWidget
{
protected:
	/* Texture atlas of all interface components. */
	PTexture* mTexture = nullptr;
	/* Pre-computed vector of each tile component in the frame border.*/
	std::vector<SFrameTile> mFrameTiles;

	/**
	 * @brief Computes and stores each tile component in the frame border.
	 * @param SizeX Tile count in X.
	 * @param SizeY Tile count in Y.
	 */
	void Initialize(uint32_t SizeX, uint32_t SizeY);

public:
	/**
	 * @param Geometry Geometry of the frame in screen tiles.
	 */
	PFrame(const FRect& Geometry);
	void Draw(const PRenderer* Renderer) const override;
};